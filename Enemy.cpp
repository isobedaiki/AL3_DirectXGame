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

}
void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}