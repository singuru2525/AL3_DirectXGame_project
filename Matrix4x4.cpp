#include "Matrix4x4.h"
#include <cmath>

Matrix4x4 Scale(const Vector3& scale) {
	Matrix4x4 result;
	result = {scale.x, 0.0f,    0.0f,    0.0f, 
		      0.0f,    scale.y, 0.0f,    0.0f,
	          0.0f,    0.0f,    scale.z, 0.0f, 
		      0.0f,    0.0f,    0.0f,    1.0f};
	return result;
}


// X軸回転行列
Matrix4x4 MakeRotationXMatrix(float radian) {
	float sin = std::sin(radian);
	float cos = std::cos(radian);

	Matrix4x4 result{1.0f, 0.0f, 0.0f, 0.0f,
		             0.0f, cos,  sin,  0.0f,
	                 0.0f, -sin, cos,  0.0f,
		             0.0f, 0.0f, 0.0f, 1.0f};
	return result;
}

// Y軸回転行列
Matrix4x4 MakeRotationYMatrix(float radian) {
	float sin = std::sin(radian);
	float cos = std::cos(radian);

	Matrix4x4 result{cos, 0.0f,  -sin, 0.0f,
		             0.0f, 1.0f, 0.0f, 0.0f,
	                 sin,  0.0f, cos,  0.0f, 
		             0.0f, 0.0f, 0.0f, 1.0f};
	return result;
}

// Z軸回転行列
Matrix4x4 MakeRotationZMatrix(float radian) {
	float sin = std::sin(radian);
	float cos = std::cos(radian);

	Matrix4x4 result{cos,  sin,  0.0f, 0.0f, 
		             -sin, cos,  0.0f, 0.0f,
	                 0.0f, 0.0f, 1.0f, 0.0f,
		             0.0f, 0.0f, 0.0f, 1.0f};
	return result;
}

Matrix4x4 MakeTransMatrix(const Vector3& trans) {
	Matrix4x4 result{1.0f,    0.0f,    0.0f,    0.0f, 
		             0.0f,    1.0f,    0.0f,    0.0f,
	                 0.0f,    0.0f,    1.0f,    0.0f,
		             trans.x, trans.y, trans.z, 1.0f};
	return result;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate) {
	
	// スケーリング
	Matrix4x4 matScale = Scale(scale);

	Matrix4x4 matRotX = MakeRotationXMatrix(rot.x);
	Matrix4x4 matRotY = MakeRotationYMatrix(rot.y);
	Matrix4x4 matRotZ = MakeRotationZMatrix(rot.z);

	Matrix4x4 matRot = matRotZ * matRotX * matRotY;

	Matrix4x4 matTranslate = MakeTransMatrix(translate);

	Matrix4x4 result = matScale * matRot * matTranslate;

	return result;
}


// 正規化
Vector3 Normalize(const Vector3& v) 
{
	Vector3 resultNormalize;
	float length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	resultNormalize.x = v.x / length;
	resultNormalize.y = v.y / length;
	resultNormalize.z = v.z / length;

	return resultNormalize;
}


// 長さ
float Length(const Vector3& v) 
{
	float resultLength = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	return resultLength;
}


// 代入演算子
Matrix4x4& operator*=(Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = {};

	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			for (size_t k = 0; k < 4; k++) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	m1 = result;
	return m1;
}

// 二項演算子
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) 
{ 
	Matrix4x4 result = m1;

	return result *= m2;
}


Vector3& operator*=(Vector3& v, float s)
{
	v.x *= s;
	v.y *= s;
	v.z *= s;

	return v;
}

const Vector3& operator*(const Vector3& v, float s)
{
	Vector3 tmp(v);
	return tmp *= s;
}