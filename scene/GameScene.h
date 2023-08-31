#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "DebugCamera.h"
#include "Enemy.h"
#include "Vector3.h"
#include "Skydome.h"
#include "RailCamera.h"
#include <sstream>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

	// 弾リストを取得
	const std::list<EnemyBullet*>& GetBullet() { return bullets_; }

	/// <summary>
	/// 敵弾を追加
	/// </summary>
	/// <param name="enemyBullet"></param>
	void AddEnemyBullet(EnemyBullet* enemyBullet);


	/// <summary>
	/// 敵を追加
	/// </summary>
	/// <param name="pos"></param>
	void AddEnemy(Vector3 pos);

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();


private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	int scene = 0;

	// ワールド変換データ
	WorldTransform worldTransform_;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

	uint32_t TitleHandle_ = 0u;
	Sprite* spriteTitle_ = nullptr; 

	uint32_t GameOverHandle_ = 0u;
	Sprite* spriteGameOver_ = nullptr; 

	uint32_t ClearHandle_ = 0u;
	Sprite* spriteClear_ = nullptr; 

	uint32_t ManualHandle_ = 0u;
	Sprite* spriteManual_ = nullptr; 

	// 3Dモデル
	Model* model_ = nullptr;


	//ビュープロジェクション
	ViewProjection viewProjection_;

	// 自キャラ
	Player* player_ = nullptr;

	// デバックカメラ
	bool isDebugCameraActive_ = false;

	DebugCamera* debugCamera_ = nullptr;

	// 敵
	std::list<Enemy*> enemy_;

	// 天球
	Skydome* skydome_ = nullptr;

	Model* modelSkydome_ = nullptr;

	// レールカメラ
	RailCamera* railCamera_ = nullptr;

	// 弾のリスト
	std::list<EnemyBullet*> bullets_;

	std::stringstream enemyPopCommands;

	bool waitFlag_ = false;

	int waitTime_;

};
