#include "Color.h"
#include <algorithm>
#include "ASSERT.h"

Color::Color() : mRed(0), mGreen(0), mBlue(0), mAlpha(0)
{}

Color::Color(unsigned char r, unsigned char g, unsigned char b) : mAlpha(0), mRed(r), mGreen(g), mBlue(b)
{}

unsigned int Color::ToHex() const
{
	return (mColor);
}

Color Color::operator+(const Color& a) const
{
	ASSERT((int)mRed + a.mRed <= 255 || (int)mGreen + a.mGreen <= 255 || (int)mBlue + a.mBlue <= 255);
	return Color(mRed + a.mRed, mGreen + a.mGreen, mBlue + a.mBlue);
}

Color Color::operator*(float a)
{
	//ASSERT(a > 1);
	if (a > 1 || a < 0)
		return *this;
	ASSERT((int)mRed * a <= 255 || (int)mGreen * a <= 255 || (int)mBlue * a <= 255);

	return Color((unsigned char)(a * mRed), (unsigned char)(a * mGreen), (unsigned char)(a * mBlue));
}

unsigned char Color::GetRed() const
{
	return mRed;
}

unsigned char Color::GetBlue()const
{
	return mBlue;
}

unsigned char Color::GetGreen()const
{
	return mGreen;
}

void Color::IntensityAttenuation(const Vector4D& pos1, const Vector4D& pos2)
{
	constexpr int	unit = 128;
	constexpr float a[3] = { 1 , 0, 0 };
	const float dist = Vector4D(pos1 - pos2).Length() / unit;
	const float attenuation = std::min(1.0f, 1 / (a[0] + a[1] * dist + a[2] * dist * dist));
	*this = *this * attenuation;
}