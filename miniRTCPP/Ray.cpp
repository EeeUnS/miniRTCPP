#include "Ray.h"

Vector4D Ray::GetOrigin() const
{
    return mOrigin;
}

Vector4D Ray::GetNomalizedDirection() const
{
    return mNomalizedDirection;
}

Ray::Ray(const Vector4D& origin, const Vector4D& nomalizedDirection)
: 
    mOrigin(origin),
    mNomalizedDirection(nomalizedDirection)
{}
