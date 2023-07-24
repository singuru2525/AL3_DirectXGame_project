#pragma once
#include "Model.h"
#include "Vector3.h"
#include "Player.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Collider.h"

/// <summary>
/// 敵の弾
/// </summary>
class EnemyBullet : public Collider
{
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

	/// <summary>
	/// 弾の消失
	/// </summary>
	/// <returns></returns>
	bool IsDead() const { return isDead_; }

	// Getter
	Vector3 GetWorldPosition() override;

	// 衝突を検知したら呼び出されるコールバック関数
	void OnCollision() override;

	const float radius_ = 1.0f;

	const float GetRadius() { return radius_; }

	void SetPlayer(Player* player) { player_ = player; }

private:

	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// 速度
	Vector3 velocity_;

	// 寿命
	static const int32_t kLifeTime = 60 * 5;

	// デスタイマー
	int32_t deathTimer_ = kLifeTime;

	// デスフラグ
	bool isDead_ = false;

	Player* player_ = nullptr;

};
