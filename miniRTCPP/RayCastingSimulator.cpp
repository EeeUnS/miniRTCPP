#include "RayCastingSimulator.h"
#include "Resolution.h"
#include "Timer.h"
#include "Ray.h"
#include "SceneManager.h"
#include "ASSERT.h"
#include <fstream>
#include <iostream>
#include "Color.h"
#include "DDraw.h"
#include "future"

RayCastingSimulator* RayCastingSimulator::instance = nullptr;


RayCastingSimulator::RayCastingSimulator()
{
	memset(mScreen, 0, WIN_WIDTH * WIN_HEIGHT);
}

void RayCastingSimulator::OutPPM()
{
	//ppm
/*
*   out << "P3\n" << W << ' ' << H << ' ' << "255\n";
	  out << (int)pix_col.x << ' '
		  << (int)pix_col.y << ' '
		  << (int)pix_col.z << '\n';
*/
	try
	{
		std::ofstream ppm("output.ppm");
		ppm << "P3\n" << WIN_WIDTH << ' ' << WIN_HEIGHT << ' ' << "255\n";
		for (int j = 0; j < WIN_HEIGHT; j++)
		{
			for (int i = 0; i < WIN_WIDTH; i++)
			{
				char arr[32];
				sprintf_s(arr, "%d %d %d\n", static_cast<int>(mScreen[j][i].GetRed()), static_cast<int>(mScreen[j][i].GetGreen()), static_cast<int>(mScreen[j][i].GetBlue()));
				ppm << arr;
			}
		}
		ppm.close();
	}
	catch (const std::exception&)
	{
		std::cout << "Someting error out ppm " << std::endl;
	}
}


//void SubProcedure(int i)
//{
//	RayCastingSimulator *simulator = RayCastingSimulator::GetInstance();
//	while (simulator->m_Exit[i])
//	{
//		WaitForSingleObject(, INFINITE);
//
//		simulator->subRayCast(simulator->m_xyStartPair[i]);
//	}
//}

void RayCastingSimulator::Initialize(HWND hWnd)
{
	m_hWnd = hWnd;
	m_pDDraw = std::make_unique<CDDraw>();

	m_pDDraw->InitializeDDraw(hWnd);

	
	for (int i = 0; i < nThreadCount; i++)
	{
		m_xyStartPair[i].first = WIN_WIDTH / 2 * (i / 2);
		m_xyStartPair[i].second = WIN_HEIGHT / 2 * (i % 2);
		//m_Exit[i] = false;
	}

	//for (int i = 0 ; i < nThreadCount ; ++i )
	//{
	//	//m_MultiThread[i] = std::thread(SubProcedure, i);
	//}
	
}

RayCastingSimulator* RayCastingSimulator::GetInstance()
{
	if (instance == nullptr)
		instance = new RayCastingSimulator();
	return instance;
}

// TODO goto Ray member function
//find min distance Intersect Object
void RayCastingSimulator::CalcMinDistanceObject(const Ray& ray, float* const outDistance, const Object** outObject)
{
	*outDistance = INFINITY;

	//find min distance Intersect Object
	const auto& objects = SceneManager::GetInstance()->GetObjects();
	for (const Object* object : objects)
	{
		const float distance = object->CalcIntersectDistanceOrNan(ray);
		if (!isnan(distance) && distance < *outDistance)
		{
			*outDistance = distance;
			*outObject = object;
		}
	}
}

int RayCastingSimulator::objectHit = 0;

Color RayCastingSimulator::castSingleRay(const Ray& ray)
{
	float minDistance = INFINITY;
	const Object* minDistanceObject = nullptr;
	CalcMinDistanceObject(ray, &minDistance, &minDistanceObject);

	if (minDistanceObject == nullptr)
	{
		return Color(0, 0, 0);
	}

	ASSERT(minDistance > FLT_EPSILON);
	ASSERT(minDistance != INFINITY);
	ASSERT(minDistanceObject != nullptr);
	objectHit++;

	//Phong reflection model

	const Vector4D rayToIntersection = ray.GetNomalizedDirection() * (minDistance);
	ASSERT(rayToIntersection.Dot(ray.GetNomalizedDirection()) > 0);
	Vector4D intersectionPoint = rayToIntersection + ray.GetOrigin();

	// phong_reflection;
	{
		//TODO apply normal map
	}

	Color color = minDistanceObject->CalcPhongModelColor(ray, minDistance);
	//unsigned int a = ambientColor.ToHex() + specularColor.ToHex() + diffuseColor.ToHex();
	//unsigned int a = ambientColor.ToHex() + specularColor.ToHex(); //+ diffuseColor.ToHex();/// +specularColor.ToHex();
	//ASSERT((a & 0xFF) == 0);
	//
	//Color	color((a>>8)&0xFF, (a >> 16) & 0xFF, (a >> 24) & 0xFF);
	color.IntensityAttenuation(intersectionPoint, ray.GetOrigin());
	return color;
}

void RayCastingSimulator::subRayCast(const std::pair<int, int>& m_xyStartPair)
{
	for (int y = 0; y < WIN_HEIGHT / 2; y++)
	{
		for (int x = 0; x < WIN_WIDTH / 2; x++)
		{
			const Vector4D rayOrigin = SceneManager::GetInstance()->GetCam().Origin;
			//rayOrigin.SetW(1);

			//ASSERT(p.first + x - WIN_WIDTH / 2 != -411 && p.second + y - WIN_HEIGHT / 2 != -36);
			Vector4D rayNomalizedDirection = Vector4D(
				static_cast<float>(m_xyStartPair.first + x - WIN_WIDTH / 2),
				static_cast<float>(m_xyStartPair.second + y - WIN_HEIGHT / 2),
				static_cast<float>(SceneManager::GetInstance()->GetCam().DistanceCamToScreen)
			);
			rayNomalizedDirection = rayNomalizedDirection.Normalize();
			const Ray ray = {
				rayOrigin,
				rayNomalizedDirection
			};

			ASSERT(abs(ray.GetNomalizedDirection().GetW()) < FLT_EPSILON);
			//ASSERT(abs(ray.GetOrigin().GetW()) < FLT_EPSILON);

			const Color color = castSingleRay(ray);

			//TODO Editing mode add
			mScreen[m_xyStartPair.second + y][m_xyStartPair.first + x] = color;

			//pixel[0] += (GetSceneEditer()->edit + 1);
		}
		//pixel[1] += (GetSceneEditer()->edit + 1);
	}
	//return (NULL);
}

RayCastingSimulator::~RayCastingSimulator()
{
}


void RayCastingSimulator::executeRayCasting()
{
	Timer::start();


	/*for (int i = 0; i < 4; i++)
	{
		subRayCast( m_xyStartPair[i]);
	}*/

	std::future<void> a[4];

	for (int i = 0; i < 4; i++)
	{
		a[i] = std::async(std::launch::async, &RayCastingSimulator::subRayCast, this, m_xyStartPair[i]);
	}
	
	for (int i = 0; i < 4; i++)
	{
		a[i].wait();
	}


	Timer::end("raycasting time");
}


void RayCastingSimulator::DrawScene()
{
	// 메시지가 없으면 게임루프

	m_pDDraw->BeginDraw();

	m_pDDraw->Clear();


	m_pDDraw->DrawBitmap(0, 0, WIN_WIDTH, WIN_HEIGHT, (char *)mScreen);

	m_pDDraw->EndDraw();

	HDC	hDC = nullptr;

	// FPS 표시
	if (m_pDDraw->BeginGDI(&hDC))
	{
		m_pDDraw->DrawInfo(hDC);
		m_pDDraw->EndGDI(hDC);
	}

	m_pDDraw->OnDraw();
	m_pDDraw->CheckFPS();

}