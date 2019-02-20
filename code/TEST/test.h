
//----------------------------------------------------------------------------
// Definition
//
//
//----------------------------------------------------------------------------
#ifndef ONCE
#define ONCE
// source file mac
//#define FILE_INPUT_PATH  "/Users/Devin/Documents/3D-Reconstruction/C++/src/"
// source file windows
#define FILE_INPUT_PATH  "C:\\Users\\zhang\\Desktop\\3d_reconstruction\\C++\\src\\"

// result file mac
//#define FILE_OUTPUT_PATH  "/Users/Devin/Documents/3D-Reconstruction/C++/result/"
// result file mac
#define FILE_OUTPUT_PATH  "C:\\Users\\zhang\\Desktop\\3d_reconstruction\\C++\\result\\"

// mid-compressed file mac
//#define FILE_COMPRESS_PATH  "/Users/Devin/Documents/3D-Reconstruction/C++/mid_result/"
// mid-compressed file mac
#define FILE_COMPRESS_PATH  "C:\\Users\\zhang\\Desktop\\3d_reconstruction\\C++\\mid_result\\"

//fomate for middle-compressed file
#define MIDDLE_FILE_NAME ".cnm"

struct Point
{
	double x;
	double y;
	double z;
};

struct Face
{
	int num_points;
	long index_1;
	long index_2;
	long index_3;
	long index_n;
	Point point_1;
	Point point_2;
	Point point_3;
	Point normal;
};

char* splite(char* data)
{
	char* temp = new char[100];
	return temp;
}

#include <iostream>
#include <typeinfo>
template<typename T>       
static void print(T data)
{
	//how to printf double with more than 6 digit. for c language.
	//printf("---%.*lf  \n",10,data);

	//more convient in cout in c++
	// if (strcmp(typeid(data).name(), "d") == 0)
	// 	std::cout.precision(4);
	std::cout<< "----" << data << std::endl;
}


#endif
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------


