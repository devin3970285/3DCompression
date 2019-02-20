//----------------------------------------------------------------------------
// Compress data
//
// build ellipsoid
// find saving data
// save ellipsoid
//
//----------------------------------------------------------------------------
#ifndef _WEOUINT8MESH_DEFINE
#define _WEOUINT8MESH_DEFINE

#include <fstream>
using namespace std;

extern int rs;
extern int ls;
extern MatrixXd OccupancyImage;
extern MatrixXd** EllipsoidList;
extern MatrixMapVd EllipsoidMap;
extern MatrixMapVd EllipsoidMapIndex;
extern MatrixXd Left_PC;
extern MatrixXd faceArray;

void writeEOUint8_withMesh(char* file_name, int MethodOpt, int r, int N)
{

	//change file name
	//strtok(file_name,".");
	strcat(file_name,MIDDLE_FILE_NAME);


	char* file_path = new char[strlen(FILE_COMPRESS_PATH) + strlen(file_name)];
	strcpy(file_path, FILE_COMPRESS_PATH);
	strcat(file_path, file_name);	

    ofstream out;
    out.open(file_path, ios::out|ios::binary);
    delete[] file_path;

    if (out.is_open()) 
    {
	    // for real
		if (MethodOpt == SPHEREBUILDIN || MethodOpt == SPHEREOUT || MethodOpt == ELLIPSOIDOUT)
		{			
			printf("write data into file, using same %s.cnm\n\n",file_name);
			//head
			out << MethodOpt <<" "<< r <<" "<< N << " "<< Left_PC.rows() <<"\n";

			MatrixXd* Ellispoid;
			for (int i=0;i<N;i++)
			{
				Ellispoid = EllipsoidList[i];

				//center
				out << Ellispoid[1](0) <<" "<< Ellispoid[1](1) <<" "<< Ellispoid[1](2) <<"\n";

				//radii
				out << Ellispoid[2](0) <<" "<< Ellispoid[2](1) <<" "<< Ellispoid[2](2) <<"\n";

				//v
				for(int j=0;j<10;j++)
				{
					out << Ellispoid[0](j) <<" ";
				}
				out <<"\n";
	
				// //evecs
				for(int j=0;j<3;j++)
				{
					for(int k=0;k<3;k++)
						out << Ellispoid[3](j,k) <<" ";
					out <<"\n";
				}
			}
			// occupancy image;
			int count = 0;
			for( int i=0;i<rs;i++)
				for( int j=0;j<ls;j++)
					if (OccupancyImage(i,j) !=0 )					
						count ++;
				
			out << count << '\n';

			for( int i=0;i<rs;i++)
				for( int j=0;j<ls;j++)
					if (OccupancyImage(i,j) !=0 )
						out << i << " " << j << " " << OccupancyImage(i,j) << '\n';

			printf("\nwrite %d non-zero data in OccupancyImage\n\n",count);

			// elliosidmap and indexmap
			for( int i=0;i<rs;i++)
				for( int j=0;j<ls;j++)
					if (OccupancyImage(i,j) !=0 )
					{
						for(int k=0; k<OccupancyImage(i,j); k++)
						{
							//__int8
							out << (int)(EllipsoidMap(i,j)(k)) << " ";
							out << (int)(EllipsoidMapIndex(i,j)(k)) << "\n";

						}
					}

			//left points
			for ( int i=0;i<Left_PC.rows();i++)
			{
				for (int j=0;j<4;j++)
					out << Left_PC(i,j) << " ";
				out << "\n";
			}
		
			printf("\nwrite %d Left_PC \n\n",Left_PC.rows());

			//face
			int len;
			for ( int i=0;i<faceArray.rows();i++)
			{
				len = faceArray(i,0);
				for( int j=0;j<=len;j++)
				{
					out << faceArray(i,j) << " ";
				}
				out << "\n";
			}

			printf("\nwrite %d face\n\n",faceArray.rows());

		}
		out.close();
    }	

    print("compress done\n");
}
#endif