#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>
#include"EnemyBullet.h"
#include<fstream>



GameScene::GameScene() { delete model_, delete title_; }

GameScene::~GameScene() {
	delete model_, delete player_, delete debugCamera_, delete skydome_, delete modelskydome_,
	    delete railCamera_;
	for (Enemy* enemy : enemyes_) {
		delete enemy;
	}
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}

}


void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	textureHandle_ = TextureManager::Load("player.png");
	
	model_ = Model::Create();
	viewProjection_.Initialize();
	TextureManager::Load("target.png");
	Vector3 playerPosition(0, 0, 40);
	player_ = new Player();
	player_->Initialize(model_, textureHandle_,playerPosition);
	//enemy_->setGameScene(this);
	modelskydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new skydome();
	skydome_->Initialize(modelskydome_);
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	railCamera_ = new RailCamera();
	railCamera_->Initialize({0.0f, 0.0f,0.0f}, {0.0f,0.0f,0.0f});
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	player_->SetParent(&railCamera_->GetWorldTransform());
	title_ = new Title();
	title_->Initialize(model_, textureHandle_);

	
	//AddEnemy({0,3,80,});
	LoadEnemypopData();
}

void GameScene::Update() {
	player_->Update(viewProjection_);
	skydome_->Update();
	title_->Update();
	railCamera_->Update();
	debugCamera_->Update();
	UpdateEnemyPopCommands();

	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	enemyes_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

	// 弾更新
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}
	for (Enemy* enemy : enemyes_) {
		enemy->Update();
	}


//#ifdef _DEBUG
//	if (input_->TriggerKey(DIK_0)) {
//		isDebugCameraActive_ = true;
//	}
//#endif // _DEBUG



	// カメラの処理
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		//viewProjection_.UpdateMatrix();
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}

	CheckAllCollosions();
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
	title_->Draw(viewProjection_);

    for (Enemy* enemy : enemyes_) {
		enemy->Draw(viewProjection_);
	}

	skydome_->Draw(viewProjection_);

	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(viewProjection_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	player_->DrawUI();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::AddEnemy(Vector3 pos) {
	Enemy* enemy = new Enemy();
	enemy->Initialize(model_, pos);
	enemy->SetPlayer(player_);
	enemy->SetGameScene(this);
	enemyes_.push_back(enemy);
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) { enemyBullets_.push_back(enemyBullet); }

void GameScene::CheckAllCollosions() {
	Vector3 posA, posB;

	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

	const std::list<EnemyBullet*>& enemyBullets = enemyBullets_;

#pragma region 自キャラと敵弾の当たり判定
	posA = player_->GetWorldPosition();

	for (EnemyBullet* bullets : enemyBullets) {
		posB = bullets->GetWorldPosition();

		float distance = (posB.x - posA.x) * (posB.x - posA.x) +
		                 (posB.y - posA.y) * (posB.y - posA.y) +
		                 (posB.z - posA.z) * (posB.z - posA.z);

		/*float Radius = (player_->GetRadius() + bullets->GetRadius()) *
		               (player_->GetRadius() + bullets->GetRadius());*/
		for (Enemy* enemy : enemyes_) {
			if (distance <= enemy->GetRadius() + player_->GetRadius()) {
				player_->OnCollision();
				bullets->OnCollision();
			}
		}
	}

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	for (Enemy* enemy : enemyes_) {
		posA = enemy->GetWorldPosition();

		for (PlayerBullet* bullets : playerBullets) {
			posB = bullets->GetWorldPosition();

			float distance = (posA.x - posB.x) * (posA.x - posB.x) +
			                 (posA.y - posB.y) * (posA.y - posB.y) +
			                 (posA.z - posB.z) * (posA.z - posB.z);

			/*float Radius =
			    (enemy->radius_ + bullets->radius_) * (enemy->radius_ + bullets->radius_);*/

			if (distance <= enemy->GetRadius() + player_->GetRadius()) {
				enemy->OnCollision();
				bullets->OnCollision();
			}
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

			/*float Radius =
			    (bulletsA->radius_ + bulletsB->radius_) * (bulletsA->radius_ + bulletsB->radius_);*/

			for (Enemy* enemy : enemyes_) {
				if (distance <= enemy->GetRadius() + player_->GetRadius()) {
					bulletsA->OnCollision();
					bulletsB->OnCollision();
				}
			}
		}
	}
#pragma endregion

}

void GameScene::LoadEnemypopData() {
	// ファイルを開く
	std::ifstream file;
	file.open("Resources/enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands() {
	// 待機処理
	if (waitFlag) {
		waitTimer--;
		if (waitTimer <= 0) {
			// 待機完了
			waitFlag = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		if (word.find("//") == 0) {
			// コメントアウト行を飛ばす
			continue;
		}
		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			AddEnemy(Vector3(x, y, z));
		}
		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機時間
			waitFlag = true;
			waitTimer = waitTime;

			// コマンドのループを抜ける
			break;
		}
	}
}
