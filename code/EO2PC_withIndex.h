
//----------------------------------------------------------------------------
// Compress data
//
// build ellipsoid
// find saving data
// save ellipsoid
//
//----------------------------------------------------------------------------
#ifndef _EO2PCMESH_DEFINE
#define _EO2PCMESH_DEFINE

extern MatrixXd pointArray;
extern MatrixXd* OccupancyList;
extern MatrixXd** EllipsoidList;
extern MatrixXd Left_PC;


void EO2PC_withIndex(int MethodOpt, int N)
{
        print("start EO2PC \n");

        if ( MethodOpt == SPHEREBUILDIN)
        {

        }
        if ( MethodOpt == SPHEREOUT || MethodOpt == ELLIPSOIDOUT)
        {
              //这里 N 作弊了，要改

              MatrixXd* E;
              VectorXd theta, phi, pc_index;
              MatrixXd occupancy, xyz, xyz_t;
              MatrixXd x,y,z;
              int pArray_long;


              for( int i=0;i<N;i++)
              {
                   E = EllipsoidList[i];
                   occupancy = OccupancyList[i];

                   if(occupancy.rows() == 0 )
                     continue;

                   theta = occupancy.col(0); 
                   phi = occupancy.col(1);      
                   pc_index = occupancy.col(2); 

                   x = MatrixXd(1,occupancy.rows());
                   y = MatrixXd(1,occupancy.rows());
                   z = MatrixXd(1,occupancy.rows());

                   for (int j=0;j<occupancy.rows();j++)
                   {
                      x(j) = E[1](0) * cos(phi(j)) * cos(theta(j));
                      y(j) = E[1](1) * cos(phi(j)) * sin(theta(j));
                      z(j) = E[1](2) * sin(phi(j));

                      // x(j) = E[1](0) * sin(phi(j)) * cos(theta(j));
                      // y(j) = E[1](1) * sin(phi(j)) * sin(theta(j));
                      // z(j) = E[1](2) * cos(phi(j));
                   }

                  xyz.resize(3, occupancy.rows());
                  xyz << x , y , z;

                  // only ellipsoid, difference

                  xyz_t = xyz.adjoint();
                  xyz_t = xyz_t * E[3].inverse();

                  for (int j=0;j<xyz_t.rows();j++)
                  {          
                    xyz_t(j,0) = xyz_t(j,0) + E[0](0);
                    xyz_t(j,1) = xyz_t(j,1) + E[0](1);
                    xyz_t(j,2) = xyz_t(j,2) + E[0](2);
                  }

                  // xyz = E[3] * xyz;
                  // for (int j=0;j<xyz.cols();j++)
                  // {          
                  //   xyz(0,j) = xyz(0,j) + E[0](0);
                  //   xyz(1,j) = xyz(1,j) + E[0](1);
                  //   xyz(2,j) = xyz(2,j) + E[0](2);
                  // }
                  // xyz_t = xyz.transpose();

                  xyz_t.conservativeResize(xyz_t.rows(),4);
                  xyz_t.col(3) = pc_index;


                  pArray_long = pointArray.rows();
                  pointArray.conservativeResize(pArray_long + xyz_t.rows(),4);
                  pointArray.block(pArray_long,0,xyz_t.rows(),4) = xyz_t;
              }
        }

        print("start sorting points array\n");
       //Append leftover points
        int pc_long = pointArray.rows();
        pointArray.conservativeResize(pc_long + Left_PC.rows(),4);
       // points << xyz;
        pointArray.block(pc_long,0,Left_PC.rows(),4) = Left_PC;

        //sort points by index (x,y,z,index)
        MatrixXd sort_point = MatrixXd::Zero(pointArray.rows(),4);
        for (int i=0;i< pointArray.rows();i++)
        {
          sort_point.row( pointArray(i,3) ) = pointArray.row(i);
        }

        pointArray = sort_point;

        print("end EO2PC \n");

}

#endif
