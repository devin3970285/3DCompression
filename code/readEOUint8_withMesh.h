//----------------------------------------------------------------------------
// Compress data
//
// build ellipsoid
// find saving data
// save ellipsoid
//
//----------------------------------------------------------------------------
#ifndef _REOUINT8MESH_DEFINE
#define _REOUINT8MESH_DEFINE


extern int rs;
extern int ls;

extern MatrixXd* OccupancyList;
extern MatrixXd** EllipsoidList;

extern MatrixXd Left_PC;

int readEOUint8_withMesh(char* file_name)
{
	// open this file.
	FILE *fp;

	char* file_path = new char[strlen(FILE_COMPRESS_PATH) + strlen(file_name)];
	strcpy(file_path, FILE_COMPRESS_PATH);
	strcat(file_path, file_name);	

    if ((fp = fopen(file_path,"r")) == NULL)
	{
		perror("Can not find this file\n");
		exit(0);
	}
	file_path = NULL;
	delete[] file_path;

	print("start reading compressed file \n");

	// Read Header:
	int Method,r,N,LeftPC_Length;
	fscanf(fp, "%d %d %d %d", &Method, &r, &N, &LeftPC_Length);


	// Initialize for outputs. 
	// Calcualte Resolution: resolution of the occupancy map.
	rs = 361*r;
	ls = 181*r;

	OccupancyList = new MatrixXd[N];
	EllipsoidList = new MatrixXd*[N];

	if ( Method == SPHEREBUILDIN || Method == SPHEREOUT || Method == ELLIPSOIDOUT)
	{
		float x,y,z;
		for(int i=0;i<N;i++)
		{
			EllipsoidList[i] = new MatrixXd[4];
			////// center
			////// radii
			//////  v
			////// evecs

			//center
			VectorXd center = VectorXd(3);
			fscanf(fp, "%f %f %f", &x, &y, &z);
			center(0) = x;
			center(1) = y;
			center(2) = z;
			EllipsoidList[i][0] = center;

			// radii
			VectorXd radii = VectorXd(3);
			fscanf(fp, "%f %f %f", &x, &y, &z);
			radii(0) = x;
			radii(1) = y;
			radii(2) = z;
			EllipsoidList[i][1] = radii;

			//v
			VectorXd v = VectorXd(10);
			for (int j=0;j<10;j++)
			{
				fscanf(fp, "%f", &x);
				v(j) = x;
			}
			EllipsoidList[i][2] = v;

			//evecs
			MatrixXd evecs = MatrixXd(3,3);
			for (int j=0;j<3;j++)
				for (int k=0;k<3;k++)
				{
					fscanf(fp, "%f", &x);
					evecs(j,k) = x;
				}

			EllipsoidList[i][3] = evecs;
		}

		// read image size
		int count;
		fscanf(fp, "%d", &count);

		// read occupancy image
		MatrixXd OccupancyImage = MatrixXd::Zero(rs,ls);
		// remove start sign
		for (int i=0;i<count;i++)
		{
			fscanf(fp, "%f %f %f", &x, &y, &z);
			OccupancyImage(x,y) = z;
		}
		printf("\nalready read %d non-zero data from OccupancyImage\n\n", count);


		// read ellipsoid map and index
		int imageCount;
		double theta,phi;

		VectorXd e_map, index_map;

		int dynamic_List, e,g;  
		for (int i=0;i<rs;i++)
		{
			for (int j=0;j<ls;j++)
			{
				imageCount = OccupancyImage(i,j);
				if(imageCount != 0)
				{
					//calculate
					theta = (i-180*r)/r *M_PI/180;
           			phi = (j-90*r)/r *M_PI/180;
           			if (theta > M_PI)
           				theta = M_PI - (1e-6); 
           			if (theta < -M_PI)
           				theta = -M_PI + (1e-6);
           			if (phi > M_PI/2)
           				phi = M_PI/2 - (1e-6); 
           			 if (phi < -M_PI/2)
           				phi = -M_PI/2 + (1e-6); 

           			// read ellipsoid map and index map
					for(int k=0;k<imageCount;k++)
					{
						//fgetc(fp);
						fscanf(fp, "%d %d", &e, &g);

						dynamic_List = OccupancyList[e].rows();
						OccupancyList[e].conservativeResize(dynamic_List+1,3);
				        OccupancyList[e](dynamic_List,0) = theta;
				        OccupancyList[e](dynamic_List,1) = phi;
				        OccupancyList[e](dynamic_List,2) = g;
					}

				}
			}
		}

		// read left points
		Left_PC = MatrixXd(LeftPC_Length,4);
		for (int i=0;i<LeftPC_Length;i++)
		{
			fscanf(fp, "%f %f %f %d", &x, &y, &z, &e);
			Left_PC(i,0) = x;
			Left_PC(i,1) = y;
			Left_PC(i,2) = z;
			Left_PC(i,3) = e;

		}
		printf("\nalready read %d non-zero Left_PC\n\n", LeftPC_Length);


	}
    //print("start uncompress\n");

    return Method;

}
#endif