#pragma once
#include"Model.h"
#include"Input.h"
#include "ViewProjection.h"
#include"WorldTransform.h"
#include"PlayerBullet.h"
#include<list>

class Player {
public:

	void Initialize(Model* model, uint32_t textureHandle);
	void Update();
	void Draw(ViewProjection& viewProjection);
	void Attack();
	~Player();

private:
	WorldTransform worldTransform_;
	uint32_t textureHandle_ = 0u;
	Model* model_ = nullptr;
	Input* input_ = nullptr;
	PlayerBullet* bullet_ = nullptr;
	std::list<PlayerBullet*> bullets_;
};