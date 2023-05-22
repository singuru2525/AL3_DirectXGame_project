#include "Enemy.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <cassert>

void Enemy::Initialize(Model* model, const Vector3& position, const Vector3& Velocity) {
	assert(model);

	model_ = model;

	velocity_ = Velocity;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("enemy.png");

	worldTransform_.Initialize();

	// 引数で受け取った初期値をセット
	worldTransform_.translation_ = position;

	// 接近フェーズの初期化呼び出し
	ApproachInitialize();

}

void Enemy::Update() 
{

	bullets_.remove_if([](EnemyBullet* bullet) 
	{
		if (bullet->IsDead())
		{
			delete bullet;
			return true;
		}
		return false;
	});

	switch (phase_) {
	case Phase::Approach:
		// 移動
		EnemyApproach();
		// 規定の位置に到達したら離脱
		if (worldTransform_.translation_.z < 0.0f) {
			phase_ = Phase::Leave;
		}
		break;
	case Phase::Leave:
		EnemyLeave();
		break;
	default:
		break;
	}



	// 弾の更新呼び出し
	for (EnemyBullet* bullet : bullets_) 
	{
		bullet->Update();
	}

	worldTransform_.UpdateMatrix();

}

void Enemy::Draw(const ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// 弾の描画呼び出し
	for (EnemyBullet* bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}
}

// 敵の接近
void Enemy::EnemyApproach() 
{
	float approachSpeed = 0.4f;
	worldTransform_.translation_.z -= approachSpeed;

	--fireTimer_; 

	if (fireTimer_ <= 0) 
	{
		// 弾を発射
		Fire();
		// 発射タイマーを初期化
		fireTimer_ = kFireInterval;
	}
}
 
// 敵の離脱
void Enemy::EnemyLeave() 
{
	float leaveSpeed = 0.2f;
	worldTransform_.translation_.x += leaveSpeed;
}

// 接近フェーズの初期化
void Enemy::ApproachInitialize() 
{
	// 発射タイマーを初期化
	fireTimer_ = kFireInterval;
}

// 弾の発射関数
void Enemy::Fire() 
{
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	bullets_.push_back(newBullet);

}

// デストラクタ
Enemy::~Enemy()
{
	for (EnemyBullet* bullet : bullets_)
	{
		delete bullet;
	}
}