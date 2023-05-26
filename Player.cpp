#include"Player.h"
#include<assert.h>
#include "ImGuiManager.h"
#include"MathUtility.h"

void Player::Initialize(Model* model, uint32_t textureHandle) {
	model_ = model;
	worldTransform_.Initialize();
	textureHandle_ = textureHandle;
	input_ = Input::GetInstance();
}

void Player::Update() { 

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

	worldTransform_.matWorld_ = MakeAffineMatrix( worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	const float kMoveLintX = 15.0f;
	const float kMoveLintY = 15.0f;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLintX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLintX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLintY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLintY);

	worldTransform_.TransferMatrix();

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

	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {

		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_,velocity);
		bullet_ = newBullet;

		bullets_.push_back(newBullet);
	}
}

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

Vector3 Player::GetWorldPosition() { 
	Vector3 worldPos;
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos; 
}

