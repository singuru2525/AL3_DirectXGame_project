#pragma once
#include "Vector3.h"
#include <cstdint>

/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider 
{
private:

	float radius_ = 1.0f;

	// 衝突属性(自分)
	uint32_t collisionAttribute_ = 0xffffffff;

	// 衝突マスク(相手)
	uint32_t CollisionMask_ = 0xffffffff;

public:

	// 半径を取得
	float GetRadius() { return radius_; }

	// 半径を設定
	void SetRadius(float radius) { radius_ = radius; }

	// 衝突時に呼ばれる関数
	virtual void OnCollision() {};

	// Getter
	virtual Vector3 GetWorldPosition() = 0;


	// 衝突属性(自分) を取得
	uint32_t GetCollisionAttribute() { return collisionAttribute_; }

	// 衝突属性(自分) を設定
	void SetCollisionAttribute(uint32_t collisionAttribute) { collisionAttribute_ = collisionAttribute; }

	
	// 衝突マスク(相手) を取得
	uint32_t GetCollisionMask() { return CollisionMask_; }

	// 衝突マスク(相手) を設定
	void SetCollisionMask(uint32_t CollisionMask) { CollisionMask_ = CollisionMask; }

};
