#include "SceneEditer.h"


SceneEditer* GetSceneEditer()
{
	static SceneEditer scene_editer;

	return (&scene_editer);
}
