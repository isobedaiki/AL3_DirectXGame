#pragma once
#include"Model.h"
#include"Input.h"
#include "ViewProjection.h"
#include"WorldTransform.h"
#include"EnemyBullet.h"
#include<list>
#include"GameScene.h"


enum class Phase {
	Approach,
	Leave, 
};

class Player;
class GameClass;

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
	void OnCollision();
	//const std::list<EnemyBullet*>& GetBullets() { return bullets_; }
	const float GetRadius() { return radius_; }
	const float radius_ = 1.0f;
	
	void setGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

private:
	WorldTransform worldTransform_;
	uint32_t textureHandle_ = 0u;
	Model* model_ = nullptr;
	Input* input_ = nullptr;
	Player* player_ = nullptr;
	EnemyBullet* bullet_ = nullptr;
	GameScene* gameScene_ = nullptr;
	//std::list<EnemyBullet*> bullets_;
	//Phase* phase_ = nullptr;
	int32_t pushTimer = 0;
};


