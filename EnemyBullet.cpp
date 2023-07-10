#include "EnemyBullet.h"
#include <cassert>
#include <cmath>
#include <iostream>

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& Velocity) 
{
	assert(model);

	model_ = model;

	velocity_ = Velocity;

	// 大きさ変更
	worldTransform_.scale_.x = 0.5f;
	worldTransform_.scale_.y = 0.5f;
	worldTransform_.scale_.z = 3.0f;

	// Y軸周り角度
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);

	// お祈り
	worldTransform_.translation_ = worldTransform_.rotation_;

	// X軸周り角度
	worldTransform_.rotation_.x = std::atan2(velocity_.y, velocity_.z);


	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("white1x1.png");

	worldTransform_.Initialize();

	// 引数で受け取った初期値をセット
	worldTransform_.translation_ = position;
}

void EnemyBullet::Update() 
{

	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

	// 弾の消失判定
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}

	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) 
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}


Vector3 EnemyBullet::GetWorldPosition() {

	Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}


void EnemyBullet::OnCollision() 
{
	isDead_ = true; 
}
