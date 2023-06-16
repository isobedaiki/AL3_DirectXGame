#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "Enemy.h"
#include"DebugCamera.h"
#include"skydome.h"
#include"RailCamera.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollosions();

	const std::list<EnemyBullet*>& GetBullets() { return enemyBullets_; }

	void AddEnemyBullet(EnemyBullet* enemyBullet);
		
private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	Model* model_ = nullptr;
	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;
	skydome* skydome_ = nullptr;
	Model* modelskydome_ = nullptr;


	ViewProjection viewProjection_;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

	bool isDebugCameraActive_ = false;
	DebugCamera* debugCamera_ = nullptr;

	RailCamera* railCamera_ = nullptr;

	std::list<EnemyBullet*> enemyBullets_;
	
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
