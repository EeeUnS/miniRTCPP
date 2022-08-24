#include "SceneManager.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include "Resolution.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <sstream>
#include "Plain.h"
#include "ASSERT.h"
#include <cfloat>

static constexpr const char* extension = "rt";
static constexpr const char* AMBIENT_LIGHTING_IDENTIFIER = "A";
static constexpr const char* CAMERA_IDENTIFIER = "C";
static constexpr const char* LIGHT_IDENTIFIER = "L";
static constexpr const char* QUDRICS_IDENTIFIER = "qd";
static constexpr const char* CYLINDER_IDENTIFIER = "cy";
static constexpr const char* PLAIN_IDENTIFIER = "pl";
static constexpr const char* SPHERE_IDENTIFIER = "sp";
static constexpr const char *WHITE_SPACE = " \t\v\r\f\n";

SceneManager* SceneManager::instance = nullptr;

//Scene::Scene() : mAmbientRatio(0)
//{}

// format d,d,d
Vector4D SceneManager::getVector(std::stringstream& stringstream) const
{
	std::string word;
	float ret[3];
	stringstream >> word;
	
	//std::cout << word  << std::endl;

	std::size_t sz;
	ret[0] = std::stof(word, &sz);
	checkValidCommaFormat(word[sz]);

	word = word.substr(sz + 1);
	ret[1] = std::stof(word, &sz);
	checkValidCommaFormat(word[sz]);

	word = word.substr(sz + 1);
	ret[2] = std::stof(word, &sz);
	return Vector4D(ret[0],ret[1],ret[2]);
}

// if Invalid Type program exit
void SceneManager::checkValidCommaFormat(const char a) const
{
	if (a != ',')
	{
		std::cout << "comma format invalid" << std::endl;
		exit(1);
	}
}


void SceneManager::checkValidColorRange(const int a[3]) const
{
	if (
		a[0] > 255 || a[0] < 0
		|| a[1] > 255 || a[1] < 0 
		|| a[2] > 255 || a[2] < 0
		)
	{
		std::cout << "color range invalid" << std::endl;
		exit(1);
	}
}


// format i,i,i
Color SceneManager::getColor(std::stringstream& stringstream) const
{
	std::string word;
	int ret[3];
	
	stringstream >> word;
	//std::cout << word  << std::endl;

	std::size_t sz;
	ret[0] = std::stoi(word, &sz);
	checkValidCommaFormat(word[sz]);

	word = word.substr(sz + 1);
	//std::cout << word  << std::endl;
	ret[1] = std::stoi(word, &sz);

	word = word.substr(sz + 1);
	//std::cout << word  << std::endl;
	ret[2] = std::stoi(word, &sz);
	
	checkValidColorRange(ret);

	return Color(ret[0],ret[1],ret[2]);
}

float SceneManager::getFloat(std::stringstream& stringstream) const
{
	std::string word;
	stringstream >> word;
	
	const float ret = (float)stod(word);
	return ret;
}

int SceneManager::getInt(std::stringstream& stringstream) const
{
	std::string word;
	stringstream >> word;

	const int ret = stoi(word);
	return ret;
}

void SceneManager::checkValidRitio(const float ratio) const
{
	if(ratio > 1.0 || ratio < 0)
	{
		std::cout << "ratio range 0.0 ~ 1.0" << std::endl;
		exit(1);
	}
}

void SceneManager::checkNoMoreString(std::stringstream& stringstream, const std::string &line) const
{
	std::string word;
	stringstream >> word;
	if (!word.empty())
	{
		std::cout << line << std::endl;
		std::cout << "Invalid format more arg" << std::endl;
		exit(1);
	}
}

SceneManager::SceneManager(const std::string& fileName) : mAmbientRatio(0)
{
	if (fileName.substr(fileName.find_last_of(".") + 1) != extension)
	{
		std::cout << "File extention error file end with .rt " << std::endl;
		exit(1);
	}
	std::ifstream readFile(fileName);
	std::string line;
	try
	{
		if (!readFile.is_open())
		{
			std::cout << "File not exist" << std::endl;
			exit(1);
		}

		while(getline(readFile, line))
		{
			//std::cout << line << std::endl;
			if (line.empty() || line[0] == '#')
			{
				continue;
			}
			
			std::stringstream stringstream(line);
			
			std::string word;
			stringstream >> word;
			if(word == AMBIENT_LIGHTING_IDENTIFIER)
			{
				const float bright = getFloat(stringstream);
				checkValidRitio(bright);

				const Color color = getColor(stringstream);
				mAmbientRatio = bright;
				mAmbientColor = color;
			}
			else if(word == CAMERA_IDENTIFIER)
			{
				const Vector4D origin = getVector(stringstream);
				const Vector4D direction = getVector(stringstream);
				const float hfovRadian =  static_cast<float>(getFloat(stringstream)  / 180 * M_PI);
				const Cam cam = {origin, direction, hfovRadian};
				mCam = cam;
			}
			else if(word == LIGHT_IDENTIFIER)
			{
				const Vector4D origin = getVector(stringstream);
				const float bright = getFloat(stringstream);
				checkValidRitio(bright);

				const Color color = getColor(stringstream);

				Light light = {origin, color, bright};
				AddLight(light);
			}
			else if (word == PLAIN_IDENTIFIER)
			{
				constexpr Object::eQudricsType type = Object::eQudricsType::PLANE;
				const Vector4D origin = getVector(stringstream);
				const Vector4D direction = getVector(stringstream);
				const Color color = getColor(stringstream);
				const float specularRS = getFloat(stringstream);
				const int specularNS = getInt(stringstream);
				//texture

				Plain *plain = new Plain(
					origin,
					direction,
					color,
					specularRS,
					specularNS
				);
				AddObject(plain);
			}
			else if(word == QUDRICS_IDENTIFIER)
			{
				const Vector4D origin = getVector(stringstream);
				const Vector4D direction = getVector(stringstream);
				const float coef[5] = {
					getFloat(stringstream),
					getFloat(stringstream),
					getFloat(stringstream),
					getFloat(stringstream),
					getFloat(stringstream)
				};
				const float heightRange[2] = {
					getFloat(stringstream),
					getFloat(stringstream)
				};
				const Color color = getColor(stringstream);
				const float specularRS = getFloat(stringstream);
				const int specularNS = getInt(stringstream);

				Object *object = new Object(
					coef,
					origin,
					direction,
					color,
					specularRS,
					specularNS,
					heightRange
				);
				AddObject(object);
			}
			else
			{
				std::cout << line << std::endl;
				std::cout << " Invalid Identifier " << std::endl;
				exit(1);
			}
			checkNoMoreString(stringstream, line);
		}
		readFile.close();
	}
	catch (const std::exception& e)
	{
		std::cout << line << std::endl;
		std::cout << "Something File error " << std::endl;
		std::cout << e.what() << std::endl;
		exit(1);
	}
	//
	convertOriginToCam();
}

void SceneManager::Init(const std::string& fileName)
{
	if (SceneManager::instance == nullptr)
	{
		SceneManager::instance = new SceneManager(fileName);
	}
}

SceneManager* SceneManager::GetInstance()
{
	return instance;
}

void SceneManager::AddLight(const Light& light)
{
	mLights.push_back(light);
}

void SceneManager::AddObject(Object *object)
{
	mObjects.push_back(object);
}

Cam SceneManager::GetCam()
{
	return mCam;
}

const std::vector<Light>& SceneManager::GetLights() const
{
	return mLights;
}

const std::vector<Object*>& SceneManager::GetObjects() const
{
	return mObjects;
}

std::vector<Light> &SceneManager::GetLights()
{
	return mLights;
}

std::vector<Object*> &SceneManager::GetObjects()
{
	return mObjects;
}

float SceneManager::GetAmbiantRatio()
{
	return mAmbientRatio;
}

Color SceneManager::GetAmbientColor()
{
	return mAmbientColor;
}

void SceneManager::convertOriginToCam()
{
	Matrix4x4 mat;
	{
		const Vector4D w = mCam.NomalizedDirection;
		Vector4D v;
		Vector4D u(0, 1, 0);
		if (w.GetZ() == 1)
		{
			v = Vector4D(-1, 0, 0);
		}
		else if (w.GetZ() == -1)
		{
			v = Vector4D(1, 0, 0);
		}
		else
		{
			u = Vector4D(w.GetY(), -w.GetX(), 0);
			u = u.Normalize();
			v = w.Cross(u);
		}
		mat = Matrix4x4(u, v, w);
	}
	//mat.Transpose();

	//light
	for (Light &light : mLights)
	{
		light.SetOrigin(light.GetOrigin() - mCam.Origin);
		light.SetOrigin(mat * light.GetOrigin());
	}

	//object
	mat[3].SetW(1);
	const Matrix4x4 trans = mat;
	for (Object* object: mObjects)
	{
		//TODO convert function To Mat44 member function
		const Vector4D origin  =  object->GetOrigin() - mCam.Origin;
		object->SetOrigin(mat * origin);
		
		object->SetNormalizedDirection(mat * object->GetNormalizedDirection());
		
		object->SetTan(mat * object->GetNormalizedDirection());

		const Matrix4x4 tmp  = trans * object->GetCoefs();
		object->SetCoefs(tmp * mat.Transpose());
	}

	// cam : origin 0,0,0 dir 0,0,1
	// cam must converted finally
	mCam.Origin = Vector4D(0, 0, 0);
	const Vector4D convertedVectorCam = mat * mCam.NomalizedDirection;
	mCam.NomalizedDirection = convertedVectorCam.Normalize();
	ASSERT(mCam.NomalizedDirection.GetX() < FLT_EPSILON);
	ASSERT(mCam.NomalizedDirection.GetY() < FLT_EPSILON);
	ASSERT((mCam.NomalizedDirection.GetZ() - 1) < FLT_EPSILON);
	ASSERT(mCam.NomalizedDirection.GetW() - 1 < FLT_EPSILON);
}
