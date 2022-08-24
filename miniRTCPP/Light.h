#pragma once

#include "Vector4D.h"
#include "Color.h"

class Light
{

public:
	Light(const Vector4D& origin, const Color& color, const float bright);
	Vector4D GetOrigin() const;
	Color GetColor() const;
	void SetOrigin(const Vector4D &a);
	float GetBright() const;
private:
	Vector4D mOrigin;
	Color mColor;
	float	mBright;
};
