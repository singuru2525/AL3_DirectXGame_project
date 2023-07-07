#pragma once
#include "Vector3.h"
#define _USE_MATH_DEFINES
#include <math.h>

/// <summary>
/// 4x4行列
/// </summary>
struct Matrix4x4 final {
	float m[4][4];

};

Matrix4x4 Scale(const Vector3& scale);

//Matrix4x4 MakeRotationMatrix(float theta);

Matrix4x4 MakeRotationXMatrix(float radian);

Matrix4x4 MakeRotationYMatrix(float radian);

Matrix4x4 MakeRotationZMatrix(float radian);

Matrix4x4 MakeTransMatrix(const Vector3& trans);

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);

// 正規化
Vector3 Normalize(const Vector3& v);

float Length(Vector3 v);

Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

// 代入演算子
Matrix4x4& operator*=(Matrix4x4& m1, const Matrix4x4& m2);

// 二項演算子
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);


Vector3& operator*=(Vector3& v, float s);


const Vector3 operator*(const Vector3& v, float s);

Vector3& operator-=(Vector3& v1, const Vector3& v2);

const Vector3 operator-(const Vector3& v1, const Vector3& v2);

Vector3& operator+=(Vector3& v1, const Vector3& v2);

const Vector3 operator+(const Vector3& v1, const Vector3& v2);