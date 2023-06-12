#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "MathUtility.h"
#include "ImGuiManager.h"

/// <summary>
/// レールカメラ
/// </summary>
class RailCamera {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Vector3& trans,Vector3& rot);


	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ビュープロジェクションを取得
	/// </summary>
	/// <returns>ビュープロジェクション</returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }
	
	// ワールドトランスフォームを取得
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

private:

	// ワールド変換データ
	WorldTransform worldTransform_;

	// ビュープロジェクション
	ViewProjection viewProjection_;
};
