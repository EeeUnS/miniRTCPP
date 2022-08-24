#include <iostream>
#include <string>
#include "MlxManager.h"
#include "timer.h"
#include "Timer.h"
#include "SceneManager.h"
#include "RayCastingSimulator.h"
#include "Resolution.h"
#include "Vector4D.h"

static_assert(sizeof(Vector4D) == (sizeof(float) * 4), "Vecotr4D is 4 float");
static_assert(sizeof(Matrix4x4) == (sizeof(float) * 4 * 4), "Matrix4x4 is 4 X 4 float");
static_assert(sizeof(Color) == (sizeof(unsigned char) * 4), "Color is 4 unsigned char");

const char *filename = "room_with_objs.rt";
//"single_circle.rt";

int main(int argc, char **argv)
{
	/*if (argc != 2 || argv == NULL)
	{
		std::cout << "Error\n\t: usage : ./miniRT <FILENAME>" << std::endl;
		return (1);
	}*/
	//std::string filename = argv[0];
	
	std::cout << "Resolution Width  :  " << WIN_WIDTH << " Height  : " << WIN_HEIGHT <<std::endl;
	Timer::start();
	
	//MlxManager::Init(filename);
	Timer::start();

	SceneManager::Init(filename);
	Timer::end("initialize");

	auto simulator = RayCastingSimulator::GetInstance();

	simulator->executeRayCasting();
	std::cout  <<  "Object Calcualte È£Ãâ È½¼ö  :  " << Object::num << std::endl;
	Timer::start();
	std::cout << "Object hit È£Ãâ È½¼ö  :  " << RayCastingSimulator::objectHit<< std::endl;

	simulator->OutPPM();
	Timer::end("out ppm");

	Timer::end("Total time");
	return 0;
}