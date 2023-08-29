#pragma once
#include"Model.h"
#include"Input.h"
#include "ViewProjection.h"
#include"WorldTransform.h"
#include"PlayerBullet.h"
#include<list>
#include"Sprite.h"


class title {
public:
	void Initialize(Model* model, uint32_t titletextureHandle);

	void Update();

	void Draw(ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_;
	uint32_t titletextureHandle_ = 0u;
	Model* model_ = nullptr;
};
