#pragma once

#include "Vector4D.h"

class Color
{
public:
	Color();
	Color(unsigned char r, unsigned char g, unsigned char b);
	unsigned int ToHex() const;
	Color operator+(const Color& a) const;
	Color operator*(float a);
	unsigned char GetRed() const;
	unsigned char GetBlue() const;
	unsigned char GetGreen() const;
	//t_color color_disruption(const t_quadrics* Q, t_vec3 point_from_cam, t_color obj_color);
	//t_color get_texture_color(const t_quadrics* Q, const t_xpm* texture, t_vec point);
	//TODO naming change
	void IntensityAttenuation(const Vector4D &pos1, const Vector4D &pos2);

private:
	//int : ARGB
	union
	{
		unsigned int mColor;
		struct{
			unsigned char mAlpha;
			unsigned char mRed;
			unsigned char mGreen;
			unsigned char mBlue;
		};
	};
};

