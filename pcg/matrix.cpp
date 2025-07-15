#include <stdio.h>
#include <math.h>
#include <stdlib.h> 
#include <string.h>
#include "double.h"
#include "matrix.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////* print functions */////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Prints a vector to a text file
 * \param DOUBLE* V: pointer to a list of DOUBLE values
 * \param int n: number of entries of the list
 * \param char* Name: absolute filename of the output
 *
 * This method prints a list of DOUBLE values to a file, where
 * all elements are line-separated.
 */
FOR_DEBUG void print_vector(DOUBLE* V, int n, const char* Name)
{
	int i;
	if (V != NULL)
	{
		FILE* file = fopen(Name, "w");
		for (i = 0; i < n; i++)
		{
			fprintf(file, "%.10e\n", (double)V[i]);
		}
		fclose(file);
	}
}

/**
 * \brief Prints an integer list to a text file
 * \param int* V: pointer to a list of integer values
 * \param int n: number of entries of the list
 * \param char* Name: absolute filename of the output
 *
 * This method prints a list of integer values to a file, where
 * all elements are line-separated.
 */
FOR_DEBUG void print_list(int* V, int n, const char* Name)
{
	int i;
	if (V != NULL)
	{
		FILE* file = fopen(Name, "w");
		for (i = 0; i < n; i++)
		{
			fprintf(file, "%d\n", (double)V[i]);
		}
		fclose(file);
	}
}

/**
 * \brief Prints a CSR matrix to a text file
 * \param CSR_matrix* A: pointer to a CSR matrix
 * \param char* Name: absolute filename of the output
 *
 * This method prints a matrix in "Compressed Sparse Row"-format (CSR) to a textfile.
 * Each row of the matrix A is represented by a line in the text file. The elements
 * are tab-separated. Each element in a row <i> is represented by a pair (<column index j>,<A_{i,j}>)
 */
FOR_DEBUG void print_CSR(CSR_matrix* A, const char* Name)
{
	if (A != NULL)
	{
		int i, j, J;
		DOUBLE a;

		FILE* file = fopen(Name, "w");
		if (file == NULL) return;

		for (i = 0; i < A->rows; i++)
		{
			for (j = A->row_start[i]; j < A->row_start[i + 1]; j++)
			{
				J = A->indices[j];
				a = A->elements[j];
				fprintf(file, "(%d,%.6f)\t", J, (double)a);
			}
			fprintf(file, "\n");
		}
		fclose(file);
	}
}

/**
 * \brief Prints a CSC matrix to a text file
 * \param CSC_matrix* A: pointer to a CSC matrix
 * \param char* Name: absolute filename of the output
 *
 * This method prints a matrix in "Compressed Sparse Column"-format (CSC) to a textfile.
 * Each column of the matrix A is represented by a line in the text file. The elements
 * are tab-separated. Each element in a column <i> is represented by a pair (<row index j>,<A_{j,i}>)
 */
FOR_DEBUG void print_CSC(CSC_matrix* A, const char* Name)
{
	if (A != NULL)
	{
		int i, j, J;
		DOUBLE a;

		FILE* file = fopen(Name, "w");
		if (file == NULL) return;

		for (i = 0; i < A->cols; i++)
		{
			for (j = A->col_start[i]; j < A->col_start[i + 1]; j++)
			{
				J = A->indices[j];
				a = A->elements[j];
				fprintf(file, "(%d,%.4f)\t", J, (double)a);
			}
			fprintf(file, "\n");
		}
		fclose(file);
	}
}

/**
 * \brief Prints a CSR matrix to the Matrix Market format
 * \param CSR_matrix* A: pointer to a CSR matrix
 * \param char* Name: absolute filename of the output
 *
 * This method prints a matrix in "Compressed Sparse Row"-format (CSR) to a textfile in a "Matrix Market exchange format".
 * see http://math.nist.gov/MatrixMarket/formats.html for the structure details. The file suffix is ".mtx".
 */
FOR_DEBUG void print_CSR_matrix_market(CSR_matrix* A, const char* Name)
{
	int i, j;

	if (A != NULL)
	{
		int rows = A->rows;
		int cols = 0;
		int entries = A->row_start[rows];

		FILE* file = fopen(Name, "w");
		for (i = 0; i < rows; i++)
		{
			j = A->row_start[i + 1] - 1;
			if (A->indices[j] > cols) cols = A->indices[j];
		}

		// header
		fprintf(file, "%d %d %d\n", rows, cols + 1, entries);

		// data
		for (i = 0; i < rows; i++)
		{
			for (j = A->row_start[i]; j < A->row_start[i + 1]; j++)
			{
				fprintf(file, "%d %d %g\n", i + 1, A->indices[j] + 1, (double)A->elements[j]);
			}
		}
		fclose(file);
	}
}

/**
 * \brief Prints a CSC matrix to the Matrix Market format
 * \param CSC_matrix* A: pointer to a CSC matrix
 * \param char* Name: absolute filename of the output
 *
 * This method prints a matrix in "Compressed Sparse Column"-format (CSC) to a textfile in a "Matrix Market exchange format".
 * see http://math.nist.gov/MatrixMarket/formats.html for the structure details. The file suffix is ".mtx".
 */
FOR_DEBUG void print_CSC_matrix_market(CSC_matrix* A, const char* Name)
{
	char Fullname[512];
	int i, j;

	if (A != NULL)
	{
		int rows = 0;
		int cols = A->cols;
		int entries = A->col_start[cols];

		sprintf(Fullname, "%s.mtx", Name);
		FILE* file = fopen(Fullname, "w");
		for (i = 0; i < cols; i++)
		{
			j = A->col_start[i + 1] - 1;
			if (A->indices[j] > rows) rows = A->indices[j];
		}

		// header
		fprintf(file, "%d %d %d\n", rows + 1, cols, entries);

		// data
		for (i = 0; i < cols; i++)
		{
			for (j = A->col_start[i]; j < A->col_start[i + 1]; j++)
			{
				fprintf(file, "%d %d %g\n", A->indices[j] + 1, i + 1, (double)A->elements[j]);
			}
		}
		fclose(file);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////* vector functions *//////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
* \brief Allocates a list of doubles initialized by zero
* \param DOUBLE* V: pointer to the previously allocated list
* \param int size: dimension of vector
* \return void : none
*/
void zero_vector(DOUBLE* V, int size)
{
	for (int i = 0; i < size; i++)
	{
		V[i] = 0;
	}
}

/**
 * \brief Clears a vector by setting all elements to zero
 * \param DOUBLE* V: Pointer to vector to be cleared)
 * \param int n: dimension of vector
 */
void clear_vector(DOUBLE* V, int n)
{
	memset((void*)V, 0, n * sizeof(DOUBLE));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////* matrix functions *//////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Frees all memory of a CSC-matrix struct
 * \param CSC_matrix** A: pointer to the pointer where the matrix is stored
 */
void free_CSC_matrix(CSC_matrix** const A)
{
	if (*A != NULL)
	{
		free(*A);
	}
}

/**
 * \brief Frees all memory of a CSR-matrix struct
 * \param CSR_matrix** A: pointer to the pointer where the matrix is stored
 */
void free_CSR_matrix(CSR_matrix** const A)
{
	if (*A != NULL)
	{
		free(*A);
	}
}

/**
 * \brief Scales a CSR matrix by a factor
 * \param CSR_matrix* A: pointer the the CSR-matrix to be scaled
 * \param DOUBLE factor: scaling factor
 */
void CSR_scale_matrix(CSR_matrix* A, DOUBLE factor)
{
	int i;

	int N = A->row_start[A->rows];
	for (i = 0; i < N; i++)
	{
		A->elements[i] *= factor;
	}
}

/**
 * \brief Helper function for "matrix_product_to_CSR"
 * \param CSR_matrix* A: first factor of matrix product
 * \param CSC_matrix* b: second factor of matrix product
 * \param int i: row index of matrix A
 * \param int j: column index of matrix b
 *
 * This helper function computes the row x column product, in order to
 * compute a matrix product.
 */
static DOUBLE CSRxCSC_row_times_col(CSR_matrix* A, CSC_matrix* b, int i, int j)
{
	int start_i = A->row_start[i];
	int end_i = A->row_start[i + 1];
	int start_j = b->col_start[j];
	int end_j = b->col_start[j + 1];

	if (A->indices[start_i] > b->indices[end_i - 1] && A->indices[end_i - 1] < b->indices[start_i]) return 0;

	int I;
	int J = 0;

	int k = start_i;
	int l = start_j;
	DOUBLE sum = 0;
	while (k < end_i)
	{
		I = A->indices[k];
		while (l < end_j)
		{
			J = b->indices[l];
			if (J >= I) break;
			l++;
		}
		if (I == J) sum += A->elements[k] * b->elements[l];
		k++;
	}

	return sum;
}

/**
 * \brief Computes the matrix product
 * \param CSR_matrix* A: first factor of matrix product
 * \param CSC_matrix* b: second factor of matrix product
 * \return CSR_matrix*: pointer to the newly allocated matrix product P=A.b
 *
 * This method computes the matrix product P of the (n x k)-matrix A and the (k x m)-matrix b. The
 * result is the (n x m)-matrix P=A.b. For faster computation the matrix A is given in row format and
 * b in column format.
 */
CSR_matrix* matrix_product_to_CSR(CSR_matrix* A, CSC_matrix* b)
{
	int i, j;
	DOUBLE sum;

	int n = A->rows;
	int m = b->cols;
	int M = b->col_start[m];

	CSR_matrix* P = (CSR_matrix*)malloc(sizeof(CSR_matrix));
	P->rows = n;

	int ptr = 0;
	int blocks = 0;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < m; j++)
		{
			sum = CSRxCSC_row_times_col(A, b, i, j);
			if (sum != 0)
			{
				if (ptr == blocks*M)
				{
					blocks++;
				}
				P->indices[ptr] = j;
				P->elements[ptr] = sum;
				ptr++;
			}
		}
		P->row_start[i + 1] = ptr;
	}

	int size = P->row_start[n];

	return P;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////* integer vector function */////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Allocates a list of integers initialized by zero
 * \param int n: number of entries of the list
 * \return int* : pointer to allocated list
 */
int* zero_int_list(int n)
{
	return (int*)calloc(n, sizeof(int));
}
