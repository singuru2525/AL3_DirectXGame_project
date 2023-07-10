#include "Matrix4x4.h"
#include <cmath>
#include <assert.h>

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


// ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) 
{
	Matrix4x4 result;
	result = 
	{
	    width / 2,        0.0f,             0.0f,                0.0f,
	    0.0f,             -height / 2,      0.0f,                0.0f,
	    0.0f,             0.0f,             maxDepth - minDepth, 0.0f,
	    width / 2 + left, height / 2 + top, minDepth,            1.0f
	};

	return result;
}


// 座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix)
{
	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] +
	           1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] +
	           1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] +
	           1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] +
	          1.0f * matrix.m[3][3];
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}


Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) 
{ 
	Vector3 temp;

	temp = 
	{
	    temp.x = (1.0f - t) * v1.x + t * v2.x,
		temp.y = (1.0f - t) * v1.y + t * v2.y,
	    temp.z = (1.0f - t) * v1.z + t * v2.z
	};

	return temp;
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

const Vector3 operator*(const Vector3& v, float s)
{
	Vector3 tmp(v);
	return tmp *= s;
}

// オーバーロード
Vector3& operator-=(Vector3& v1, const Vector3& v2)
{
	v1.x -= v2.x;
	v1.y -= v2.y;
	v1.z -= v2.z;

	return v1;
}

const Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
	Vector3 tmp(v1);
	return tmp -= v2;
}

Vector3& operator+=(Vector3& v1, const Vector3& v2) 
{
	v1.x += v2.x;
	v1.y += v2.y;
	v1.z += v2.z;

	return v1;

}

const Vector3 operator+(const Vector3& v1, const Vector3& v2) 
{
	Vector3 tmp(v1);
	return tmp += v2;
}

