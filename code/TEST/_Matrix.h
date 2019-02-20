
//----------------------------------------------------------------------------
// _Matrix
//
//
//----------------------------------------------------------------------------
#ifndef _Matrix_FROM_DEFINE
#define _Matrix_FROM_DEFINE

class _Matrix
{
	private:

	public:

		int m;
		int n;
		float *arr;

		_Matrix(int row = 0,int col = 0);

		void set_m(int row);

		void set_n(int col);

		void init_Matrix();

		void free_Matrix();

		float read(int i,int j);

		int write(int i,int j,float val);

		//eye 3
		// 1 0 0 
		// 0 1 0 
		// 0 0 1
		void init_Matrix(int num);
};

int reverSign(_Matrix* A);

int split_col(_Matrix* A, int col, _Matrix* B);
int split_row(_Matrix* A, int row, _Matrix* B);

int add(_Matrix *A,_Matrix *B,_Matrix *C);

int subtract(_Matrix *A,_Matrix *B,_Matrix *C);

int multiply(_Matrix *A,_Matrix *B,_Matrix *C);

int transpos(_Matrix *A,_Matrix *B);

int inverse(_Matrix *A,_Matrix *B);

void printf_matrix(_Matrix *A);

#endif
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------


