#include "Enemy.h"
#include "Player.h"
#include<assert.h>
#include<cmath>
#include "ImGuiManager.h"
#include"MathUtility.h"


void Enemy::Initialize(Model* model, const Vector3& pos) {
	model_ = model;
	worldTransform_.Initialize();
	textureHandle_ = TextureManager::Load("zaki.png");
	worldTransform_.translation_ = pos;
	//Fire();
	Approach();
}

void Enemy::Update() { 

	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	//const float kcharacterSpeed = 0.3f;
	worldTransform_.UpdateMatrix();
	//worldTransform_.translation_.z -= kcharacterSpeed;

	switch (phase_) {
	case Phase::Approach:
	default:
		/*worldTransform_.translation_.z -= kcharacterSpeed;
		if (worldTransform_.translation_.z < 0.0f) {
			phase_ = Phase::Leave;
		}*/
		pushTimer--;
		if (pushTimer <= 0) {
			Fire();
			pushTimer = KFireInterval;
		}
		break;
	case Phase::Leave:
		/*worldTransform_.translation_.z += kcharacterSpeed;*/
		break;
	}

	
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}
}

void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

}

void Enemy::Fire() {
	assert(player_);

	const float kBulletSpeed = 1.0f;

	/*player_->GetWorldPosition();
	GetWorldPosition();*/
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

	//velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);
	//bullet_ = newBullet;

	bullets_.push_back(newBullet);
}

void Enemy::Approach() { 
	pushTimer = KFireInterval; 
}

Enemy::~Enemy() {
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
}

void Enemy::OnCollision() {}
