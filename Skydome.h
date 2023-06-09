#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

/// <summary>
///	天球
/// </summary>
class Skydome {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model);

	/// <summary>
	///	更新
	/// </summary>
	void Update();
	
	/// /// <summary>
	///	描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

private:

	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	Skydome* skydome_ = nullptr;
};
