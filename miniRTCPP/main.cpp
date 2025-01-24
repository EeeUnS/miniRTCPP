#include <iostream>
#include <string>
#include "MlxManager.h"
#include "timer.h"
#include "Timer.h"
#include "SceneManager.h"
#include "RayCastingSimulator.h"
#include "Resolution.h"
#include "Vector4D.h"
#include "Windows.h"

static_assert(sizeof(Vector4D) == (sizeof(float) * 4), "Vecotr4D is 4 float");
static_assert(sizeof(Matrix4x4) == (sizeof(float) * 4 * 4), "Matrix4x4 is 4 X 4 float");
static_assert(sizeof(Color) == (sizeof(unsigned char) * 4), "Color is 4 unsigned char");

const char *filename = "room_with_objs.rt";
//"single_circle.rt";

HWND	g_hMainWindow = nullptr;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

constexpr int MAX_LOADSTRING = 64;

HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING] = L"연습용";                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING] = L"DDD";            // the main window class name

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}
	g_hMainWindow = hWnd;
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	/*if (argc != 2 || argv == NULL)
	{
		std::cout << "Error\n\t: usage : ./miniRT <FILENAME>" << std::endl;
		return (1);
	}*/
	//std::string filename = argv[0];

	{// init
		if (!InitInstance(hInstance, nCmdShow))
		{
			return FALSE;
		}

		RayCastingSimulator::GetInstance()->Initialize(g_hMainWindow);

	}

	
	std::cout << "Resolution Width  :  " << WIN_WIDTH << " Height  : " << WIN_HEIGHT <<std::endl;
	Timer::start();
	
	//MlxManager::Init(filename);
	Timer::start();

	SceneManager::Init(filename);
	Timer::end("initialize");

	auto simulator = RayCastingSimulator::GetInstance();

	simulator->executeRayCasting();
	std::cout  <<  "Object Calcualte 호출 횟수  :  " << Object::num << std::endl;
	Timer::start();
	std::cout << "Object hit 호출 횟수  :  " << RayCastingSimulator::objectHit<< std::endl;

	simulator->OutPPM();
	Timer::end("out ppm");

	Timer::end("Total time");
	return 0;
}