#include "title.h"
#include <assert.h>

void Title::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	model_ = model;
	worldTransform_.Initialize();
	textureHandle_ = textureHandle;
}

void Title::Update() { worldTransform_.TransferMatrix(); }

void Title::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}