#pragma once
#include"Model.h"
#include"Input.h"
#include "ViewProjection.h"
#include"WorldTransform.h"

class Enemy {
public:
	void Initialize(Model* model, uint32_t textureHandle);
	void Update();
	void Draw(ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_;
	uint32_t textureHandle_ = 0u;
	Model* model_ = nullptr;
	Input* input_ = nullptr;
};
