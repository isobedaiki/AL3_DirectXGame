#include "EnemyBullet.h"
#include <cassert>

void EnemyBullet::Initialize(Model* model, const Vector3& pos, const Vector3& velocity) {
	assert(model);
	model_ = model;

	texturehandle_ = TextureManager::Load("uvChecker.png");

	world_.Initialize();
	world_.translation_ = pos;
	velocity_ = velocity;
}

void EnemyBullet::Update() {
	world_.translation_.x += velocity_.x;
	world_.translation_.y += velocity_.y;
	world_.translation_.z += velocity_.z;
	world_.UpdateMatrix();

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

}

void EnemyBullet::Draw(ViewProjection& viewProjection) {
	model_->Draw(world_, viewProjection, texturehandle_);
}