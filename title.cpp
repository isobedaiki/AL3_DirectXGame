#include "title.h"
#include <assert.h>

void title::Initialize(Model* model, uint32_t titletextureHandle) {
	assert(model);

	model_ = model;
	worldTransform_.Initialize();
	titletextureHandle_ = titletextureHandle;
}

void title::Update() { worldTransform_.TransferMatrix(); }

void title::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, titletextureHandle_);
}