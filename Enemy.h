#pragma once
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

/// <summary>
/// 敵
/// </summary>
class Enemy {

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="position"></param>
	/// <param name="Velocity"></param>
	void Initialize(Model* model, const Vector3& position, const Vector3& Velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection);

private:

	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// 速度
	Vector3 velocity_;


};
