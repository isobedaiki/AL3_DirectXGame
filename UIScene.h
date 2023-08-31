﻿#pragma once
#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

/// <summary>
/// ゲームシーン
/// </summary>
class UIScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	UIScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~UIScene();

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

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	// スプライト
	Sprite* sprite_ = nullptr;
	Sprite* sprite2_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	uint32_t textureHandle2_ = 0;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
