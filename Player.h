#pragma once
#include"Model.h"
#include"Input.h"
#include "ViewProjection.h"
#include"WorldTransform.h"
#include"PlayerBullet.h"
#include<list>
#include"Sprite.h"

class Player {
public:
	
	void Initialize(Model* model, uint32_t textureHandle,const Vector3& pos);
	void Update(ViewProjection& viewProjection);
	void Draw(ViewProjection& viewProjection);
	void DrawUI();
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
	//PlayerBullet* bullet_ = nullptr;
	std::list<PlayerBullet*> bullets_;
	// 3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3Dreticle_;

	// 2Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;
};