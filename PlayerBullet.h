#pragma once
#include"Model.h"
#include"Vector3.h"
#include"ViewProjection.h"
#include"WorldTransform.h"

class PlayerBullet {
public:
	void Initialize(Model* model, const Vector3& pos, const Vector3& velocity);
	void Update();
	void Draw(ViewProjection& viewProjection);
	Vector3 GetWorldPosition();
	bool IsDead() const { return isDead_; } 
	void OnCollision();
	const float GetRadius() { return radius_; }
	const float radius_ = 1.0f;

private:
	WorldTransform worldTransform_;
	static const int32_t kLifetime = 60 * 5;
	int32_t deathTimer_ = kLifetime;
	bool isDead_ = false;

	Vector3 velocity_;
	Model* model_;
	uint32_t texturehandle_;
};