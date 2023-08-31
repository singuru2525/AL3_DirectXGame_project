#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"
#include "ImGuiManager.h"
#include "Matrix4x4.h"
#include "PlayerBullet.h"
#include "MathUtility.h"
#include "Sprite.h"
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
	void Initialize(Model* model, uint32_t textureHandle, Vector3 playerPosition);

	/// <summary>
	///	更新
	/// </summary>
	void Update(ViewProjection &viewProjection);
	
	/// /// <summary>
	///	描画
	/// </summary>
	void Draw(ViewProjection &viewProjection);

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

	/// <summary>
	/// 親となるワールドトランスフォームをセット
	/// </summary>
	/// <param name="parent">親となるワールドトランスフォーム</param>
	void SetParent(const WorldTransform* parent);


	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI();

	// 撃破消滅
	bool IsDead() const { return isDead_; }

	// デスフラグ
	bool isDead_ = false;

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

	// 初期位置
	Vector3 playerPosition_{};

	WorldTransform worldTransform3DReticle_;

	Sprite* sprite2DReticle_ = nullptr;

	// ビュープロジェクション
	ViewProjection viewProjection_;

};
