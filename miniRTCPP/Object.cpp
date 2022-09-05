#include "Object.h"
#include <algorithm>
#include "ASSERT.h"
#include "SceneManager.h"
#include "RayCastingSimulator.h"

Vector4D Object::getTan() const
{
	if (mNormalizedDirection.GetY() == 1)
	{
		return Vector4D(1, 0, 0, 0);
	}
	else if (mNormalizedDirection.GetY() == -1)
	{
		return Vector4D(-1, 0, 0, 0);
	}
	Vector4D a(0, 1, 0);
	return a.Cross(mNormalizedDirection).Normalize();
}

Object::Object(
	const float coefs[5],
	const Vector4D& origin,
	const Vector4D& direction,
	const Color& color,
	const float specularRS,
	const int specularNS,
	const float heightRange[2])
	:
	mOrigin(origin),
	mColor(color),
	mNormalizedDirection(direction.Normalize()),
	mSpecularNS(specularNS),
	mSpecularRS(specularRS),
	mHeightRange{ heightRange[0], heightRange[1] },
	mDisruption(false)
{
	mTan = getTan();

	float inCoefs[5] = {
		coefs[0],
		coefs[1],
		coefs[2],
		coefs[3],
		coefs[4]
	};
	
	if(inCoefs[0] < 0)
	{
		inCoefs[0] *= -1;
		inCoefs[1] *= -1;
		inCoefs[2] *= -1;
		inCoefs[3] *= -1;
		inCoefs[4] *= -1;
	}

	const Matrix4x4 col(
		Vector4D(inCoefs[0], 0, 0, 0),
		Vector4D(0, inCoefs[1], 0, 0),
		Vector4D(0, 0, inCoefs[2], inCoefs[3]),
		Vector4D(0, 0, inCoefs[3], inCoefs[4])
	);

	mCoefs = col;

	const Matrix4x4 mat(
		mTan,
		mNormalizedDirection.Cross(mTan),
		mNormalizedDirection,
		Vector4D(0, 0, 0, 1)
	);
	mCoefs = mat.Transpose() * mCoefs * mat;
}

Object::Object(
	const Vector4D& origin, 
	const Vector4D& direction, 
	const Color& color, 
	const float specularRS, 
	const int specularNS,
	const float heightRange[2]
)
:
	mOrigin(origin),
	mNormalizedDirection(direction.Normalize()),
	mColor(color),
	mSpecularNS(specularNS),
	mSpecularRS(specularRS),
	mDisruption(false),
	mHeightRange{heightRange[0], heightRange[1]}
{
	mTan = getTan();
}

Color Object::GetColor() const
{
	return mColor;
}

Vector4D Object::GetOrigin() const
{
	return mOrigin;
}

void Object::SetOrigin(const Vector4D& a)
{
	mOrigin = a;
}

Vector4D Object::GetNormalizedDirection() const
{
	return mNormalizedDirection;
}

void Object::SetNormalizedDirection(const Vector4D& a)
{
	mNormalizedDirection = a;
}

Vector4D Object::GetTan()
{
	return mTan;
}

void Object::SetTan(const Vector4D& tan)
{
	mTan = tan;
}

Matrix4x4 Object::GetCoefs()
{
	return mCoefs;
}

void Object::SetCoefs(const Matrix4x4& coefs)
{
	mCoefs = coefs;
}

//(a)t^2 + (2b)t + (c) = 0
// return smaller first
std::pair<float, float> solveQuadraticHalfEq(float a, float b, float c)
{
	std::pair<float, float>	sol{ NAN, NAN };

	if (a == 0)
	{
		if (b != 0)
		{
			sol.first = -c / (2 * b);
		}
		return (sol);
	}

	const float	discriminant= b * b - a * c;
	//ASSERT(discriminant < 0, "want discriminat positive");
	if (discriminant < FLT_EPSILON)
	{
		return (sol);
	}

	sol.first = (-b - sqrtf(discriminant)) / a; 
	sol.second = (-b + sqrtf(discriminant)) / a;
	return (sol);
}

bool Object::isHeightRange(const Ray& ray, float distance) const
{
	const Vector4D intersection = (ray.GetNomalizedDirection() * distance) + ray.GetOrigin();
	const Vector4D objectOriginToRayOrigin = intersection - mOrigin;
	const float	height = objectOriginToRayOrigin.Dot(mNormalizedDirection);
	return (mHeightRange[0] - FLT_EPSILON <= height && height <= mHeightRange[1] + FLT_EPSILON);
}

int Object::num = 0;

//return smaller distance or nan
float Object::CalcIntersectDistanceOrNan(const Ray& ray) const
{
	num++;
	Vector4D objectOriginToRayOrigin = ray.GetOrigin() - mOrigin;
	objectOriginToRayOrigin.SetW(1);

	ASSERT(ray.GetNomalizedDirection().GetW() < FLT_EPSILON);

	const Vector4D tmp = mCoefs * ray.GetNomalizedDirection();

	const float a = tmp.Dot(ray.GetNomalizedDirection());
	const float b = tmp.Dot(objectOriginToRayOrigin);
	const float c = (mCoefs * objectOriginToRayOrigin).Dot(objectOriginToRayOrigin);
		
	const std::pair<float, float> distanceSols = solveQuadraticHalfEq(a, b, c);

	//ASSERT(isnan(distanceSols.first), "want not nan ");

	if (!isnan(distanceSols.first) 
		&& distanceSols.first > FLT_EPSILON 
		&& isHeightRange(ray, distanceSols.first))
	{
		return (distanceSols.first);
	}
	if (!isnan(distanceSols.second) 
		&& distanceSols.second>  FLT_EPSILON 
		&& isHeightRange(ray, distanceSols.second))
	{
		return (distanceSols.second);
	}
	return (NAN);
}

void Object::CalcSpecularAndDiffuseScale(
	const Light& light, 
	const Ray& ray, 
	const Vector4D& intersectionNormal,
	const Vector4D& intersectionPoint,
	float * const outDiffuseScale,
	float * const outSpecularScale
) const
{
	const Vector4D normalizedReflectedRay = ray.GetNomalizedDirection().GetNormalizedReflection(intersectionPoint);

	const Vector4D pToLight = light.GetOrigin() - intersectionPoint;
	const float pToLightLen = pToLight.Length();
	const Ray intersectionToLight(intersectionPoint, pToLight / pToLightLen );
	
	*outDiffuseScale = intersectionToLight.GetNomalizedDirection().Dot(intersectionNormal);
	*outSpecularScale = intersectionToLight.GetNomalizedDirection().Dot(normalizedReflectedRay);

	if (*outDiffuseScale <= 0 && *outSpecularScale <= 0)
	{
		*outDiffuseScale = 0;
		*outSpecularScale = 0;
		return ;
	}

	float distance = NAN;
	const Object* object = nullptr;
	RayCastingSimulator::GetInstance()->CalcMinDistanceObject(intersectionToLight, &distance, &object);
	if (!isnan(distance) && distance < pToLightLen + FLT_EPSILON)
	{
		//object collaps
		*outDiffuseScale = 0;
		*outSpecularScale = 0;
		return ;
	}

	*outDiffuseScale *= light.GetBright();
	*outDiffuseScale *= (1 - mSpecularRS);

	*outSpecularScale = mSpecularRS * powf(*outSpecularScale, static_cast<float>(mSpecularNS));
	*outSpecularScale *= light.GetBright();
}

Color Object::CalcSpecularAndDiffuseLight(const Ray& ray, const Vector4D& intersectionPoint) const
{
	
	//TODO texture, disruption
	Color obColor = mColor;

	Color color;

	const auto& lights = SceneManager::GetInstance()->GetLights();
	for (const Light& light : lights)
	{
		const Vector4D intersectionNormal = CalcNormalVector(intersectionPoint, ray.GetOrigin());
		const Vector4D fixIntersectionPoint = intersectionPoint + intersectionNormal * 0.01f;

		float diffuseScale;
		float specularScale;
		CalcSpecularAndDiffuseScale(light, ray, intersectionNormal, fixIntersectionPoint, &diffuseScale, &specularScale);

		if (diffuseScale > FLT_EPSILON)
		{
			const Color lightColor = light.GetColor();
			const Color colorr = lightColor * obColor;

			const Color diffuseSingleLightColor = colorr * diffuseScale;
			color = color + diffuseSingleLightColor;
		}
		if (specularScale > FLT_EPSILON)
		{
			const Color lightSpecular = light.GetColor() * specularScale;
			color = color + lightSpecular;
		}
	}
	return (color);
}

Color Object::CalcPhongModelColor(const Ray& ray, const float distance) const
{
	{
		//TODO apply normal map
	}
	const Vector4D rayToIntersection = ray.GetNomalizedDirection() * (distance);
	ASSERT(rayToIntersection.Dot(ray.GetNomalizedDirection()) > 0);
	Vector4D intersectionPoint = rayToIntersection + ray.GetOrigin();

	const Color ambientColor = CalcAmbientColor(intersectionPoint);

	const Color specularAndDiffuseColor = CalcSpecularAndDiffuseLight(ray, intersectionPoint);

	unsigned int a = ambientColor.ToHex() + specularAndDiffuseColor.ToHex();
	ASSERT((a & 0xFF) == 0);
	Color	color((a >> 8) & 0xFF, (a >> 16) & 0xFF, (a >> 24) & 0xFF);

	return color;
}

Vector4D Object::CalcNormalVector(const Vector4D& intersection, const Vector4D& rayOrigin) const
{
	
	Vector4D originToIntersection = intersection - mOrigin;
	//originToIntersection.SetW(1);
	
	Vector4D normal(
		mCoefs[0].Dot(originToIntersection),
		mCoefs[1].Dot(originToIntersection),
		mCoefs[2].Dot(originToIntersection)
	);
	
	 normal = normal.Normalize();

	// inner, outter
	if (normal.Dot(intersection - rayOrigin) > 0)
		normal = normal * -1;
	return (normal);
}


Color Object::CalcAmbientColor(const Vector4D& intersectionPoint) const
{
	Color obColor = mColor;

	/*
	TODO apply texture color
	 disruption
	obj_color = get_texture_color(Q, &(Q->textures[T_TEXTURE]), intersectionPoint);
	obj_color = color_disruption(Q, intersectionPoint, obj_color);
	*/
	Color SceneAmbientColor = SceneManager::GetInstance()->GetAmbientColor();
	float SceneAmbientRatio = SceneManager::GetInstance()->GetAmbiantRatio();

	float red = roundf((float)obColor.GetRed() * ((float)SceneAmbientColor.GetRed() / 255)) * SceneAmbientRatio;
	float green = roundf((float)obColor.GetGreen() * ((float)SceneAmbientColor.GetGreen() / 255)) * SceneAmbientRatio;
	float blue = roundf((float)obColor.GetBlue() * ((float)SceneAmbientColor.GetBlue() / 255)) * SceneAmbientRatio;

	Color ambientColor(static_cast<unsigned char>(red), static_cast<unsigned char>(green), static_cast<unsigned char>(blue));

	return ambientColor;
}
