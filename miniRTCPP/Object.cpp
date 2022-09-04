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

float Object::diffuseHelper(const Light &light, const Vector4D &intersectionNormal, const Vector4D &intersectionPoint) const
{
	const Vector4D pToLight = light.GetOrigin() - intersectionPoint;
	const Ray intersectionToLight(
		intersectionPoint,
		pToLight.Normalize()
	);
	float diffuse = intersectionToLight.GetNomalizedDirection().Dot(intersectionNormal);
	if (diffuse < 0)
	{
		return 0;
	}

	float distance;
	const Object *object;
	RayCastingSimulator::GetInstance()->CalculateMinDistanceObject(intersectionToLight, &distance, &object);
	const float pToLightLen = pToLight.Length();
	if (!isnan(distance) && distance < pToLightLen + FLT_EPSILON)
	{
		// on other object in light ray, intersectionPoint is shadow
		return 0;
	}

	diffuse *= light.GetBright();
	return diffuse;
}

Color Object::CalcDiffuseColor(const Vector4D& intersectionPoint, const Vector4D& intersectionNormal) const
{
	Color obColor = mColor;
	
	//TODO texture, disruption
	Color diffuseColor;
	const std::vector<Light> &lights = SceneManager::GetInstance()->GetLights();
	for (const Light& light : lights)
	{
		const float diffuse = diffuseHelper(light, intersectionNormal, intersectionPoint);
		if (diffuse < FLT_EPSILON)
		{
			continue;
		}

		const Color lightColor = light.GetColor();
		float red = (((float)lightColor.GetRed() * ((float)obColor.GetRed() / 255)));
		float green = (((float)lightColor.GetGreen() * ((float)obColor.GetGreen() / 255)));
		float blue = (((float)lightColor.GetBlue() * ((float)obColor.GetBlue() / 255)));
		const float scale = (diffuse * (1 - mSpecularRS));
		red = red * scale;
		green = green * scale;
		blue = blue * scale;
		
		const Color diffuseSingleLightColor(
			static_cast<unsigned char>(red), 
			static_cast<unsigned char>(green), 
			static_cast<unsigned char>(blue)
		);
		diffuseColor = diffuseColor + diffuseSingleLightColor;
	}
	return diffuseColor;
}

//TODO naming change
float	Object::specular_helper(
	const Light &light,
	const Vector4D &normalizedReflectedRay,
	const Vector4D &intersectionPoint
) const 
{
	//ASSERT(mCoefs[3].GetW() != -25);
	const Vector4D pToLight = light.GetOrigin() - intersectionPoint;
	const Ray intersectionToLight(
		intersectionPoint,
		pToLight.Normalize()
	);

	float specular = intersectionToLight.GetNomalizedDirection().Dot(normalizedReflectedRay);
	//ASSERT(specular >= FLT_EPSILON);
	if (specular <= FLT_EPSILON)
	{
		return 0;
	}
	//ASSERT(mCoefs[3].GetW() != -25, "for debug");
	float distance = NAN;
	const Object* object = nullptr;
	RayCastingSimulator::GetInstance()->CalculateMinDistanceObject(intersectionToLight, &distance, &object);
	const float pToLightLength = pToLight.Length();
	if (!isnan(distance) && distance < pToLightLength + FLT_EPSILON)
		return (0);
	return (specular);
}

Color Object::CalcSpecularLight(const Vector4D& normalizedReflectedRay, const Vector4D& intersectionPoint) const
{
	
	Color color;
	const auto &lights = SceneManager::GetInstance()->GetLights();

	for (const Light& light : lights)
	{
		float specular = specular_helper(light, normalizedReflectedRay, intersectionPoint);
		specular = mSpecularRS * powf(specular, static_cast<float>(mSpecularNS));
		if (specular < FLT_EPSILON)
		{
			continue;
		}
		
		const float scale = specular * light.GetBright();
		const Color lightSpecular = light.GetColor() * scale ;
		color = color + lightSpecular;
	}

	return (color);
}
