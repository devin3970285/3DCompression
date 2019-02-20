//----------------------------------------------------------------------------
// Main Process
//
// reading file
// compute
// save file
//
//----------------------------------------------------------------------------


#include "Eigen/Dense" 
using namespace Eigen;

#include "default_header.h"
#include "ReadFile.h"
#include "SaveFile.h"
#include "PC2EO.h"
#include "OCL2OCI.h"
#include "writeEOUint8_withMesh.h"
#include "readEOUint8_withMesh.h"
#include "EO2PC_withIndex.h"
#include "write_colored_ply.h"

// all the point we got.
MatrixXd pointArray;
//face for show in window.
MatrixXd faceArray;


MatrixXd* OccupancyList = NULL;
//std::shared_ptr<MatrixXd> OccupancyList;
MatrixXd** EllipsoidList = NULL;
MatrixXd Left_PC;

MatrixXd OccupancyImage;
MatrixMapVd EllipsoidMap;
MatrixMapVd EllipsoidMapIndex;


int rs;
int ls;

int main(int argc,char **argv)
{ 

	/// parameter file
	int r = 5;
	rs = 361*r;
	ls = 181*r;

	int N = 50;
	int MethodOpt = SPHEREOUT; 
	int ErrorOpt = PROPORTION;

	VectorXd Parameter = VectorXd::Zero(4);
	Parameter << 1,  0.05,  1,  200;

	//printf("test........%d.........%s\n",argc,*(argv+2));

	//-- reading file.
	char* file_name = new char[100];
	strcpy(file_name, *(argv+1));

	char* file_type = new char[100];

	Read_File(file_name,file_type);

	// write_colored_ply(file_name, file_type, pointArray, faceArray);
	// exit(0);


	//-- compute
	OccupancyList = new MatrixXd[N];
	EllipsoidList = new MatrixXd*[N];

	PC2EO(pointArray,MethodOpt,ErrorOpt,Parameter,N);

	//Obtain the OccupancyImage (a representation of all the points in the
	//occupancy grid) using the occupancy list and the resolution.
	//retrun OccupancyImage, EllipsoidMap,EllipsoidMapIndex
	OCL2OCI(r,N);

	//-- saving file -- compressed file.
	writeEOUint8_withMesh(file_name,MethodOpt,r,N);


	// delete old data, start reading from compressed file.
	pointArray.resize(0,0);
	//faceArray.resize(0,0);
	Left_PC.resize(0,0);
	OccupancyImage.resize(0,0);
	EllipsoidMap.resize(0,0);
	EllipsoidMapIndex.resize(0,0);

	OccupancyList = NULL;
	delete[] OccupancyList;

	for(int i=0;i<N;i++)
	{
		EllipsoidList[i] = NULL;
		delete[] EllipsoidList[i];
	}
	EllipsoidList = NULL;
	delete[] EllipsoidList;


	////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////


	// -- reading middle file.
	// OccupancyList = new MatrixXd[N];
	// EllipsoidList = new MatrixXd*[N];

	MethodOpt = readEOUint8_withMesh(file_name);

	//-- un-compute

	EO2PC_withIndex(MethodOpt, N);
	//print(pointArray);

	//-- saving file -- original file again.

	write_colored_ply(file_name, file_type, pointArray, faceArray);


	OccupancyList = NULL;
	delete[] OccupancyList;

	for(int i=0;i<N;i++)
	{
		EllipsoidList[i] = NULL;
		delete[] EllipsoidList[i];
	}
	EllipsoidList = NULL;
	delete[] EllipsoidList;

	file_name = NULL;
	file_type = NULL;
	delete[] file_name;
	delete[] file_type;

	print("out of system\n");

	return 0;
}
