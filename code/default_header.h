
//----------------------------------------------------------------------------
// Definition
//
//
//----------------------------------------------------------------------------
#ifndef ONCE
#define ONCE
// source file mac
#define FILE_INPUT_PATH  "/Users/Devin/Documents/3D-Reconstruction/C++/src/"
// source file windows
//#define FILE_INPUT_PATH  "C:\\Users\\zhang\\Desktop\\3d_reconstruction\\C++\\src\\"

// result file mac
#define FILE_OUTPUT_PATH  "/Users/Devin/Documents/3D-Reconstruction/C++/result/"
// result file mac
//#define FILE_OUTPUT_PATH  "C:\\Users\\zhang\\Desktop\\3d_reconstruction\\C++\\result\\"

// mid-compressed file mac
#define FILE_COMPRESS_PATH  "/Users/Devin/Documents/3D-Reconstruction/C++/mid_result/"
// mid-compressed file mac
//#define FILE_COMPRESS_PATH  "C:\\Users\\zhang\\Desktop\\3d_reconstruction\\C++\\mid_result\\"

//fomate for middle-compressed file
#define MIDDLE_FILE_NAME ".cnm"

//MOTHED OPTION
#define SPHEREBUILDIN 1
#define SPHEREOUT 2
#define ELLIPSOIDOUT 3

#define PROPORTION 4
#define FIXEDNUMBER 5
#define FIXEDERROR 6

typedef Matrix<VectorXd, Dynamic, Dynamic> MatrixMapVd;



#include <iostream>
//#include <typeinfo>

template<typename T>       
static void print(T data)
{
	//how to printf double with more than 6 digit. for c language.
	//printf("---%.*lf  \n",10,data);

	//more convient in cout in c++
	// if (strcmp(typeid(data).name(), "d") == 0)
	// 	std::cout.precision(4);
	std::cout<< "+++" << data << std::endl;
}

#endif
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------


