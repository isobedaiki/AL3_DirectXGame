#include "Enemy.h"
#include<assert.h>
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

	const float kBulletSpeed = 1.0f;
	Vector3 velocity(kBulletSpeed, 0, 0);

	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);
	bullet_ = newBullet;

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