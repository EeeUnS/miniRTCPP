#include "Vector4D.h"
#include <cmath>
#include "ASSERT.h"
#include <smmintrin.h>

Vector4D::Vector4D() : mX(0), mY(0), mZ(0), mW(0)
{}
Vector4D::Vector4D(float x, float y, float z) : mX(x), mY(y), mZ(z), mW(0)
{}
Vector4D::Vector4D(float mX, float mY, float mZ, float mW) : mX(mX), mY(mY), mZ(mZ), mW(mW)
{}

Vector4D	Vector4D::operator+(const Vector4D& b) const
{
	return (Vector4D(mX + b.mX, mY + b.mY, mZ + b.mZ, mW + b.mW));
}

Vector4D	Vector4D::operator-(const Vector4D& b) const
{
	return (Vector4D(mX - b.mX, mY - b.mY, mZ - b.mZ, mW - b.mW));
}

Vector4D	Vector4D::operator*(float scalar) const
{
	return (Vector4D(mX * scalar, mY * scalar, mZ * scalar, mW * scalar));
}

Vector4D	Vector4D::operator/(float scalar) const
{
	return (Vector4D(mX / scalar, mY / scalar, mZ / scalar, mW / scalar));
}

float	Vector4D::Dot(const Vector4D& b) const
{
	//ASSERT(mW == 0 || b.mW == 0);
	return ((mX * b.mX) + (mY * b.mY) + (mZ * b.mZ) + (mW * b.mW));
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