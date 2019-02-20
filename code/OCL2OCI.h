//----------------------------------------------------------------------------
// Compress data
//
// build ellipsoid
// find saving data
// save ellipsoid
//
//----------------------------------------------------------------------------
#ifndef _OCL2OCI_FROM_DEFINE
#define _OCL2OCI_FROM_DEFINE


extern MatrixXd* OccupancyList;
extern int rs;
extern int ls;
extern MatrixXd OccupancyImage;
extern MatrixMapVd EllipsoidMap;
extern MatrixMapVd EllipsoidMapIndex;

void OCL2OCI(int r, int N)
{
	// length of occupancyList = N;

	print("do OCL2OCI computation \n");


	OccupancyImage = MatrixXd::Zero(rs, ls);
	EllipsoidMap =  MatrixMapVd::Zero(rs, ls);
	EllipsoidMapIndex =  MatrixMapVd::Zero(rs, ls);

	MatrixXd oc_index;
	MatrixXd occupancyradius;
	MatrixXd occupancydegree;
	for (int i=0; i<N; i++ )
	{
		occupancyradius = OccupancyList[i];
		if(occupancyradius.rows() == 0)
			continue;

		oc_index = occupancyradius.col(2);
		occupancyradius = occupancyradius.leftCols(2);

		occupancydegree = MatrixXd(occupancyradius.rows(),2);
		for(int j=0; j<occupancyradius.rows();j++)
		{

			//361*r 181*r
			occupancydegree(j,0) = round(occupancyradius(j,0) * 180 * r / M_PI) + 180*r;
			occupancydegree(j,1) = round(occupancyradius(j,1) * 180 * r / M_PI) + 90*r;

			// occupancydegree(j,0) = round(occupancyradius(j,0) * 180 / M_PI) * r + 180*r;
			// occupancydegree(j,1) = round(occupancyradius(j,1) * 180 / M_PI) * r + 90*r;
		}

		int vectorlen = 0;
		for (int j=0; j< occupancydegree.rows();j++)
		{

			// record how many in each list
			OccupancyImage(occupancydegree(j,0),occupancydegree(j,1)) = OccupancyImage(occupancydegree(j,0),occupancydegree(j,1)) + 1;

			// index inside of N
			vectorlen = EllipsoidMap(occupancydegree(j,0),occupancydegree(j,1)).rows();
			EllipsoidMap(occupancydegree(j,0),occupancydegree(j,1)).conservativeResize(vectorlen+1);
			EllipsoidMap(occupancydegree(j,0),occupancydegree(j,1))(vectorlen) = i;
			//EllipsoidMap[(int)occupancydegree(j,0)][(int)occupancydegree(j,1)] = (int)(i);

			// point index
			vectorlen = EllipsoidMapIndex(occupancydegree(j,0),occupancydegree(j,1)).rows();
			EllipsoidMapIndex(occupancydegree(j,0),occupancydegree(j,1)).conservativeResize(vectorlen+1);
			EllipsoidMapIndex(occupancydegree(j,0),occupancydegree(j,1))(vectorlen) = oc_index(j);

			//EllipsoidMapIndex[(int)occupancydegree(j,0)][(int)occupancydegree(j,1)] = (int)(oc_index(j));
		}
	}
}

#endif