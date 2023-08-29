#pragma once
#include"Model.h"
#include"Input.h"
#include"Vector3.h"
#include "ViewProjection.h"
#include"WorldTransform.h"
#include"EnemyBullet.h"
#include<list>

class Player;
class GameScene;

enum class Phase {
	Approach,
	Leave, 
};


class Enemy {
public:

	~Enemy();

	void Initialize(Model* model, const Vector3& pos);
	void Update();
	void Draw(ViewProjection& viewProjection);
    // 接近
	void ApproachUpdate();
	// 離脱
	void LeaveUpdate();

	void Fire();

	static const int KFireInterval = 60;

    void Approach();

    void SetPlayer(Player* player) { player_ = player; }

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }
	
	void OnCollision();
	
	bool IsDead() const { return isDead_; }

	Vector3 GetWorldPosition();
	
	//const std::list<EnemyBullet*>& GetBullets() { return bullets_; }
	const float GetRadius() { return radius_; }
	const float radius_ = 1.0f;

private:
	WorldTransform worldTransform_;
	uint32_t textureHandle_ = 0u;
	Model* model_ = nullptr;

	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
	};
	Phase phase_ = Phase::Approach;

	Input* input_ = nullptr;
	Player* player_ = nullptr;

	GameScene* gameScene_ = nullptr;

	int32_t pushTimer = 0;

	int32_t deathTimer_ = 300;
	// デスフラグ
	bool isDead_ = false;
};


