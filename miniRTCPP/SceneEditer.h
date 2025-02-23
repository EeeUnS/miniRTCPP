#pragma once
#include "Light.h"
#include "Object.h"

//Singletone

enum class eTarget
{
	NONE = 0,
	LIGHT,
	CAM,
	OBJ
};

enum class eDebugMode
{
	NONE = 0,
	SIMPLE,
	DETAIL
}	t_debug_mode;


class SceneEditer
{
public:
	Object* selectedObject = nullptr;
	Light* selectedLight = nullptr;
	Object* selectedObject = nullptr;
	unsigned int		edit;

	//bool isEditMode = false;
	eTarget targetScene = eTarget::NONE;
	unsigned int clicked = 0;
	int 	prev_pixel[2] = {0,0};
	eDebugMode debug = eDebugMode::NONE;
};


SceneEditer* GetSceneEditer();