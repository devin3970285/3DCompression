
//----------------------------------------------------------------------------
// Save different kinds of file.
//
// .off
// .ply
// .obj
//
//----------------------------------------------------------------------------
#ifndef _SAVEFILE_DEFINE
#define _SAVEFILE_DEFINE

//#include <cstring>
// #include "default_header.h"
// #include "Eigen/Dense" 
// using namespace Eigen;

extern MatrixXd pointArray;
extern MatrixXd faceArray;


void Save_off(FILE *fp)
{
	int num_vertex = 0, num_face = 0, num_edge = 0;

	// char* buffer = new char[strlen("OFF")+(num_vertex+num_face+num_edge)*3*8];
	// char* temp = new char[3*8];

	// strcpy(buffer, "OFF\n");
	// sprintf(temp,"%d %d %d\n",num_vertex, num_face, num_edge);
	// strcat(buffer,temp);

	// for (int i=0; i<num_vertex; i++)
	// {
	// 	sprintf(temp,"%lf %lf %lf\n",pointArray[i].x, pointArray[i].y, pointArray[i].z);
	// 	strcat(buffer,temp);
	// }

	// for (int i=0; i<num_face; i++)
	// {
	// 	if (faceArray[i].num_points == 4)
	// 		sprintf(temp,"%d %ld %ld %ld %ld\n",faceArray[i].num_points,faceArray[i].index_1, faceArray[i].index_2, faceArray[i].index_3, faceArray[i].index_n);
	// 	else
	// 		sprintf(temp,"%d %ld %ld %ld\n",faceArray[i].num_points,faceArray[i].index_1, faceArray[i].index_2, faceArray[i].index_3);

	// 	strcat(buffer,temp);
	// }
	// // used in c language...
	// fputs(buffer,fp);

	// delete[] buffer;
	// delete[] temp;
}
void Save_ply(FILE *fp)
{
	int num_vertex = 0, num_face = 0, num_edge = 0;

	
}
void Save_obj(FILE *fp)
{
	
}

void Save_File(char* file_name)
{
	if (file_name == NULL)
	{
		perror("Please input filename \n");
		exit(0);
	}

	// open this file.
	FILE *fp;

	char* file_path = new char[strlen(FILE_OUTPUT_PATH) + strlen(file_name)];
	strcpy(file_path, FILE_OUTPUT_PATH);
	strcat(file_path, file_name);	

    if ((fp = fopen(file_path,"w")) == NULL)
	{
		perror("Can not find this file\n");
		exit(0);
	}
	delete[] file_path;

	// to each file type, obj is different with other 2 type
	char* tmpStr = strtok(file_name,".");
	char* file_type = new char[10];

	while (tmpStr != NULL)
	{
		strcpy(file_type, tmpStr);
		tmpStr = strtok(NULL,".");
	}
	// .off file
	if (strcmp(file_type,"off") == 0)
	{
		Save_off(fp);
	}
	// .ply file
	else if (strcmp(file_type,"ply") == 0)
	{
		Save_ply(fp);
	}
	// .obj file
	else if (strcmp(file_type,"obj") == 0)
	{
		Save_obj(fp);
	}

	delete[] file_type;

	fclose(fp);
}

#endif
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------


