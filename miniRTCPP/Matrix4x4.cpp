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

#ifdef USE_SIMD 
Matrix4x4 Matrix4x4::operator*(const Matrix4x4& a) const
{
	const Matrix4x4 aT = a.Transpose();

	const Matrix4x4 ret(
		aT * mCol[0],
		aT * mCol[1],
		aT * mCol[2],
		aT * mCol[3]
	);
	return ret;/*
	__m128 row1 = _mm_load_ps(&B[0]);
	__m128 row2 = _mm_load_ps(&B[4]);
	__m128 row3 = _mm_load_ps(&B[8]);
	__m128 row4 = _mm_load_ps(&B[12]);
	for (int i = 0; i < 4; i++)
	{
		__m128 brod1 = _mm_set1_ps(A[4 * i + 0]);
		__m128 brod2 = _mm_set1_ps(A[4 * i + 1]);
		__m128 brod3 = _mm_set1_ps(A[4 * i + 2]);
		__m128 brod4 = _mm_set1_ps(A[4 * i + 3]);
		__m128 row = _mm_add_ps(
			_mm_add_ps(
				_mm_mul_ps(brod1, row1),
				_mm_mul_ps(brod2, row2)),
			_mm_add_ps(
				_mm_mul_ps(brod3, row3),
				_mm_mul_ps(brod4, row4)));
		_mm_store_ps(&C[4 * i], row);
	}*/
}
#else
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
#endif

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
