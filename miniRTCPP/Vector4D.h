#pragma once
class Vector4D
{
public:
	Vector4D();
	Vector4D(float x, float y, float z);
	Vector4D(float x, float y, float z, float w);

	Vector4D	operator+(const Vector4D& b) const;
	Vector4D	operator-(const Vector4D& b) const;
	Vector4D	operator*(float scalar) const;
	Vector4D	operator/(float scalar) const;
	
	float	Length()const;
	Vector4D	Normalize() const;

	float	Dot(const Vector4D& b) const;
	Vector4D	Cross(const Vector4D& b) const;

	float GetX() const;
	float GetY() const;
	float GetZ() const;
	float GetW() const;
	void SetX(float x);
	void SetY(float y);
	void SetZ(float z);
	void SetW(float w);
	Vector4D GetNormalizedReflection(const Vector4D& normal) const;
private:
	float	mX;
	float	mY;
	float	mZ;
	float	mW;
};

