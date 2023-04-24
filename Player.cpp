#include "Player.h"
#include <cassert>

struct Matrix4x4 
{
	float m[4][4];
};

Matrix4x4 Scale(Matrix4x4 matrix1, Matrix4x4 matrix2)
{
	Matrix4x4 result;
	result.m[0][0] = matrix1.m[0][0] * matrix2.m[0][0] +
	                 matrix1.m[0][1] * matrix2.m[1][0] +
	                 matrix1.m[0][2] * matrix2.m[2][0] +
	                 matrix1.m[0][3] * matrix2.m[3][0] ;
	result.m[0][1] = matrix1.m[0][0] * matrix2.m[0][1] +
		             matrix1.m[0][1] * matrix2.m[1][1] +
	                 matrix1.m[0][2] * matrix2.m[2][1] +
		             matrix1.m[0][3] * matrix2.m[3][1] ;
	result.m[0][2] = matrix1.m[0][0] * matrix2.m[0][2] +
		             matrix1.m[0][1] * matrix2.m[1][2] +
	                 matrix1.m[0][2] * matrix2.m[2][2] + 
		             matrix1.m[0][3] * matrix2.m[3][2] ;
}

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
		move.y -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y += kCharacterSpeed;
	}

	// 座標移動
	worldTransform_.translation_.x = move.x + worldTransform_.translation_.x;
	worldTransform_.translation_.y = move.y + worldTransform_.translation_.y;
	worldTransform_.translation_.z = move.z + worldTransform_.translation_.z;


}

void Player::Draw(ViewProjection viewProjection_) 
{
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}

