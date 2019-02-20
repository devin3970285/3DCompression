//----------------------------------------------------------------------------
// Compress data
//
// build ellipsoid
// find saving data
// save ellipsoid
//
//----------------------------------------------------------------------------
#ifndef _ELLIPSOID_FROM_DEFINE
#define _ELLIPSOID_FROM_DEFINE


//#include "Eigen/Eigenvalues"

//#include "default_header.h"

//#include "Eigen/Dense" 
//using namespace Eigen;

extern VectorXd center;
extern VectorXd radii;
extern Matrix3d evecs;
extern VectorXd v;

//center, radii, evecs, v
int ellipsoid_fit(MatrixXd points, const char* flag)
{

	int length = points.rows();
	VectorXd x(length);
	VectorXd y(length);
	VectorXd z(length);

	x = points.col(0);
	y = points.col(1);
	z = points.col(2);

	if (length < 9 &&  strcmp( flag, "" ) ==0 )
	{
		perror("must have more than 9 points to do ellipsoid fit\n");
		return 1;
	}
	if (length < 4 &&  strcmp( flag, "xyz" ) ==0 )
	{
		perror("must have more than 4 points to do sphere fit\n");
		return 1;
	}

	MatrixXd D;
	if (strcmp( flag, "" ) == 0)
	{
		D = MatrixXd::Zero(length,9);

		D.col(0) = x.array()*x.array() + y.array()*y.array() - 2 * z.array()*z.array();
		D.col(1) = x.array()*x.array() + z.array()*z.array() - 2 * y.array()*y.array();
		// D.col(0) = x.array()*x.array() + y.array()*y.array() + z.array()*z.array();
		// D.col(1) = x.array()*x.array() + z.array()*z.array() + y.array()*y.array();

		D.col(2) = 2 * x.array() * y.array();
		D.col(3) = 2 * x.array() * z.array();
		D.col(4) = 2 * y.array() * z.array();
		D.col(5) = 2 * x;
		D.col(6) = 2 * y;
		D.col(7) = 2 * z;
		D.col(8).fill(1);

	}
	else if (strcmp( flag, "xyz" ) ==0)
	{
		D = MatrixXd::Zero(length,4);
		D.col(0) = 2 * x;
		D.col(1) = 2 * y;
		D.col(2) = 2 * z;
		D.col(3).fill(1);
	}
	///////
	/// data is too big, could have some difference;
	//////

	// solve the normal system of equations	
	VectorXd d2(length);
	d2 = x.array() * x.array() + y.array()*y.array() + z.array()*z.array();
	//the RHS of the llsq problem (y's)

	VectorXd u;
	// //u = ( D' * D ) \ ( D' * d2 );  % solution to the normal equations
	u = (D.adjoint() * D).inverse() * (D.adjoint() * d2);
	//u = (D.transpose() * D).fullPivHouseholderQr().solve(D.transpose() * d2);

	v = VectorXd::Zero(10);
	if (strcmp( flag, "" ) == 0)
	{
        v(0) = u(0) +     u(1) - 1;
    	v(1) = u(0) - 2 * u(1) - 1;
    	v(2) = u(1) - 2 * u(0) - 1;

    	v.segment(3,7) = u.segment(2,7);
    	// for( int i = 0; i< 7;i++)
    	// 	v(3+i) = u(2+i);
 	}
	else if (strcmp( flag, "xyz" ) ==0)
	{
		v(0) = -1;
		v(1) = -1;
		v(2) = -1;
		v(3) = 0;
		v(4) = 0;
		v(5) = 0;
		v(6) = u(0);
		v(7) = u(1);
		v(8) = u(2);
		v(9) = u(3);
	}

	MatrixXd A = MatrixXd::Zero(4,4);
	A << v(0) , v(3) , v(4) , v(6) ,
		 v(3) , v(1) , v(5) , v(7) ,
		 v(4) , v(5) , v(2) , v(8) ,
		 v(6) , v(7) , v(8) , v(9);

	center = (-A.block(0,0,3,3)).inverse() * v.segment(6,3);
	//center = (-A.block(0,0,3,3)).fullPivHouseholderQr().solve(v.segment(6,3));

    MatrixXd T = MatrixXd::Identity(4,4);
    T.row(3).segment(0,3) = center.adjoint();
    // VectorXd center_tran = center.transpose();
    // for(int i=0; i< 3; i++)	
    // 	T( 3, i ) = center_tran(i);

   	MatrixXd R = T * A * T.adjoint();

   	Matrix3d egien_data = R.block(0,0,3,3) / (-R(3,3));
	EigenSolver<Matrix3d> es(egien_data);

    Matrix3d evals = es.pseudoEigenvalueMatrix();
    evecs = es.pseudoEigenvectors();


    //print(evecs * evals * evecs.transpose());

    Matrix3d abs_evals = evals.array().abs();
    MatrixXd diag_evals = abs_evals.diagonal();
    
    radii = diag_evals.array().inverse().array().sqrt();		
	VectorXd sgns = evals.diagonal().array().sign();
	radii = radii.array() * sgns.array();

	double v_end = v(v.rows()-1);
	int sign = (int)(v_end/fabs(v_end));

	if (fabs(v_end) > 1e-6)
	    v = (-v) / v_end; 
	else
	    v = (-sign) * v;

	return 0;
}
#endif