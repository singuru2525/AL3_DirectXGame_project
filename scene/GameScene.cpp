#include "GameScene.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "AxisIndicator.h"
#include <cassert>
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() 
{
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
	for (Enemy* enemy : enemy_) {
		delete enemy;
	}
	delete model_;
	delete player_;
	delete debugCamera_;
	delete skydome_;
	delete railCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("sample.png");
	TextureManager::Load("target.png");

	model_ = Model::Create();

	viewProjection_.Initialize();

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	Vector3 playerPosition(0, 0, 20);

	player_->Initialize(model_,textureHandle_,playerPosition);

	


	// 天球
	skydome_ = new Skydome();

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	skydome_->Initialize(modelSkydome_);


	// レールカメラ
	railCamera_ = new RailCamera();

	railCamera_->Initialize(worldTransform_.translation_,worldTransform_.rotation_);


	// 自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldTransform());

	//デバックカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	//軸方向の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する (アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	AddEnemy({10.f,5.f,100.f});
	LoadEnemyPopData();
}

void GameScene::Update() 
{
	// 自キャラの更新
	player_->Update(viewProjection_);

	// 敵の更新
	for (Enemy* enemy : enemy_) {
		enemy->Update();
	}

	// 天球
	skydome_->Update();

	// レールカメラ
	railCamera_->Update();

	//デバックカメラの更新
	debugCamera_->Update();

	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// 弾の更新呼び出し
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

	UpdateEnemyPopCommands();

	// 0を入力するとデバックカメラが動く
//  #ifdef _DEBUG
	if (input_->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = 1;
	}
//#endif // _DEBUG

	// カメラの処理
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;              //デバックカメラのビュー行列
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;  //デバックカメラのプロジェクション行列
		//ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else if (!isDebugCameraActive_)
	{
		//ビュープロジェクション行列の更新と転送
		viewProjection_.matView = railCamera_->GetViewProjection().matView;              //デバックカメラのビュー行列
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;  //デバックカメラのプロジェクション行列
		viewProjection_.TransferMatrix();
	}
	CheckAllCollisions();
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

	skydome_->Draw(viewProjection_);

	// 敵の描画
	for (Enemy* enemy : enemy_) {
		enemy->Draw(viewProjection_);
	}

	// 弾の描画呼び出し
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画

	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	player_->DrawUI();
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

	const std::list<EnemyBullet*>& enemyBullets = bullets_;

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
	
	for (Enemy* enemy : enemy_) {

		posA = enemy->GetWorldPosition();

		for (PlayerBullet* bullet : playerBullets) {
			 posB = bullet->GetWorldPosition();

			 float ball = (posB.x - posA.x) * (posB.x - posA.x) +
			              (posB.y - posA.y) * (posB.y - posA.y) +
			              (posB.z - posA.z) * (posB.z - posA.z);

			 if (ball <=
			     (bullet->radius_ + enemy->radius_) * (bullet->radius_ + enemy->radius_)) {
				 enemy->OnCollision();

				 bullet->OnCollision();
			 }
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

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) 
{

	bullets_.push_back(enemyBullet);

}

void GameScene::AddEnemy(Vector3 pos) 
{
	// 敵の生成
	Enemy*obj = new Enemy();
	// 敵の初期化
	obj->Initialize(model_, pos, {0, 0, -0.5});

	// 敵キャラに自キャラのアドレスをわたす
	obj->SetPlayer(player_);

	obj->SetGameScene(this);

	enemy_.push_back(obj);
}


void GameScene::LoadEnemyPopData() 
{
	// ファイルを開く
	std::ifstream file;
	file.open("Resources/enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();

}

void GameScene::UpdateEnemyPopCommands() 
{
	// 待機処理
	if (waitFlag_) 
	{
		waitTime_--;
		if (waitTime_ <= 0) 
		{
			// 待機完了
			 waitFlag_ = false;
		}
		return;
	}


	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands,line)) 
	{
		// 1行分の文字列をストリームに変換して解析しやすくなる
		std::istringstream line_stream(line);

		std::string word;

		// ,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) 
		{
			// コメント行を飛ばす
			 continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			 // x座標
			 getline(line_stream, word, ',');
			 float x = (float)std::atof(word.c_str());

			 // x座標
			 getline(line_stream, word, ',');
			 float y = (float)std::atof(word.c_str());

			 // x座標
			 getline(line_stream, word, ',');
			 float z = (float)std::atof(word.c_str());

			 // 敵を発生させる
			 AddEnemy(Vector3(x, y, z));
		} 
		else if (word.find("WAIT") == 0)
		{
			 getline(line_stream, word, ',');

			 // 待ち時間
			 int32_t waitTime = atoi(word.c_str());

			 // 待機開始
			 waitFlag_ = true;
			 waitTime_ = waitTime;

			 // コマンドループを抜ける
			 break;
		}

	}


}