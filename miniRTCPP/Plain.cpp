#include "Plain.h"
#include <array>
#include "ASSERT.h"
#include <float.h>

Plain::Plain(
	const Vector4D& origin, 
	const Vector4D& direction, 
	const Color& color, 
	const float specularRS, 
	const int specularNS
)
:Object(
	origin,
	direction,
	color,
	specularRS,
	specularNS,
	heightRange
)
{
	const Matrix4x4 coefs =
	Matrix4x4
	(
		Vector4D(),
		Vector4D(),
		Vector4D(0,0,0,1),
		Vector4D(0,0,1,0)
	);

	mCoefs = coefs;
}

Vector4D Plain::CalcNormalVector(const Vector4D& intersection, const Vector4D& rayOrigin) const
{
	//TODO
	return GetNormalizedDirection();
}

float Plain::CalcIntersectDistanceOrNan(const Ray& ray) const
{
	const float cos = ray.GetNomalizedDirection().Dot(GetNormalizedDirection());
	if (std::abs(cos) < FLT_EPSILON)
		return NAN;
	
	ASSERT(ray.GetNomalizedDirection().GetW() == 0);
	const Vector4D vec = GetOrigin() - ray.GetOrigin();
	const float dist = vec.Dot(GetNormalizedDirection()) / cos;
	if (dist <= 0)
		return NAN;
	return (dist);
}
