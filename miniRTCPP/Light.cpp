#include "Light.h"

Light::Light(const Vector4D& origin, const Color& color, const float bright)
	:
	mOrigin(origin),
	mColor(color),
	mBright(bright)
{}

Vector4D Light::GetOrigin() const
{
	return mOrigin;
}

Color Light::GetColor() const
{
	return mColor;
}

void Light::SetOrigin(const Vector4D& a)
{
	mOrigin = a;
}

float Light::GetBright() const
{
	return mBright;
}

