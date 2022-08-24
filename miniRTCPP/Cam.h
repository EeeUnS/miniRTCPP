#pragma once
#include "Vector4D.h"
#include <math.h>
#include "Resolution.h"

struct Cam
{
	Vector4D Origin;
	Vector4D NomalizedDirection;
	float	HfovRadian;
	double	DistanceCamToScreen;

	Cam(): HfovRadian(0), DistanceCamToScreen(0)
	{
		NomalizedDirection.SetW(1);
	}
	Cam(
		const Vector4D &origin,
		const Vector4D &direction, 
		const float hfov
	)
		: Origin(origin), 
		NomalizedDirection(direction.Normalize()), 
		HfovRadian(hfov)
	{
		const double distanceCamToScreen = (WIN_WIDTH / 2) / tan(hfov / 2);
		DistanceCamToScreen = distanceCamToScreen;
	}
};
