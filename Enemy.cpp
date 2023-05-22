#include "Enemy.h"

void Enemy::Initialize(Model* model, const Vector3& pos) {
	model_ = model;
	worldTransform_.Initialize();
	textureHandle_ = TextureManager::Load("zaki.png");
	worldTransform_.translation_ = pos;
}
void Enemy::Update() { 
	const float kcharacterSpeed = 0.3f;
	worldTransform_.UpdateMatrix();
	worldTransform_.translation_.z -= kcharacterSpeed;

	switch (phase_) {
	case Phase::Approach:
	default:
		worldTransform_.translation_.z -= kcharacterSpeed;
		if (worldTransform_.translation_.z < 0.0f) {
			phase_ = Phase::Leave;
		}
		break;
	case Phase::Leave:
		worldTransform_.translation_.z += kcharacterSpeed;
		break;
	}

}
void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}