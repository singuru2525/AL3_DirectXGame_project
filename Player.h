#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"
#include "ImGuiManager.h"
#include "Matrix4x4.h"
#include "PlayerBullet.h"
#include "MathUtility.h"
#include <list>


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

	void Rotate();

	void Attack();

	// Getter
	Vector3 GetWorldPosition();

	const float radius_ = 1.0f;

	const float GetRadius() { return radius_; }

	// 弾リストを取得
	const std::list<PlayerBullet*>& GetBullet() { return bullets_; }

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	// 衝突を検知したら呼び出されるコールバック関数
	void OnCollision();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//キーボード入力
	Input* input_ = nullptr;

	std::list<PlayerBullet*> bullets_;// 弾のリスト

	
};
