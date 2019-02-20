
//----------------------------------------------------------------------------
// _Matrix
//
//
//----------------------------------------------------------------------------

#include "_Matrix.h"


_Matrix::_Matrix(int row,int col)
{
	m = row;
	n = col;
}

void _Matrix::set_m(int row)
{
	m = row;
}


void _Matrix::set_n(int col)
{
	n = col;
}

 
void _Matrix::init_Matrix()
{
	arr = new float[m * n];
}

void _Matrix::init_Matrix(int num)
{
	m = num;
	n = num;
	init_Matrix();
	for (int i=0;i<m;i++)
	{
		for (int j=0;j<m;j++)
		{
			if (i == j)
				write(i,j,1);
			else
				write(i,j,0);
		}
	}
}
 
void _Matrix::free_Matrix()
{
	delete []arr;
}

 
float _Matrix::read(int i,int j)
{
	if (i >= m || j >= n)
	{
		return -9999;
	}

	return *(arr + i * n + j);
}



int _Matrix::write(int i,int j,float val)
{
	if (i >= m || j >= n)
	{
		return -1;
	}

	*(arr + i * n + j) = val;
	return 1;
}


int add(_Matrix *A,_Matrix *B,_Matrix *C)
{

	int i = 0;
	int j = 0;

	if (A->m != B->m || A->n != B->n || A->m != C->m || A->n != C->n)
	{
		return -1;
	}


	for (i = 0;i < C->m;i++)
	{
		for (j = 0;j < C->n;j++)
		{
			C->write(i,j,A->read(i,j) + B->read(i,j));
		}
	}

	return 1;
}


int subtract(_Matrix *A,_Matrix *B,_Matrix *C)
{
	int i = 0;
	int j = 0;

	if (A->m != B->m || A->n != B->n || A->m != C->m || A->n != C->n)
	{
		return -1;
	}

	for (i = 0;i < C->m;i++)
	{
		for (j = 0;j < C->n;j++)
		{
			C->write(i,j,A->read(i,j) - B->read(i,j));
		}
	}

	return 1;
}


int multiply(_Matrix *A,_Matrix *B,_Matrix *C)
{

	int i = 0;
	int j = 0;
	int k = 0;
	float temp = 0;

	if (A->m != C->m || B->n != C->n || A->n != B->m)
	{
		return -1;
	}

	for (i = 0;i < C->m;i++)
	{
		for (j = 0;j < C->n;j++)
		{
			temp = 0;
			for (k = 0;k < A->n;k++)
			{
				temp += A->read(i,k) * B->read(k,j);
			}
			C->write(i,j,temp);
		}
	}

	return 1;
}


int transpos(_Matrix *A,_Matrix *B)
{
	int i = 0;
	int j = 0;

	if (A->m != B->n || A->n != B->m)
	{
		return -1;
	}

	for (i = 0;i < B->m;i++)
	{
		for (j = 0;j < B->n;j++)
		{
			B->write(i,j,A->read(j,i));
		}
	}

	return 1;
}

int inverse(_Matrix *A,_Matrix *B)
{

	int i = 0;
	int j = 0;
	int k = 0;

	_Matrix m(A->m,2 * A->m);

	float temp = 0;
	float b = 0;

	
	if (A->m != A->n || B->m != B->n || A->m != B->m)
	{
		return -1;
	}

	m.init_Matrix();

	for (i = 0;i < m.m;i++)
	{
		for (j = 0;j < m.n;j++)
		{
			if (j <= A->n - 1)
			{
				m.write(i,j,A->read(i,j));
			}
			else
			{
				if (i == j - A->n)
				{
					m.write(i,j,1);
				}
				else
				{
					m.write(i,j,0);
				}
			}
		}
	}

	

	for (k = 0;k < m.m - 1;k++)
	{
		if (m.read(k,k) == 0)
		{
			for (i = k + 1;i < m.m;i++)
			{
				if (m.read(i,k) != 0)
				{
					break;
				}
			}

			if (i >= m.m)
			{
				return -1;
			}
			else
			{
				for (j = 0;j < m.n;j++)
				{

					temp = m.read(k,j);

					m.write(k,j,m.read(k + 1,j));

					m.write(k + 1,j,temp);

				}
			}
		}

		
		for (i = k + 1;i < m.m;i++)
		{
			b = m.read(i,k) / m.read(k,k);

			for (j = 0;j < m.n;j++)
			{
				temp = m.read(i,j) - b * m.read(k,j);

				m.write(i,j,temp);
			}
		}
	}


	for (k = m.m - 1;k > 0;k--)
	{

		if (m.read(k,k) == 0)
		{
			for (i = k + 1;i < m.m;i++)
			{
				if (m.read(i,k) != 0)
				{
					break;
				}
			}

			if (i >= m.m)
			{
				return -1;
			}
			else
			{
				for (j = 0;j < m.n;j++)
				{
					temp = m.read(k,j);

					m.write(k,j,m.read(k + 1,j));

					m.write(k + 1,j,temp);
				}
			}
		}

		for (i = k - 1;i >= 0;i--)
		{
			b = m.read(i,k) / m.read(k,k);

			for (j = 0;j < m.n;j++)
			{
				temp = m.read(i,j) - b * m.read(k,j);

				m.write(i,j,temp);
			}
		}
	}


	for (i = 0;i < m.m;i++)
	{
		if (m.read(i,i) != 1)
		{
			b = 1 / m.read(i,i);

			for (j = 0;j < m.n;j++)
			{
				temp = m.read(i,j) * b;

				m.write(i,j,temp);
			}
		}
	}


	for (i = 0;i < B->m;i++)
	{
		for (j = 0;j < B->m;j++)
		{

			B->write(i,j,m.read(i,j + m.m));

		}

	}

	m.free_Matrix();

	return 1;
}


int split_col(_Matrix* A, int col, _Matrix* B)
{
	if (col > A->n)
		return 0;

	for (int i = 0;i < A->m;i++)
	{
		B->write(i,0,A->read(i,col));	
	}

	return 1;
}

int split_row(_Matrix* A, int row, _Matrix* B)
{
	if (row > A->m)
		return 0;

	for (int i = 0;i < A->n;i++)
	{
		B->write(0,i,A->read(row,i));	
	}

	return 1;
}

int reverSign(_Matrix* A)
{
	for (int i = 0;i < A->m;i++)
	{
		for (int j = 0;j < A->n;j++)
		{
			A->write(i,j,-A->read(i,j));	
		}

	}
	return 1;
}

void printf_matrix(_Matrix *A)
{
	int i = 0;
	int j = 0;
	int m = 0;
	int n = 0;

	m = A->m;
	n = A->n;

	for (i = 0;i < m;i++)
	{
		for (j = 0;j < n;j++)
		{
			printf("%f  ",A->read(i,j));
		}
		printf("\n");
	}

}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------


