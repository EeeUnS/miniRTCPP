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
#include "ASSERT.h"
#include "fstream"

std::fstream outfile ("output.txt", std::ios::out);

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
		CW_USEDEFAULT, 0, WIN_WIDTH, WIN_HEIGHT, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return false;
	}
	g_hMainWindow = hWnd;
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return true;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL,	IDI_APPLICATION);              // predefined app. icon 
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = L"MainMenu";    // name of menu resource 
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(hInstance, // small class icon 
		MAKEINTRESOURCE(5));

	//wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		//case IDM_ABOUT:
		//	DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		//	break;
		//case IDM_EXIT:
		//	DestroyWindow(hWnd);
		//	break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_SIZE:
		//if (g_pGame)
		//{
		//	g_pGame->OnUpdateWindowSize();
		//}
		break;
	case WM_MOVE:
		/*if (g_pGame)
		{
			g_pGame->OnUpdateWindowPos();
		}*/
		break;
	case WM_KEYDOWN:
	{

		UINT	uiScanCode = (0x00ff0000 & lParam) >> 16;
		UINT	vkCode = MapVirtualKey(uiScanCode, MAPVK_VSC_TO_VK);
		if (!(lParam & 0x40000000))
		{
			//if (g_pGame)
			//{
			//	g_pGame->OnKeyDown(vkCode, uiScanCode);
			//}

		}
	}
	break;
	case WM_KEYUP:
	{
		UINT	uiScanCode = (0x00ff0000 & lParam) >> 16;
		UINT	vkCode = MapVirtualKey(uiScanCode, MAPVK_VSC_TO_VK);
		//if (g_pGame)
		//{
		//	g_pGame->OnKeyUp(vkCode, uiScanCode);
		//}
	}
	break;
	case WM_SYSKEYDOWN:
	{
		UINT	uiScanCode = (0x00ff0000 & lParam) >> 16;
		UINT	vkCode = MapVirtualKey(uiScanCode, MAPVK_VSC_TO_VK);
		BOOL	bAltKeyDown = false;
		if ((HIWORD(lParam) & KF_ALTDOWN))
		{
			bAltKeyDown = true;
		}
		//if (!g_pGame->OnSysKeyDown(vkCode, uiScanCode, bAltKeyDown))
		//{
		//	DefWindowProc(hWnd, message, wParam, lParam);
		//}
	}
	break;
	/*case WM_KEYDOWN:
case WM_SYSKEYDOWN:
	if (wParam == VK_RETURN)
		if ((HIWORD(lParam) & KF_ALTDOWN))
			ToggleFullscreen();
	break;*/

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
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

		//LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
		//LoadStringW(hInstance, IDC_DDrawSample, szWindowClass, MAX_LOADSTRING);

		MyRegisterClass(hInstance);
		if (!InitInstance(hInstance, nCmdShow))
		{
			ASSERT(false);
			return false;
		}

		RayCastingSimulator::GetInstance()->Initialize(g_hMainWindow);
	}

	
	outfile << "Resolution Width  :  " << WIN_WIDTH << " Height  : " << WIN_HEIGHT <<std::endl;
	Timer::start();
	
	//MlxManager::Init(filename);
	Timer::start();

	SceneManager::Init(filename);
	Timer::end("initialize");

	while (1) {
		auto simulator = RayCastingSimulator::GetInstance();

		simulator->executeRayCasting();
		
		outfile << "Object Calcualte 호출 횟수  :  " << Object::num << std::endl;
		outfile << "Object hit 호출 횟수  :  " << RayCastingSimulator::objectHit << std::endl;

		Timer::start();

		simulator->DrawScene();
		
		Timer::end("out ppm");

	}
	Timer::end("Total time");
	
	return 0;
}