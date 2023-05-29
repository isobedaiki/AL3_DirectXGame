#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() { delete model_, delete player_, delete debugCamera_, delete enemy_; }

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	textureHandle_ = TextureManager::Load("sample.png");
	model_ = Model::Create();
	viewProjection_.Initialize();
	player_ = new Player();
	player_->Initialize(model_, textureHandle_);
	enemy_ = new Enemy();
	enemy_->Initialize(model_, {10.0f, 0.0f, 50.0f});
	enemy_->SetPlayer(player_);
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {
	player_->Update();
	enemy_->Update();

	CheckAllCollosions();

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = true;
	}
#endif // _DEBUG

	// カメラの処理
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	///
	player_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollosions() {
	Vector3 posA, posB;

	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定
	posA = player_->GetWorldPosition();

	for (EnemyBullet* bullets : enemyBullets) {
		posB = bullets->GetWorldPosition();

		float distance = (posB.x - posA.x) * (posB.x - posA.x) +
		                 (posB.y - posA.y) * (posB.y - posA.y) +
		                 (posB.z - posA.z) * (posB.z - posA.z);

		float Radius = (player_->GetRadius() + bullets->GetRadius()) *
		               (player_->GetRadius() + bullets->GetRadius());

		if (distance <= Radius) {
			player_->OnCollision();
			bullets->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
		posA = enemy_->GetWorldPosition();
	
		for (PlayerBullet* bullets : playerBullets) {
			posB = bullets->GetWorldPosition();
	
			float distance = (posB.x - posA.x) * (posB.x - posA.x) +
			                 (posB.y - posA.y) * (posB.y - posA.y) +
			                 (posB.z - posA.z) * (posB.z - posA.z);
	
			float Radius = (enemy_->radius_ + bullets->radius_) * (enemy_->radius_ + bullets->radius_);
	
			if (distance <= Radius) {
				enemy_->OnCollision();
				bullets->OnCollision();
			}
		}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	    for (PlayerBullet* bulletsA : playerBullets) {
		    for (EnemyBullet* bulletsB : enemyBullets) {

			    posA = bulletsA->GetWorldPosition();
			    posB = bulletsB->GetWorldPosition();

			    float distance = (posB.x - posA.x) * (posB.x - posA.x) +
			                     (posB.y - posA.y) * (posB.y - posA.y) +
			                     (posB.z - posA.z) * (posB.z - posA.z);

			    float Radius = (bulletsA->radius_ + bulletsB->radius_) *
			                   (bulletsA->radius_ + bulletsB->radius_);

			    if (distance <= Radius) {
				    bulletsA->OnCollision();
				    bulletsB->OnCollision();
			    }
		    }
	    }
#pragma endregion
}
