#include "Enemy.h"
#include "Player.h"
#include<assert.h>
#include<cmath>
#include "ImGuiManager.h"
#include"MathUtility.h"
#include"GameScene.h"

Enemy::~Enemy() {
}

void Enemy::Initialize(Model* model, const Vector3& pos) {
	model_ = model;
	worldTransform_.Initialize();
	textureHandle_ = TextureManager::Load("zaki.png");
	worldTransform_.translation_ = pos;
	Approach();
}

void Enemy::Update() { 
	

	switch (phase_) {
	case Phase::Approach:
	default:
		ApproachUpdate();
		break;
	case Phase::Leave:
		LeaveUpdate();
		break;
	}
	
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::ApproachUpdate() {
	worldTransform_.translation_.z -= 0.0f;
	// 発射タイマーをデクリメント
	pushTimer--;
	// 指定時間に達した
	if (pushTimer <= 0) {
		// 弾を発射
		Fire();
		// 発射タイマーを初期化
		pushTimer = KFireInterval;
	}
}

void Enemy::LeaveUpdate() {
	worldTransform_.translation_.x -= 0.2f;
	worldTransform_.translation_.y += 0.2f;

	if (deathTimer_-- < 0) {
		isDead_ = true;
	}
}

void Enemy::Fire() {
	assert(player_);

	const float kBulletSpeed = 1.5f;

	Vector3 playerPos;
	Vector3 move;
	playerPos = player_->Player::GetWorldPosition();
	move.x = player_->GetWorldPosition().x - GetWorldPosition().x;
 	move.y = player_->GetWorldPosition().y - GetWorldPosition().y;
	move.z = player_->GetWorldPosition().z - GetWorldPosition().z;

	float length = sqrtf(move.x * move.x + move.y * move.y + move.z * move.z);

	Vector3 dir = {
		move.x /length,
		move.y /length,
		move.z /length
	};

	Vector3 velocity(dir.x * kBulletSpeed, dir.y * kBulletSpeed, dir.z * kBulletSpeed);


	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	gameScene_->AddEnemyBullet(newBullet);
}

void Enemy::Approach() { 
	pushTimer = KFireInterval; 
}

void Enemy::OnCollision() { isDead_ = true; }

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
}

