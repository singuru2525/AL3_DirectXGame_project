#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
/// <summary>
/// 自キャラ
/// </summary>
class Player 
{
public:
	/// <summary>
	///	初期化
	/// </summary>
	///  <param name="model">モデル</param>
	///  <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	///	更新
	/// </summary>
	void Update();
	
	/// /// <summary>
	///	描画
	/// </summary>
	void Draw(ViewProjection viewProjection_);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
};
