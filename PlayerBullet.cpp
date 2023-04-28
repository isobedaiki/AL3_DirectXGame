#include"PlayerBullet.h"
#include <cassert>

void PlayerBullet::Initialize(Model* model, const Vector3& pos) { 
	assert(model);
	model_ = model;

	texturehandle_ = TextureManager::Load("uvChecker.png");

	world_.Initialize();
	world_.translation_ = pos;
}

void PlayerBullet::Update() {
	world_.UpdateMatrix();
}

void PlayerBullet::Draw(ViewProjection& viewProjection) {
	model_->Draw(world_, viewProjection, texturehandle_);
}
