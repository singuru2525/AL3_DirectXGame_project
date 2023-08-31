#include "Player.h"
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

	// レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("target.png");

	// スプライト生成
	sprite2DReticle_ = Sprite::Create(
	    textureReticle,
	    {640,360},
	    {1, 1, 1, 1},
		{0.5f, 0.5f});

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

void Player::Update(ViewProjection &viewProjection)
{



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
	// 3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_.x = GetWorldPosition().x + offset.x;
	worldTransform3DReticle_.translation_.y = GetWorldPosition().y + offset.y;
	worldTransform3DReticle_.translation_.z = GetWorldPosition().z + offset.z;
	worldTransform3DReticle_.UpdateMatrix();


	// 3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算	
	Vector3 positionReticle = worldTransform3DReticle_.translation_;

	// ビューポート行列
	Matrix4x4 matViewport = 
		MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport =
	    viewProjection.matView * viewProjection.matProjection * matViewport;

	// ワールド->スクリーン座標変換 (ここで3Dから2Dになる)
	positionReticle = Transform(positionReticle, matViewProjectionViewport);

	// スプライトのレティクルに座標変換
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));


	// マウス座標
	POINT mousePosition;

	// マウス座標を取得する
	GetCursorPos(&mousePosition);
	// クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);
	// マウス座標を2Dレティクルのスプライトに代入する
	sprite2DReticle_->SetPosition(Vector2(float(mousePosition.x), float(mousePosition.y)));

	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matVPV = viewProjection.matView * viewProjection.matProjection * matViewport;
	// 合成行列の逆行列を計算する
	Matrix4x4 matInverseVPV = Inverse(matVPV);

	// スクリーン座標
	Vector3 posNear = Vector3(float(mousePosition.x), float (mousePosition.y), 0);
	Vector3 posFar = Vector3(float(mousePosition.x), float(mousePosition.y), 1);
	// スクリーン座標系からワールド座標系へ
	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);

	// マウスレイの方向
	Vector3 mouseDirection = posNear - posFar;
	mouseDirection = Normalize(mouseDirection);
	// カメラから照準オブジェクトの距離
	const float kDistanceTestObject = -50.f;
	worldTransform3DReticle_.translation_ = posNear + mouseDirection * kDistanceTestObject;

	// worldTransform3DReticle_のワールド座標行列更新と転送
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

	
	worldTransform_.UpdateMatrix();

	// ゲームパッド操作
	// ゲームパッドの状態を得る変数 (XINPUT)
	XINPUT_STATE joyState;

	// ゲームパッド状態取得
	if (Input::GetInstance()->GetJoystickState(0,joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	}

	// 範囲制限
	const float kMobeLimitX = 35.0f;
	const float kMobeLimitY = 35.0f;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMobeLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMobeLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMobeLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMobeLimitY);

	// 座標移動
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	// スプライトの現在座標を取得
	Vector2 spritePosition = sprite2DReticle_->GetPosition();

	


	////ImGui
	//ImGui::SetNextWindowPos({0,0});
	//ImGui::SetNextWindowSize({300, 200});

	//ImGui::Begin("Player");
	//float silderValue[3] = {
	//    worldTransform_.translation_.x, worldTransform_.translation_.y,
	//    worldTransform_.translation_.z};
	//ImGui::SliderFloat3("position", silderValue, -20.0f, 20.0f);
	//worldTransform_.translation_ = {silderValue[0], silderValue[1], silderValue[2]};

	//ImGui::Text("2DReticle:(%f,%f)", mousePosition.x, mousePosition.y);
	//ImGui::Text("Near:(%+.2f,%+.2f,%+.2f)", posNear.x, posNear.y, posNear.z);
	//ImGui::Text("Far:(%+.2f,%+.2f,%+.2f)", posFar.x, posFar.y, posFar.z);
	//ImGui::Text("3DReticle:(%+.2f,%+.2f,%+.2f)", worldTransform3DReticle_.translation_.x,
	//    worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);
	//ImGui::End();


	Rotate();

	Attack();

	for (PlayerBullet* bullet : bullets_) 
	{
		bullet->Update();
	}
}

void Player::Draw(ViewProjection &viewProjection) 
{
	// プレイヤー
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	for (PlayerBullet* bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}

	// 3Dレティクル
	model_->Draw(worldTransform3DReticle_, viewProjection);
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

	if (input_->IsPressMouse(0)) 
	{
		
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 自機から照準オブジェクトへのベクトル
		velocity = worldTransform3DReticle_.translation_ - worldTransform_.translation_;
		velocity = Normalize(velocity) * kBulletSpeed;

		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);

		bullets_.push_back(newBullet);
	
	}

	// ゲームパッド操作
	XINPUT_STATE joyState;

	// ゲームパッド未接続なら何もせず抜ける
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}

	// Rトリガーを押していたら
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {

		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 自機から照準オブジェクトへのベクトル
		velocity = worldTransform3DReticle_.translation_ - worldTransform_.translation_;
		velocity = Normalize(velocity) * kBulletSpeed;

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
	delete sprite2DReticle_;
	for (PlayerBullet* bullet : bullets_) 
	{
		delete bullet;
	}
}

void Player::OnCollision() 
{
	isDead_ = true; 
}

void Player::SetParent(const WorldTransform* parent) 
{
	worldTransform_.parent_ = parent; 
}

void Player::DrawUI() 
{
	sprite2DReticle_->Draw(); 
}
