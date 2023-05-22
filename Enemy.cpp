#include "Enemy.h"
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
}

void Enemy::Update() 
{

	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

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

	worldTransform_.UpdateMatrix();

}

void Enemy::Draw(const ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::EnemyApproach() 
{
	float approachSpeed = 0.2f;
	worldTransform_.translation_.z += approachSpeed;
}

void Enemy::EnemyLeave() 
{
	float leaveSpeed = 0.2f;
	worldTransform_.translation_.x += leaveSpeed;
	worldTransform_.translation_.y += leaveSpeed;
	worldTransform_.translation_.z += leaveSpeed;
}