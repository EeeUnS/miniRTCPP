#pragma once

#include "Color.h"
#include "Matrix4x4.h"
#include "Ray.h"
#include "Light.h"

//TODO object range check 
class Object
{
public:
	enum class eQudricsType
	{
		PLANE = 0,
		QUADRICS
	};
	enum class eTextureType
	{
		NORMAL = 0,
		TEXTURE,
	};

	Object(
		const float coefs[5],
		const Vector4D &origin,
		const Vector4D &direction, 
		const Color &color,
		const float specularRS,
		const int  specularNS,
		const float heightRange[2]
		);
	Object(
		const Vector4D& origin,
		const Vector4D& direction,
		const Color& color,
		const float specularRS,
		const int  specularNS,
		const float heightRange[2]
	);
	Color GetColor() const;

	Vector4D GetOrigin() const;
	void SetOrigin(const Vector4D &a);
	Vector4D GetNormalizedDirection() const;
	void SetNormalizedDirection(const Vector4D& a);
	
	Vector4D GetTan();
	void SetTan(const Vector4D &tan);
	Matrix4x4 GetCoefs();
	void SetCoefs(const Matrix4x4& coefs);

	static int num;

	//virtual
	virtual Vector4D CalcNormalVector(const Vector4D& intersection, const Vector4D& rayOrigin) const;
	virtual float CalcIntersectDistanceOrNan(const Ray& ray) const;

	Color CalcPhongModelColor(const Ray &ray, const float distance) const;
	void CalcSpecularAndDiffuseScale(
		const Light& light,
		const Ray& ray,
		const Vector4D& intersectionNormal,
		const Vector4D& intersectionPoint,
		float* const outDiffuseScale,
		float* const outSpecularScale
	) const;

protected:
	Matrix4x4 mCoefs;

private:
	Vector4D getTan() const;
	bool isHeightRange(const Ray& ray, float distance) const;
	Color calcSpecularAndDiffuseLight(const Ray& ray, const Vector4D& intersectionPoint) const;
	Color calcAmbientColor(const Vector4D& intersectionPoint) const;

	Vector4D mOrigin;
	Vector4D mNormalizedDirection;
	Vector4D mTan;
	Color mColor;
	float mSpecularRS;
	int  mSpecularNS;
	float mHeightRange[2];
	bool  mDisruption;
	//t_xpm textures[2];
};
