#pragma once
#include "Cam.h"
#include <vector>
#include "Light.h"
#include "Object.h"
#include <string>
#include "Color.h"
//Singletone
class SceneManager
{
public:
	static void Init(const std::string& fileName);
	static SceneManager* GetInstance();
	void AddLight(const Light &light);
	void AddObject(Object *object);
	Cam GetCam();
	
	const std::vector<Light> &GetLights() const;
	const std::vector<Object*> &GetObjects() const;

	std::vector<Light> &GetLights();
	std::vector<Object *> &GetObjects();

	float GetAmbiantRatio();
	Color GetAmbientColor();

private:

	static SceneManager* instance;
	//Scene();
	SceneManager(const std::string& fileName);

	//Cam convert 0,0,0
	void convertOriginToCam();

	float getFloat(std::stringstream& stringstream) const;
	Vector4D getVector(std::stringstream& stringstream)  const;
	Color getColor(std::stringstream& stringstream)  const;
	int getInt(std::stringstream& stringstream) const;

	// If invalid exit program
	void checkValidCommaFormat(const char a)  const;
	// If invalid exit program
	void checkValidColorRange(const int a[3]) const;
	// If invalid exit program
	void checkValidRitio(const float ratio) const;
	// If invalid exit program
	void checkNoMoreString(std::stringstream& stringstream, const std::string& line) const;

	float mAmbientRatio;
	Color mAmbientColor;
	Cam mCam;
	std::vector<Light> mLights;
	std::vector<Object*> mObjects;
};
