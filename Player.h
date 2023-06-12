#pragma once
#include"Model.h"
#include"Input.h"
#include "ViewProjection.h"
#include"WorldTransform.h"
#include"PlayerBullet.h"
#include<list>

class Player {
public:
	
	void Initialize(Model* model, uint32_t textureHandle,const Vector3& pos);
	void Update();
	void Draw(ViewProjection& viewProjection);
	void Attack();
	~Player();
	Vector3 GetWorldPosition();
	void OnCollision();

	const std::list<PlayerBullet*>& GetBullets() { return bullets_; }
	const float GetRadius() { return radius_; }
	const float radius_ = 1.0f;
	void SetParent(const WorldTransform* parent);

private:
	WorldTransform worldTransform_;
	uint32_t textureHandle_ = 0u;
	Model* model_ = nullptr;
	Input* input_ = nullptr;
	PlayerBullet* bullet_ = nullptr;
	std::list<PlayerBullet*> bullets_;

};