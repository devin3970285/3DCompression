//----------------------------------------------------------------------------
// Compress data
//
// build ellipsoid
// find saving data
// save ellipsoid
//
//----------------------------------------------------------------------------
#ifndef _PC2EO_FROM_DEFINE
#define _PC2EO_FROM_DEFINE

#include "Ellipsoid_Fit.h"

// #include "default_header.h"

// #include "Eigen/Dense" 
// using namespace Eigen;

VectorXd center;
VectorXd radii;
Matrix3d evecs;
VectorXd v;

extern MatrixXd* OccupancyList;
extern MatrixXd** EllipsoidList;
extern MatrixXd Left_PC;

MatrixXd occupancy_grid;
MatrixXd* EL;


void swap(double array[],double array_index[], int i, int j){
    if(i == j) return;

    double tmp=array[i];  
    array[i]=array[j];  
    array[j]=tmp; 

    tmp=array_index[i];  
    array_index[i]=array_index[j];  
    array_index[j]=tmp; 
}

int partition(double array[],double array_index[], int start, int end){
    // if(array == nullptr || length <= 0 || start < 0 || end >= length)
    //     throw new std::exception("Invalid Parameter");

    int index = rand() % (end - start + 1) + start; 
    swap(array, array_index , index, end);
    int small = start - 1;
    for(index = start; index < end; ++index){
        if(array[index] < array[end]){
            ++small;
            if(small != index)
                swap(array, array_index, small, index);
        }
    }

    ++small;
    swap(array, array_index, small, end);

    return small;
}


void quickSort(double array[],double array_index[], int start, int end){
    if(start == end) return;

    int index = partition(array, array_index, start, end);
    if(index > start)
        quickSort(array, array_index, start, index-1);
    if(index < end)
        quickSort(array, array_index, index+1, end);
}
  
void ShellSort(double array[],double array_index[], int n)  
{  
    for(int delta=n/2;delta>0;delta/=2)  
    {  
        for(int i=0;i<delta;i++)  
        {  
            for(int j=i+delta;j<n;j+=delta)  
            {  
                for(int k=j;k>0;k-=delta)  
                {  
                    if(array[k]<array[k-1])  
                        swap(array,array_index,k,k-1);  
                }  
            }  
        }  
    }  

}  

void PC2EO_index(MatrixXd points, int MethodOpt,int ErrorOpt, VectorXd Parameter)
{
	////可以优化其中的循环，加快速度，需进一步测试eigen
	////找出是哪一部分慢
	
	int MaxDistance = Parameter(0);    
	float r = Parameter(1);
	double e = Parameter(2);               
	int n = Parameter(3);

	if ( MethodOpt == SPHEREBUILDIN)
	{

	}

	if ( MethodOpt == SPHEREOUT || MethodOpt == ELLIPSOIDOUT )
	{
		int status = 0;
		if ( MethodOpt == SPHEREOUT )
		    status = ellipsoid_fit(points,"xyz");
		else
		    status = ellipsoid_fit(points,"");



		if (status == 0)
		{

			EL = new MatrixXd[4];
			EL[0] = v;
			EL[1] = center;
			EL[2] = radii;
			EL[3] = evecs;

			MatrixXd Removed_PC;   
		  	int length = points.rows();

		    VectorXd Dist = VectorXd::Zero(length);    
		 	VectorXd Dist_index = VectorXd::Zero(length);    

			double x,y,z;

			double abE;
			double total = 0;
		 	for (int i = 0; i < length ;i++)
		 	{
				x = points(i,0);
				y = points(i,1);
				z = points(i,2);
				abE = v(0)*x*x + v(1)*y*y + v(2)*z*z + 
					   2*v(3)*x*y + 2*v(4)*x*z + 2*v(5)*y*z + 
					   2*v(6) *x + 2*v(7)*y +2*v(8)*z +v(9);
			    Dist(i) = abE;


				// abE = (x - center(0))*(x - center(0))/radii(0)*radii(0)
				//  +  (y - center(1))*(y - center(1))/radii(1)*radii(1) 
				//  + (z - center(2))*(z - center(2))/radii(2)*radii(2) -1;

			 //    Dist(i) = fabs(abE);

			    Dist_index(i) = i;
		 	}

		    if ( ErrorOpt == PROPORTION )   
		    {   
		        quickSort(Dist.data(),Dist_index.data(),0, length-1);
		        //ShellSort(Dist.data(),Dist_index.data(),length);

		        int NR = round(r*length);

				Removed_PC = MatrixXd::Zero(NR,4);
				for(int i=0;i<NR;i++)
					Removed_PC.row(i) = points.row( Dist_index(i));

				Left_PC = MatrixXd::Zero(length - NR,4);
				for(int i=NR;i<length ;i++)
					Left_PC.row(i-NR) = points.row( Dist_index(i));
				    	
		    }
		    if (ErrorOpt == FIXEDNUMBER)   
		    {    
		        quickSort(Dist.data(),Dist_index.data(),0, length-1);

		        int NR = n;
		        if (NR > points.rows())
		        	NR = points.rows();

				Removed_PC = MatrixXd::Zero(NR,4);
				for(int i=0;i<NR;i++)
					Removed_PC.row(i) = points.row(Dist_index(i));

				Left_PC = MatrixXd::Zero(length - NR,4);
				for(int i=NR;i<length ;i++)
					Left_PC.row(i-NR) = points.row(Dist_index(i));
			
		    }
		    if (ErrorOpt == FIXEDERROR)   
		    {  
		    	Removed_PC = MatrixXd::Zero(length,4);
		    	Left_PC = MatrixXd::Zero(length,4);

		    	int index_r = 0;
		    	int index_l = 0;
		    	for(int i=0;i<length;i++)
		    	{
		    		if( Dist(i) < e)
		    		{
		    			Removed_PC.row(index_r) = points.row(i);
		    			index_r ++;
		    		}
		    		else 
		    		{
		    			Left_PC.row(index_l) = points.row(i);
		    			index_l ++;	
		    		}

		    	}

		    	Removed_PC.conservativeResize(index_r,4);
		    	Left_PC.conservativeResize(index_l,4);
		    }

		    Dist.resize(0);
		    Dist_index.resize(0); 


		    for(int i=0;i<Removed_PC.rows();i++)
		    {
		    	Removed_PC(i,0) = (Removed_PC(i,0) - center(0));
		    	Removed_PC(i,1) = (Removed_PC(i,1) - center(1));
		    	Removed_PC(i,2) = (Removed_PC(i,2) - center(2));
		    }

			Removed_PC.leftCols(3) = Removed_PC.leftCols(3)*evecs;


			occupancy_grid = MatrixXd::Zero(Removed_PC.rows(),3);
		    int pc_index;
		    double r;
			for(int i = 0; i< Removed_PC.rows();i++)
			{
				x = Removed_PC(i,0);
				y = Removed_PC(i,1);
				z = Removed_PC(i,2);
				pc_index = Removed_PC(i,3);


				r = sqrt(y*y+x*x);
				occupancy_grid(i,0) =  atan2(y,x); //theta

				occupancy_grid(i,1) =  atan(z/r); //phi

				// r = sqrt(x*x+y*y+z*z);
				// occupancy_grid(i,0) =  atan2(y,x); //theta
				// occupancy_grid(i,1) =  acos(z/r); //phi

				occupancy_grid(i,2) =  pc_index;
			}
		}
		else
		{
			print("too few points left\n");
			Left_PC = points;
			EL = new MatrixXd[4];
			EL[0] = VectorXd::Zero(10);
			EL[1] = VectorXd::Zero(3);
			EL[2] = VectorXd::Zero(3);
			EL[3] = MatrixXd::Zero(3,3);

			occupancy_grid = MatrixXd(0,0);
		}

	}

}

void PC2EO(MatrixXd points,int MethodOpt,int ErrorOpt, VectorXd Parameter, int N)
{

	print("do PC2EO computation\n");

	for(int i= 0; i<N;i++)
	{

		PC2EO_index(points,MethodOpt,ErrorOpt,Parameter);

		OccupancyList[i] = occupancy_grid;
		EllipsoidList[i] = EL;
     	points = Left_PC;

     	//print(points.rows());
     	//print(occupancy_grid);
	}
}

#endif