#include"Player.h"
#include<assert.h>
#include "ImGuiManager.h"

void Player::Initialize(Model* model, uint32_t textureHandle) {
	model_ = model;
	worldTransform_.Initialize();
	textureHandle_ = textureHandle;
	input_ = Input::GetInstance();
}

void Player::Update() { 

	Vector3 move = {0, 0, 0};
	const float kcharacterSpeed = 0.2f;

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
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	worldTransform_.matWorld_ = MakeAffineMatrix( worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);


	const float kMoveLintX = 10.0f;
	const float kMoveLintY = 10.0f;

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

}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}