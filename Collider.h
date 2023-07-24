#pragma once
#include "Vector3.h"
/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider 
{
private:

	float radius_ = 1.0f;

public:

	// 半径を取得
	float GetRadius() { return radius_; }

	// 半径を設定
	void SetRadius(float radius) { radius_ = radius; }

	// 衝突時に呼ばれる関数
	virtual void OnCollision() {};

	// Getter
	virtual Vector3 GetWorldPosition() = 0;

};
