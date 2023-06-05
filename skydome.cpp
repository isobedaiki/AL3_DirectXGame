#include "skydome.h"


void skydome::Initialize(Model* model) {
	model_ = model;
	worldTransform_.Initialize();
}

void skydome::Update() { worldTransform_.TransferMatrix(); }

void skydome::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}
