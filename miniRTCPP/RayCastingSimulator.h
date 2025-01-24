#pragma once
#include <vector>
#include "Resolution.h"
#include "Ray.h"
#include "Color.h"
#include "Object.h"
#include "Windows.h"
#include "DDraw.h"
#include <memory>

class RayCastingSimulator
{
public:	
	static int objectHit;
	void executeRayCasting();
	void DrawScene();
	void OutPPM();
	void Initialize(HWND window);
	static RayCastingSimulator* GetInstance();
	void CalcMinDistanceObject(const Ray& ray, float* const outDistance, const Object** outObject);
private:
	RayCastingSimulator();
	Color castSingleRay(const Ray& ray);
	void subRayCast(const std::pair<int, int>& p);

	HWND m_hWnd = nullptr;
	std::unique_ptr<CDDraw> m_pDDraw = nullptr;
	
	DWORD m_dwCurFPS = 0;


	Color mScreen[WIN_HEIGHT][WIN_WIDTH];
	~RayCastingSimulator();
	static RayCastingSimulator* instance;
	static constexpr DWORD m_dwFPS = 60;
	static constexpr float m_fTicksPerGameFrame = 16.6f;
};

