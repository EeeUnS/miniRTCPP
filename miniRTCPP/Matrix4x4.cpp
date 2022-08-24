#include "Matrix4x4.h"

Matrix4x4::Matrix4x4()
	: mCol{Vector4D(), Vector4D(), Vector4D(), Vector4D()}
{}

Matrix4x4::Matrix4x4(const Vector4D a[4])
: mCol{ a[0],a[1],a[2],a[3]}
{}

Matrix4x4::Matrix4x4(
	const Vector4D & a, 
	const Vector4D & b, 
	const Vector4D & c
)
: mCol{ a,b,c, Vector4D()}
{}

Matrix4x4::Matrix4x4(
	const Vector4D& a,
	const Vector4D& b,
	const Vector4D& c,
	const Vector4D& d
)
	:mCol{a,b,c,d}
{}

Vector4D Matrix4x4::operator[](int i) const
{
	return mCol[i];
}

Vector4D& Matrix4x4::operator[](int i)
{
	return mCol[i];
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& a) const
{
	const Matrix4x4 aT = a.Transpose();

	const Matrix4x4 ret(
		aT * mCol[0],
		aT * mCol[1],
		aT * mCol[2],
		aT * mCol[3]
	);
	return ret;
}

Vector4D Matrix4x4::operator*(const Vector4D& a) const
{
	const Vector4D vec(
		mCol[0].Dot(a),
		mCol[1].Dot(a),
		mCol[2].Dot(a),
		mCol[3].Dot(a)
	);
	return vec;
}

Matrix4x4 Matrix4x4::Transpose() const
{
	Matrix4x4 ret;

	ret.mCol[0] = Vector4D(mCol[0].GetX(), mCol[1].GetX(), mCol[2].GetX(), mCol[3].GetX());
	ret.mCol[1] = Vector4D(mCol[0].GetY(), mCol[1].GetY(), mCol[2].GetY(), mCol[3].GetY());
	ret.mCol[2] = Vector4D(mCol[0].GetZ(), mCol[1].GetZ(), mCol[2].GetZ(), mCol[3].GetZ());
	ret.mCol[3] = Vector4D(mCol[0].GetW(), mCol[1].GetW(), mCol[2].GetW(), mCol[3].GetW());

	return ret;
}
