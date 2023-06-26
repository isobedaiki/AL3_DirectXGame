#include "RailCamera.h"
#include "ImGuiManager.h"

void RailCamera::Initialize(Vector3 pos, Vector3 rot) {
	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	worldTransform_.rotation_ = rot;

	viewProjection_.farZ = 500;
	viewProjection_.Initialize();

}

void RailCamera::Update() { 
	Vector3 move = {0.0f, 0.0f, 0.0f};
	Vector3 rot = {0.0f, 0.00f, 0.0f};

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	worldTransform_.rotation_.x += rot.x;
	worldTransform_.rotation_.y += rot.y;
	worldTransform_.rotation_.z += rot.z;

	worldTransform_.UpdateMatrix();
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
	viewProjection_.TransferMatrix();

	//ImGui::SetNextWindowSize({300, 100});

	ImGui::Begin("Camera");
	//pos
	float sliderPos[3] = {worldTransform_.translation_.x, worldTransform_.translation_.y,worldTransform_.translation_.z};
	ImGui::SliderFloat3("position", sliderPos, -20.0f, 20.0f);
	worldTransform_.translation_ = {sliderPos[0], sliderPos[1], sliderPos[2]};

	//rot
	float sliderRot[3] = {worldTransform_.rotation_.x, worldTransform_.rotation_.y,worldTransform_.rotation_.z};
	ImGui::SliderFloat3("rotation", sliderRot, -20.0f, 40.0f);
	worldTransform_.rotation_ = {sliderRot[0], sliderRot[1], sliderRot[2]};
	ImGui::End();

}
