#pragma once
#include "Light.h"
#include "Object.h"

//Singletone
class SceneEditer
{
public:
	enum e_target
	{
		E_NONE = 0,
		E_LIGHT,
		E_CAM,
		E_OBJ
	};
private:
	Object* selectedObject;
	Light* selectedLight;
	bool isEditMode;
	enum e_target target_scene;
	unsigned int clicked;
	int 	prev_pixel[2];
	int debug;
};
