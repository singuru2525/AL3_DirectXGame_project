#include "Player.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <cassert>


void Player::Initialize(Model* model, uint32_t textureHandle) { 
	assert(model);

	model_ = model;

	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

void Player::Update() {
	worldTransform_.TransferMatrix();

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

	//行列更新
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	//行列転送
	worldTransform_.TransferMatrix();


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

	if (bullet_) 
	{
		bullet_->Update();
	}
}

void Player::Draw(ViewProjection viewProjection_) 
{
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	if (bullet_) 
	{
		bullet_->Draw(viewProjection_);
	}
}

void Player::Rotate() 
{
	const float kRotSpeed = 0.02f;

	// 押した方向で移動ベクトルを変更
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
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_,worldTransform_.translation_);

		bullet_ = newBullet;
	}

}