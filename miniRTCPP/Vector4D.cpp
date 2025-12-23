#include "Vector4D.h"
#include <cmath>
#include "ASSERT.h"


#ifdef USE_SIMD
#include <smmintrin.h>
#endif

Vector4D::Vector4D() : mX(0), mY(0), mZ(0), mW(0)
{}
Vector4D::Vector4D(float x, float y, float z) : mX(x), mY(y), mZ(z), mW(0)
{}
Vector4D::Vector4D(float mX, float mY, float mZ, float mW) : mX(mX), mY(mY), mZ(mZ), mW(mW)
{}

Vector4D::Vector4D(__m128 simd) : mSimd(simd)
{
}

#ifdef USE_SIMD
//안한게 더 빠름 
// const __m128			cc = *((__m128*)&a);
// const __m128			kk = _mm_mul_ps(cc, cc);
// const __m128			kkk = _mm_hadd_ps(kk, kk);
// return (_mm_cvtss_f32(_mm_sqrt_ps(_mm_hadd_ps(kkk, kkk))));
//t_vec3	make_v3(double const x, double const y, double const z)
//{
//	 const __m128			cc = *((__m128*)&a);
//	 const __m128			kk = _mm_mul_ps(cc, cc);
//	 const __m128			kkk = _mm_hadd_ps(kk, kk);
//	 return (_mm_cvtss_f32(_mm_sqrt_ps(_mm_hadd_ps(kkk, kkk))));
//}

// const double	len = v3_l2norm(a);
// const t_vec3	n = {a.x / len, a.y / len, a.z / len, a.w / len};
// return (n);
//float	v3_l2norm(t_vec3 a)
//{
//	return (sqrtf(a.x * a.x + a.y * a.y + a.z * a.z));
//}

// const __m128			cc = *((__m128*)&a);
// const __m128			s = _mm_mul_ps(cc, cc);
// const __m128			l = _mm_hadd_ps(s, s);
// const __m128			no = _mm_mul_ps(cc, _mm_rsqrt_ps((_mm_hadd_ps(l, l))));
// const t_vec3			*p = (t_vec3 *)&no;
// return (*p);
//t_vec3	v3_normalize(t_vec3 a)
//{
//	 const __m128			cc = *((__m128*)&a);
//	 const __m128			s = _mm_mul_ps(cc, cc);
//	 const __m128			l = _mm_hadd_ps(s, s);
//	 const __m128			no = _mm_mul_ps(cc, _mm_rsqrt_ps((_mm_hadd_ps(l, l))));
//	 const t_vec3			*p = (t_vec3 *)&no;
//	 return (*p);
//}
//t_vec3	v3_mul(t_vec3 a, float s)
//{
//	const t_vec3	n = { a.x * s, a.y * s, a.z * s, 0 };
//
//	return (n);
//}
//
//float	v3_dot(t_vec3	a, t_vec3 b)
//{
//	const __m128			cc = *((__m128*) & a);
//	const __m128			dd = *((__m128*) & b);
//	const __m128			kk = _mm_mul_ps(cc, dd);
//	const __m128			kkk = _mm_hadd_ps(kk, kk);
//
//	return (_mm_cvtss_f32(_mm_hadd_ps(kkk, kkk)));
//}

Vector4D	Vector4D::operator+(const Vector4D& b) const
{
	return (_mm_add_ps(this->mSimd, b.mSimd));
}


Vector4D	Vector4D::operator-(const Vector4D& b) const
{
	return _mm_sub_ps(this->mSimd, b.mSimd);
}

// 기존로직 유지
float	Vector4D::Dot(const Vector4D& b) const
{
	//const __m128			kk = _mm_mul_ps(mSimd, b.mSimd);
	//const __m128			kkk = _mm_hadd_ps(kk, kk);
	//
	//return (_mm_cvtss_f32(_mm_hadd_ps(kkk, kkk)));

	//ASSERT(mW == 0 || b.mW == 0);
	return ((mX * b.mX) + (mY * b.mY) + (mZ * b.mZ) + (mW * b.mW));
}



#else

Vector4D	Vector4D::operator+(const Vector4D& b) const
{
	return (Vector4D(mX + b.mX, mY + b.mY, mZ + b.mZ, mW + b.mW));
}

Vector4D	Vector4D::operator-(const Vector4D& b) const
{
	return (Vector4D(mX - b.mX, mY - b.mY, mZ - b.mZ, mW - b.mW));
}

float	Vector4D::Dot(const Vector4D& b) const
{
	//ASSERT(mW == 0 || b.mW == 0);
	return ((mX * b.mX) + (mY * b.mY) + (mZ * b.mZ) + (mW * b.mW));
}

#endif


Vector4D	Vector4D::operator*(float scalar) const
{
	return (Vector4D(mX * scalar, mY * scalar, mZ * scalar, mW * scalar));
}

Vector4D	Vector4D::operator/(float scalar) const
{
	return (Vector4D(mX / scalar, mY / scalar, mZ / scalar, mW / scalar));
}


float	Vector4D::Length()const
{
	//ASSERT(mW == 0);
	return sqrtf((mX * mX) + (mY * mY) + (mZ * mZ) + (mW * mW));
}

Vector4D	Vector4D::Normalize() const
{
	//ASSERT(mW == 0);
	return (*this / Length());
}

Vector4D	Vector4D::Cross(const Vector4D& b) const
{
	const float x = mY * b.mZ - mZ * b.mY;
	const float y = mZ * b.mX - mX * b.mZ;
	const float z = mX * b.mY - mY * b.mX;
	return Vector4D(x, y, z);
}

float Vector4D::GetX()  const
{
	return mX;
}

 
float Vector4D::GetY()  const
{
	return mY;
}


float Vector4D::GetZ()  const
{
	return mZ;
}

float Vector4D::GetW()  const
{
	return mW;
}

void Vector4D::SetX(float x)
{
	mX = x;
}

void Vector4D::SetY(float y)
{
	mY = y;
}

void  Vector4D::SetZ(float z)
{
	mZ= z;
}

void  Vector4D::SetW(float w)
{
	mW = w;
}

Vector4D Vector4D::GetNormalizedReflection(const Vector4D& normal) const
{
	const Vector4D normalizedNormal = normal.Normalize();
	const Vector4D a = normalizedNormal * Dot(normalizedNormal) * -2;
	return (a + *this); 
}