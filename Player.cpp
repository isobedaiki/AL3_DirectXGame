#include "Player.h"
#include <assert.h>

void Player::Initialize(Model* model,uint32_t textureHandle) { 
	assert(model);

	model_ = model;
	worldTransform_.Initialize();
	textureHandle_ = textureHandle;
}

void Player::Update() { worldTransform_.TransferMatrix(); }

void Player::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
