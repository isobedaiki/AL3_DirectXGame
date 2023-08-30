#include"Player.h"
#include<assert.h>
#include "ImGuiManager.h"
#include"Input.h"
#include"MathUtility.h"

void Player::Initialize(Model* model, uint32_t textureHandle,const Vector3& pos) {
	model_ = model;
	
	textureHandle_ = textureHandle;
	input_ = Input::GetInstance();

	// レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("target.png");

	// スプライト生成
	sprite2DReticle_ = Sprite::Create(textureReticle, spritePos, {1, 1, 1, 1}, {(0.5f), (0.5f)});

	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;

	// 3Dレティクルのワールドトランスフォーム初期化
	worldTransform3Dreticle_.Initialize();


}

void Player::Update(ViewProjection& viewProjection) { 

	//Vector3 positionReticle = worldTransform3Dreticle_.translation_;

	//// ビューポート行列
	//Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	//// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	//Matrix4x4 matviewProjectionViewport =
	//    viewProjection.matView * viewProjection.matProjection * matViewport;

	//// ワールド→スクリーン座標行列(ここで2Dから3Dになる)
	//positionReticle = Transform(positionReticle, matviewProjectionViewport);

	//// スプライトのレティクルに座標設定
	//sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

	// ビューポート行列
	Matrix4x4 matViewport =
	    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	POINT mousePosision;
	// マウス座標(スクリーン座標)を取得する
	GetCursorPos(&mousePosision);

	// クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosision);

	// マウス座標を2Dレティクルのスプライトに代入する
	sprite2DReticle_->SetPosition(Vector2(float(mousePosision.x), float(mousePosision.y)));

	// ビュープロジェクションビューポート合成行列
	Matrix4x4 matVPV = viewProjection.matView * viewProjection.matProjection * matViewport;

	// 合成行列の逆行列を計算する
	Matrix4x4 matInverseVPV = Inverse(matVPV);

	// スクリーン座標
	Vector3 posNear = Vector3(float(mousePosision.x), float(mousePosision.y), 0);
	Vector3 posFar = Vector3(float(mousePosision.x), float(mousePosision.y), 1);

	// スクリーン座標系からワールド座標系へ
	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);

	// マウスレイの方向
	Vector3 mouseDirection = {posFar.x - posNear.x, posFar.y - posNear.y, posFar.z - posNear.z};
	mouseDirection = Normalize(mouseDirection);

	// カメラから照準オブジェクトの距離
	const float kDistanceTestObject = 70.0f;
	worldTransform3Dreticle_.translation_.x = (posNear.x + mouseDirection.x) * kDistanceTestObject;
	worldTransform3Dreticle_.translation_.y = (posNear.y + mouseDirection.y) * kDistanceTestObject;
	worldTransform3Dreticle_.translation_.z = (posNear.z + mouseDirection.z) * kDistanceTestObject;


	Vector3 move = {0, 0, 0};
	const float kcharacterSpeed = 0.2f;

	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	//矢印キー
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kcharacterSpeed;
	} 
	else if (input_ -> PushKey(DIK_RIGHT)) {
		move.x += kcharacterSpeed;
	} 
	else if (input_->PushKey(DIK_UP)) {
		move.y += kcharacterSpeed;
	} 
	else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kcharacterSpeed;
	}

	//WASD
	if (input_->PushKey(DIK_A)) {
		move.x -= kcharacterSpeed;
	} else if (input_->PushKey(DIK_D)) {
		move.x += kcharacterSpeed;
	} else if (input_->PushKey(DIK_W)) {
		move.y += kcharacterSpeed;
	} else if (input_->PushKey(DIK_S)) {
		move.y -= kcharacterSpeed;
	}

	const float MatRotSpeed = 0.02f;
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= MatRotSpeed;
	} 
	else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += MatRotSpeed;
	}

	Attack();
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	
	const float kMoveLintX = 15.0f;
	const float kMoveLintY = 15.0f;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLintX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLintX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLintY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLintY);

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	//worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.UpdateMatrix();

	// 自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 40.0f;
	// 自機から3Dレティクルへのオフセット(Z+向き)
	Vector3 offset = {0, 0, 1.0f};
	// 自機のワールド行列の回転を反映
	offset = TransformNormal(offset, worldTransform_.matWorld_);
	// ベクトルの長さを整える
	float length = sqrtf((offset.x * offset.x) + (offset.y * offset.y) + (offset.z * offset.z));
	Vector3 dir(offset.x / length, offset.y / length, offset.z / length);
	offset = {
	    dir.x * kDistancePlayerTo3DReticle,
	    dir.y * kDistancePlayerTo3DReticle,
	    dir.z * kDistancePlayerTo3DReticle,
	};
	// 3Dレティクルの座標を設定
	worldTransform3Dreticle_.translation_ = {
	    worldTransform_.translation_.x + offset.x, worldTransform_.translation_.y + offset.y,
	    worldTransform_.translation_.z + offset.z};
	worldTransform3Dreticle_.UpdateMatrix();

	#ifdef DEBUG
	ImGui::SetNextWindowSize({300, 100});
    ImGui::Begin("Debug");
	float sliderValue[3] = { worldTransform_.translation_.x, worldTransform_.translation_.y,worldTransform_.translation_.z};
	ImGui::SliderFloat3("position", sliderValue, -20.0f, 20.0f); 
	worldTransform_.translation_ = {sliderValue[0], sliderValue[1], sliderValue[2]};
	ImGui::End();
	#endif // DEBUG

	/*ImGui::Begin("Player");
	ImGui::Text("2DReticle:(%f,%f)", spritePos.x, spritePos.y);
	ImGui::Text("Near:(%+.2f,%+.2f,%+.2f)", posNear.x, posNear.y, posNear.z);
	ImGui::Text("Far:(%+.2f,%+.2f,%+.2f)", posFar.x, posFar.y, posFar.z);
	ImGui::Text("3DReticle(%+.2f,%+.2f,%+.2f)", worldTransform3Dreticle_.translation_.x,worldTransform3Dreticle_.translation_.y, worldTransform3Dreticle_.translation_.z);
	ImGui::End();*/
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	//model_->Draw(worldTransform3Dreticle_, viewProjection, textureHandle_);

	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::DrawUI() { sprite2DReticle_->Draw();}

//攻撃　　後に右クリックにする　
void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {

		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		
		// velocity = TransFormNormal(velocity, worldTransform_.matWorld_);
		velocity = {
		    worldTransform3Dreticle_.translation_.x - worldTransform_.translation_.x,
		    worldTransform3Dreticle_.translation_.y - worldTransform_.translation_.y,
		    worldTransform3Dreticle_.translation_.z - worldTransform_.translation_.z,
		};
		Vector3 n = Normalize(velocity);
		velocity.x = n.x * kBulletSpeed;
		velocity.y = n.y * kBulletSpeed;
		velocity.z = n.z * kBulletSpeed;

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);
		//bullet_ = newBullet;

		bullets_.push_back(newBullet);
	}
}

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	delete sprite2DReticle_;
}

Vector3 Player::GetWorldPosition() { 
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos; 
}

void Player::OnCollision() {}

void Player::SetParent(const WorldTransform* parent) { 
	worldTransform_.parent_ = parent;
}


