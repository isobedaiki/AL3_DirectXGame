#pragma once
#include"Model.h"
#include"Input.h"
#include "ViewProjection.h"
#include"WorldTransform.h"
#include"EnemyBullet.h"
#include<list>


enum class Phase {
	Approach,
	Leave, 
};

class Player;

class Enemy {
public:
	void Initialize(Model* model, const Vector3& pos);
	void Update();
	void Draw(ViewProjection& viewProjection);
	void Fire();
	Phase phase_ = Phase::Approach;
	~Enemy();
	static const int KFireInterval = 60;
	void Approach();
	void SetPlayer(Player* player) { player_ = player; }
	Vector3 GetWorldPosition();
	Vector3 move;

private:
	WorldTransform worldTransform_;
	uint32_t textureHandle_ = 0u;
	Model* model_ = nullptr;
	Input* input_ = nullptr;
	Player* player_ = nullptr;
	EnemyBullet* bullet_ = nullptr;
	std::list<EnemyBullet*> bullets_;
	//Phase* phase_ = nullptr;
	int32_t pushTimer = 0;
};


