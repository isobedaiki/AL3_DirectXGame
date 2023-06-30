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
	sprite2DReticle_ = Sprite::Create(textureReticle, {640, 360}, {1, 1, 1, 1}, {(0.5f), (0.5f)});

	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;

	// 3Dレティクルのワールドトランスフォーム初期化
	worldTransform3Dreticle_.Initialize();


}

void Player::Update(ViewProjection& viewProjection) { 

	Vector3 positionReticle = worldTransform3Dreticle_.translation_;

	// ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matviewProjectionViewport =
	    viewProjection.matView * viewProjection.matProjection * matViewport;

	// ワールド→スクリーン座標行列(ここで2Dから3Dになる)
	positionReticle = Transform(positionReticle, matviewProjectionViewport);

	// スプライトのレティクルに座標設定
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

	Vector3 move = {0, 0, 0};
	const float kcharacterSpeed = 0.2f;

	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

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

	const float MatRotSpeed = 0.02f;
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= MatRotSpeed;
	} 
	else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += MatRotSpeed;
	}

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	
	const float kMoveLintX = 20.0f;
	const float kMoveLintY = 20.0f;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLintX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLintX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLintY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLintY);

	worldTransform_.UpdateMatrix();

	// 自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 50.0f;
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


	ImGui::Begin("Debug");
	float sliderValue[3] = { worldTransform_.translation_.x, worldTransform_.translation_.y,worldTransform_.translation_.z};
	ImGui::SliderFloat3("position", sliderValue, -20.0f, 20.0f); 
	worldTransform_.translation_ = {sliderValue[0], sliderValue[1], sliderValue[2]};
	ImGui::End();

	Attack();
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	//model_->Draw(worldTransform3Dreticle_, viewProjection, textureHandle_);

	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::DrawUI() { sprite2DReticle_->Draw();}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {

		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

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


