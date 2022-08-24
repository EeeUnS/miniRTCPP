#pragma once
#include "Vector4D.h"

class Ray
{
public:
	Vector4D GetOrigin() const;
	Vector4D GetNomalizedDirection() const;
	Ray(const Vector4D& origin, const Vector4D& nomalizedDirection);
private:
	Vector4D mOrigin;
	Vector4D mNomalizedDirection;
};