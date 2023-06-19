﻿#include "Player.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <cassert>


void Player::Initialize(Model* model, uint32_t textureHandle, Vector3 playerPosition) { 
	assert(model);

	model_ = model;

	textureHandle_ = textureHandle;

	worldTransform_.translation_ = playerPosition;

	worldTransform_.Initialize();

	worldTransform3DReticle_.Initialize();

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

void Player::Update() {

	bullets_.remove_if([](PlayerBullet* bullet) {

		if (bullet->IsDead()) 
		{
			delete bullet;
			return true;
		}
		return false;
	});

	worldTransform_.TransferMatrix();

	// 自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 50.0f;
	// 自機から3Dレティクルへのオフセット(Z+向き)
	Vector3 offset = {0, 0, 1.0f};
	// 自機のワールド行列の回転を反映
	offset = TransformNormal(offset, worldTransform_.matWorld_);
	// ベクトルの長さを整える
	offset = Normalize(offset) * kDistancePlayerTo3DReticle;
	// 3dレティクルの座標を設定
	worldTransform3DReticle_.translation_.x = GetWorldPosition().x + offset.x;
	worldTransform3DReticle_.translation_.y = GetWorldPosition().y + offset.y;
	worldTransform3DReticle_.translation_.z = GetWorldPosition().z + offset.z;
	worldTransform3DReticle_.UpdateMatrix();


	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	// 押した方向でベクトルを変更 (左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
	// 押した方向でベクトルを変更 (上下)
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	//範囲制限
	const float kMobeLimitX = 35.0f;
	const float kMobeLimitY = 35.0f;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMobeLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMobeLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMobeLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMobeLimitY);

	// 座標移動
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	worldTransform_.UpdateMatrix();


	//ImGui
	ImGui::SetNextWindowPos({0,0});
	ImGui::SetNextWindowSize({300, 100});

	ImGui::Begin("Player");
	float silderValue[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	ImGui::SliderFloat3("position", silderValue, -20.0f, 20.0f);
	worldTransform_.translation_ = {silderValue[0], silderValue[1], silderValue[2]};
	ImGui::End();


	Rotate();

	Attack();

	for (PlayerBullet* bullet : bullets_) 
	{
		bullet->Update();
	}
}

void Player::Draw(ViewProjection viewProjection_) 
{
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	for (PlayerBullet* bullet : bullets_)
	{
		bullet->Draw(viewProjection_);
	}

	model_->Draw(worldTransform3DReticle_, viewProjection_);
}

void Player::Rotate() 
{
	const float kRotSpeed = 0.02f;

	// 押した方向で回転ベクトルを変更
	if (input_->PushKey(DIK_A)) 
	{
		worldTransform_.rotation_.y += kRotSpeed; 
	} else if (input_->PushKey(DIK_D)) 
	{
		worldTransform_.rotation_.y -= kRotSpeed; 
	}
}

void Player::Attack() 
{

	if (input_->TriggerKey(DIK_SPACE)) 
	{
		
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);

		bullets_.push_back(newBullet);
	
	}

}

Vector3 Player::GetWorldPosition() 
{

	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;

}


Player::~Player() 
{ 
	for (PlayerBullet* bullet : bullets_) 
	{
		delete bullet;
	}
}

void Player::OnCollision() {}

void Player::SetParent(const WorldTransform* parent) 
{
	worldTransform_.parent_ = parent; 
}
