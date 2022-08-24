#pragma once
#include <vector>
#include "Resolution.h"
#include "Ray.h"
#include "Color.h"
#include "Object.h"

class RayCastingSimulator
{
public:	
	static int objectHit;
	void executeRayCasting();
	void OutPPM();
	static RayCastingSimulator* GetInstance();
	void CalculateMinDistanceObject(const Ray& ray, float* const outDistance, const Object** outObject);
private:
	static RayCastingSimulator* instance;

	RayCastingSimulator();
	Color castSingleRay(const Ray& ray);
	void subRayCast(const std::pair<int, int>& p);

	Color mScreen[WIN_HEIGHT][WIN_WIDTH];
};

