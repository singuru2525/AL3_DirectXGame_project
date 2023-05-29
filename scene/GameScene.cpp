#include "GameScene.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "AxisIndicator.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() 
{
	delete model_;
	delete player_;
	delete debugCamera_;
	delete enemy_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("sample.png");

	model_ = Model::Create();

	viewProjection_.Initialize();

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(model_,textureHandle_);

	// 敵の生成
	enemy_ = new Enemy();
	// 敵の初期化
	enemy_->Initialize(model_, {0, 3, 50}, {0,0,-0.5});

	// 敵キャラに自キャラのアドレスをわたす
	enemy_->SetPlayer(player_);

	//デバックカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	//軸方向の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する (アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() 
{
	// 自キャラの更新
	player_->Update();

	CheckAllCollisions();

	if (enemy_) 
	{
		// 敵の更新
		enemy_->Update();
	}

	//デバックカメラの更新
	debugCamera_->Update();

	// 0を入力するとデバックカメラが動く
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = 1;
	}
#endif // _DEBUG

	// カメラの処理
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;              //デバックカメラのビュー行列
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;  //デバックカメラのプロジェクション行列
		//ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		//ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	/// 
	// 自キャラの描画
	player_->Draw(viewProjection_);

	if (enemy_) 
	{
		// 敵の描画
		enemy_->Draw(viewProjection_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() 
{ 
	Vector3 posA, posB;

	const std::list<PlayerBullet*>& playerBullets = player_->GetBullet();

	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullet();

	#pragma region 自キャラと敵弾の当たり判定

	posA = player_->GetWorldPosition();

	for (EnemyBullet* bullet : enemyBullets) 
	{
		posB = bullet->GetWorldPosition();

		float ball = (posB.x - posA.x) * (posB.x - posA.x) +
		               (posB.y - posA.y) * (posB.y - posA.y) +
		               (posB.z - posA.z) * (posB.z - posA.z);

		if (ball <= (bullet->radius_ + player_->radius_) * (bullet->radius_ + player_->radius_))
		{
			 player_->OnCollision();

			 bullet->OnCollision();
		}

	}

	#pragma endregion

	#pragma region 自弾と敵キャラの当たり判定

	posA = enemy_->GetWorldPosition();

	for (PlayerBullet* bullet : playerBullets) {
		posB = bullet->GetWorldPosition();


		float ball = (posB.x - posA.x) * (posB.x - posA.x) +
		             (posB.y - posA.y) * (posB.y - posA.y) +
		             (posB.z - posA.z) * (posB.z - posA.z);

		if (ball <= (bullet->radius_ + enemy_->radius_) * (bullet->radius_ + enemy_->radius_))
		{
			 enemy_->OnCollision();

			 bullet->OnCollision();
		}
	}

	#pragma endregion

	#pragma region 自弾と敵弾の当たり判定

	for (PlayerBullet* bullets : playerBullets) 
	{
		posA = bullets->GetWorldPosition();

		for (EnemyBullet* bullet : enemyBullets) 
		{
			 posB = bullet->GetWorldPosition();

			 float ball = (posB.x - posA.x) * (posB.x - posA.x) +
			              (posB.y - posA.y) * (posB.y - posA.y) +
			              (posB.z - posA.z) * (posB.z - posA.z);

			 if (ball <= (bullet->radius_ + bullets->radius_) * (bullet->radius_ + bullets->radius_)) {
				 bullets->OnCollision();

				 bullet->OnCollision();
			 }
		}
	}

	#pragma endregion
}