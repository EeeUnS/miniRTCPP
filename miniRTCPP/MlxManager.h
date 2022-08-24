//#pragma once
//
//#include "mlx.h"
//#include <string>
//#include "Resolution.h"
//
//struct Image
//{
//	void* img;
//	char* addr;
//	int		bpp;
//	int		line;
//	int		endian;
//};
//
//typedef struct s_xpm
//{
//	Image	img;
//	int		img_width;
//	int		img_height;
//}	t_xpm;
//
//class MlxManager
//{
//public:
//	static void Init(const std::string& fileName);
//	static MlxManager*  getInstance();
//private:
//	static MlxManager* instance;
//	void* mMlx;
//	void* mWin;
//	Image mImage;
//	MlxManager(const std::string& fileName);
//};
//
//void MlxManager::Init(const std::string& fileName)
//{
//	if (instance == nullptr)
//		instance = new MlxManager(fileName);
//}
//
//MlxManager* MlxManager::getInstance()
//{
//	return instance;
//}
//
//MlxManager::MlxManager(const std::string& fileName)
//{
//	mMlx = mlx_init();
//	mWin = mlx_new_window(mMlx, WIDTH, HEIGHT,  const_cast<char *>(fileName.c_str()));
//	mImage.img = mlx_new_image(mMlx, WIDTH, HEIGHT);
//	mImage.addr = mlx_get_data_addr(mImage.img, &(mImage.bpp), &(mImage.line), &(mImage.endian));
//}

