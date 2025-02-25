#pragma once
#include <vector>
#include "Resolution.h"
#include "Ray.h"
#include "Color.h"
#include "Object.h"
#include "Windows.h"
#include "DDraw.h"
#include <memory>
#include <thread>

class RayCastingSimulator
{
public:	
	static int objectHit;
	void executeRayCasting();
	void DrawScene();
	void OnDraw();
	void OutPPM();
	void Initialize(HWND window);
	static RayCastingSimulator* GetInstance();
	void CalcMinDistanceObject(const Ray& ray, float* const outDistance, const Object** outObject);
	
	static constexpr int nThreadCount = 4;
	void subRayCast(const std::pair<int, int>& m_xyStartPair);
	std::pair<int, int> m_xyStartPair[nThreadCount];
	bool m_Exit[nThreadCount];
	bool m_JobEnd[nThreadCount];

private:

	RayCastingSimulator();
	Color castSingleRay(const Ray& ray);
	

	HWND m_hWnd = nullptr;
	std::unique_ptr<CDDraw> m_pDDraw = nullptr;
	
	DWORD m_dwCurFPS = 0;
	std::thread m_MultiThread[nThreadCount];

	Color mScreen[WIN_HEIGHT][WIN_WIDTH];
	~RayCastingSimulator();
	static RayCastingSimulator* instance;
	static constexpr DWORD m_dwFPS = 60;
	static constexpr float m_fTicksPerGameFrame = 16.6f;
};

