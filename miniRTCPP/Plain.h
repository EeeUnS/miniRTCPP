#pragma once
#include "Object.h"
class Plain :
    public Object
{
public:
    Plain(
		const Vector4D& origin,
		const Vector4D& direction,
		const Color& color,
		const float specularRS,
		const int specularNS
	);
	virtual Vector4D CalcNormalVector(const Vector4D& intersection, const Vector4D& rayOrigin) const override;
	virtual float CalcIntersectDistanceOrNan(const Ray& ray) const override;

private:
	static constexpr float heightRange[2] = { 0,0 };

};

