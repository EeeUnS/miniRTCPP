#pragma once
#include "Vector4D.h"

class Matrix4x4
{
public:
	Matrix4x4();
	Matrix4x4(const Vector4D a[4]);
	Matrix4x4(
		const Vector4D& a,
		const Vector4D& b,
		const Vector4D& c
	);
	Matrix4x4(
		const Vector4D& a,
		const Vector4D& b,
		const Vector4D& c,
		const Vector4D& d
	);
	Vector4D operator[](int i) const;
	Vector4D& operator[](int i);
	Matrix4x4 operator*(const Matrix4x4& a) const;
	Vector4D operator*(const Vector4D& a) const;
	//M^T
	Matrix4x4 Transpose() const;
private:
	Vector4D mCol[4];
};
