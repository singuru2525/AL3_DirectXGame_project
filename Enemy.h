#pragma once
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "MathUtility.h"
#include <list>
#include "EnemyBullet.h"

enum class Phase {
	Approach, // 接近する
	Leave,    // 離脱する
};

class Player;

class GameScene;

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


	void SetPlayer(Player* player) { player_ = player; }


	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection);

	// 接近関数
	void EnemyApproach();

	// 離脱関数
	void EnemyLeave();

	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

	// Getter
	Vector3 GetWorldPosition();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy();

	// 発射間隔
	static const int kFireInterval = 30;

	// 接近フェーズ初期化
	void ApproachInitialize();

	// 衝突を検知したら呼び出されるコールバック関数
	void OnCollision();

	const float radius_ = 1.0f;

	const float GetRadius() { return radius_; }

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

private:

	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// 速度
	Vector3 velocity_;

	// フェーズ
	Phase phase_ = Phase::Approach;

		// 弾のリスト
	std::list<EnemyBullet*> bullets_;

	// 発射タイマー
	int32_t fireTimer_;

	Player* player_ = nullptr;

	GameScene* gameScene_ = nullptr;

};
