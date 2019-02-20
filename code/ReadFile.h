
//----------------------------------------------------------------------------
// Read different kinds of file.
//
// .off
// .ply
// .obj
//
//----------------------------------------------------------------------------
#ifndef _READFILE_DEFINE
#define _READFILE_DEFINE

//#include <stdio.h>
//#include <cstring>
// #include "default_header.h"
// #include "Eigen/Dense" 
// using namespace Eigen;

extern MatrixXd pointArray;
extern MatrixXd faceArray;

void Read_off(FILE *fp)
{

	int num_vertex = 0, num_face = 0, num_edge = 0;

	char* buffer = new char[10];
	fscanf(fp, "%s", buffer);
	if (strcmp(buffer, "OFF") != 0)
	{
		perror("This file is demaged\n");
		exit(0);	
	}
	buffer = NULL;
	delete[] buffer;

	// number of vertex and face, no edge in .off file
	fscanf(fp, "%d %d %d", &num_vertex, &num_face, &num_edge);

	// position of vertex
	int i=0;
	
	pointArray = MatrixXd(num_vertex,4);

	double x,y,z;
	while(i < num_vertex)
	{	
		// could be wrong,different version, change it to %f
		fscanf(fp, "%lf %lf %lf", &x, &y, &z);
		pointArray(i,0) = x;
		pointArray(i,1)	= y;
		pointArray(i,2) = z;
		pointArray(i,3) = i;

		i++;
	}

	// consistence of face
	int num_point;
	i=0;

	faceArray = MatrixXd(num_face,3);
}
void Read_ply(FILE *fp)
{
	int num_vertex = 0, num_face = 0, num_edge = 0;

	char* buffer = new char[200];
	fscanf(fp, "%s", buffer);
	if (strcmp(buffer, "ply") != 0)
	{
		perror("This file is demaged\n");
		exit(0);	
	}

  	char* vertex = new char[20];
  	char* face = new char[20];
  	char* end = new char[20];
    while (true)
    {
    	fgetc(fp);
    	//fscanf(fp, "%c");
    	fscanf(fp, "%[^\n]", buffer);

    	vertex = strstr(buffer,"element vertex");
    	face = strstr(buffer,"element face");

    	if (vertex != NULL)
    	{
    		vertex = vertex + strlen("element vertex ");
    		num_vertex = atoi(vertex);
    	}
    	else if(face != NULL)
    	{
    		face = face + strlen("element face ");
    		num_face = atoi(face);
    	}
    	else
    	{
    		end = strstr(buffer,"end_header");
	    	if (end != NULL)
	    		break;
    	}

    	// print(buffer);
    	// now only works for ascii file.
    }
    vertex = NULL;
	face = NULL;
	end = NULL;
	buffer = NULL;

   	delete[] vertex;
	delete[] face;
	delete[] end;
	delete[] buffer;

    printf("\nthis file contains %d points  %d faces\n\n", num_vertex, num_face);

    float x,y,z;
	pointArray = MatrixXd(num_vertex,4);

    for (int i=0; i<num_vertex; i++)
    {
    	//从当前这个位置开始移动
		//fseek ( fp , 9 , SEEK_CUR );

    	fscanf(fp, "%f %f %f", &x, &y, &z);

    	pointArray(i,0) = x;
    	pointArray(i,1) = y;
    	pointArray(i,2) = z;
    	pointArray(i,3) = i;

    	// skip other usless data except xyz
    	while(fgetc(fp)!='\n'){}
    }
	
	// read face
	int arrayCols = 4;
	int l,e;
	faceArray = MatrixXd::Zero(num_face,arrayCols);
	for (int i=0;i< num_face;i++)
	{
	    fscanf(fp, "%d", &l);
		if ((l+1) > arrayCols)
		{
			faceArray.conservativeResize(num_face,++arrayCols);
			faceArray.col(l).fill(0);
		}
		faceArray(i,0) = l;
		for (int j=1;j<=l;j++)
		{
    		fscanf(fp, "%d", &e);
    		faceArray(i,j) = e;
		}

		// skip other usless data except xyz
    	while(fgetc(fp)!='\n'){}
	}

}
void Read_obj(FILE *fp)
{
	
}

void Open_File(char* file_name, char* file_type)
{
	// open this file.
	FILE *fp;

	char* file_path = new char[strlen(FILE_INPUT_PATH) + strlen(file_name)];
	strcpy(file_path, FILE_INPUT_PATH);
	strcat(file_path, file_name);	

	print(file_path);
    if ((fp = fopen(file_path,"r")) == NULL)
	{
		perror("Can not find this file\n");
		exit(0);
	}
	file_path = NULL;
	delete[] file_path;

	// to each file type, obj is different with other 2 type
	char* tmpStr = strtok(file_name,".");

	while (tmpStr != NULL)
	{
		strcpy(file_type, tmpStr);
		tmpStr = strtok(NULL,".");
	}
	print(file_type);
	print("start reading file \n");
	// .off file
	if (strcmp(file_type,"off") == 0)
	{
		Read_off(fp);
	}
	// .ply file
	else if (strcmp(file_type,"ply") == 0)
	{
		Read_ply(fp);
	}
	// .obj file
	else if (strcmp(file_type,"obj") == 0)
	{
		Read_obj(fp);
	}

	fclose(fp);

	print(" finished reading \n");
}

void Read_File(char* file_name, char* file_type)
{
	if (file_name == NULL)
	{
		perror("Please input filename or use \"all\" to use on all file\n");
		exit(0);
	}
	else 
	{
		Open_File(file_name, file_type);
	}
}
#endif

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------


