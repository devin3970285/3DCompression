//----------------------------------------------------------------------------
// Compress data
//
// build ellipsoid
// find saving data
// save ellipsoid
//
//----------------------------------------------------------------------------
#ifndef _WRITECOLOREDPLY_DEFINE
#define _WRITECOLOREDPLY_DEFINE

#include <fstream>
using namespace std;

extern int rs;
extern int ls;

void write_colored_ply(char* file_name, char* file_type,  MatrixXd pointArray, MatrixXd faceArray)
{

    print("saving uncompressed data\n");

	//change file name
	strtok(file_name,".");
	strcat(file_name, ".");
	strcat(file_name, file_type);


	char* file_path = new char[strlen(FILE_OUTPUT_PATH) + strlen(file_name)];
	strcpy(file_path, FILE_OUTPUT_PATH);
	strcat(file_path, file_name);	

    ofstream out;
    out.open(file_path, ios::out|ios::binary);
    delete[] file_path;

    if (out.is_open()) 
    {
		out << "ply\n" << "format ascii 1.0\n";
		out << "comment VCGLIB generated\n";
		out << "element vertex " << pointArray.rows() << "\n";
		out << "property float x\nproperty float y\nproperty float z\n";

		out << "element face " << faceArray.rows() << "\n";
		out << "property list uchar int vertex_indices\n";
		out << "end_header\n";

		for( int i=0; i< pointArray.rows();i++)
			out << pointArray(i,0)  << " " << pointArray(i,1)  << " " << pointArray(i,2) << "\n";

		for( int i=0; i< faceArray.rows();i++)
		{
			out << (int)faceArray(i,0)   << " ";

			for (int j=1;j<=faceArray(i,0);j++)
				out << (int)faceArray(i,j)   << " ";
			out << "\n";
		}

		out.close();
    }	

    print("uncompress done\n");
}
#endif