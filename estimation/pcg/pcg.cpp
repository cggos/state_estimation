#include <stdio.h>
#include <math.h>
#include <stdlib.h> 
#include <string.h>

#include "double.h"
#include "matrix.h"
#include "pcg.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////* local (global) variables *//////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static PrecId precon_mode = PCG_NONE;											// Preconditioner mode ID, function pointer.
static void(*precon)(CSR_matrix* A, DOUBLE* R, DOUBLE* X, int n) = NULL;				// point to an specific precondition function, which approximates (A^-1).X and stores in R
static void(*user_def_mult)(DOUBLE* R, DOUBLE* X, int n) = NULL;						// user-defined multiplcation routine R=A.X
static DOUBLE pcg_residuum;																// the final residuum is stored in this variable
static CSC_matrix IL;																	// incomplete Lower Triangular matrix
static CSC_matrix ILT;																	// the transposed of the above


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////* local helper functions *//////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//// list functions
/**
 * \brief Allocates a list of integers initialized by zero
 * \param int* L: pointer to allocated list
 * \param int size: size of list
 * \return void: none
 */
static void zero_int_list(int* L, int n)
{
	for (int i = 0; i < n; i++)
	{
		L[i] = 0;
	}
	return;
}

/**
 * \brief Inserts an integer into an ordered list
 * \param int* list: pointer to list
 * \param DOUBLE* vals: pointer to list of DOUBLE values associated with each integer
 * \param int size: number of entries in both lists
 * \param int index: index that is inserted
 * \param DOUBLE value: DOUBLE value that will be inserted in the DOUBLE-valued list
 *
 * This method operates on a row/column of a matrix with ordered indices.
 * The row/column is reporesented by a list of column/row (list) indices and the
 * associated matrix values (vals). Both, the index and the element values are inserted,
 * such that the ordering is preserved (in ascending order).
 * The method is not optimizeed yet but is sufficient for small row/column sizes (~10)
 */
static void insert_int_list(int* list, DOUBLE* vals, int size, int index, DOUBLE value)
{	// inserts element in ordered integer list 
	int i = 0;
	while (i < size)
	{
		if (index < list[i]) break;
		i++;
	}
	int d = size - i;
	if (d > 0)
	{
		memmove(&(list[i + 1]), &(list[i]), d * sizeof(int));
		memmove(&(vals[i + 1]), &(vals[i]), d * sizeof(DOUBLE));
	}
	list[i] = index;
	vals[i] = value;
}

/**
 * \brief Returns a copy of a vector
 * \param DOUBLE* V: pointer to the source vector
 * \param DOUBLE* C: pointer to the previously allocated copy of vector V
 * \param int n: dimension of vector
 * \return void : none
 */
static void copy_vector(DOUBLE* V, DOUBLE* C, int n)
{
	memmove((void*)C, (void*)V, n * sizeof(DOUBLE));
	return;
}

/**
 * \brief Scales a vector V <- factor*V
 * \param DOUBLE* V: pointer to the vector te be scaled
 * \param DOUBLE factor: scaling factor
 * \param int size: dimension of vector
 * \return void : none
 */
static void vector_scale(DOUBLE* V, DOUBLE factor, int size)
{
	for (int i = 0; i < size; i++)
	{
		V[i] *= factor;
	}
	return;
}

/**
 * \brief Adds a scaled vector V <- V+factor*Y
 * \param DOUBLE* V: pointer to the vector to be manipulated
 * \param DOUBLE* Y: pointer to the vector that will be added
 * \param DOUBLE factor: scaling factor
 * \param int size: dimension of vector
 * \return void : none
 */
static void vector_addmult(DOUBLE* V, DOUBLE* Y, DOUBLE factor, int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		V[i] += factor*Y[i];
	}
}

/**
 * \brief Computes the euclidian scalar product of two vectors
 * \param DOUBLE* V: first argument
 * \param DOUBLE* Y: second argument
 * \param int size: dimension of vector
 * \return DOUBLE: scalar product <V,Y>
 */
static DOUBLE vector_scalar(DOUBLE* V, DOUBLE* Y, int size)
{
	int i;
	DOUBLE sum = 0;
	for (i = 0; i < size; i++)
	{
		sum += V[i] * Y[i];
	}
	return sum;
}

/**
* \brief Computes the special euclidian scalar product of two vectors
* \param DOUBLE* V: first argument
* \param DOUBLE* Y: second argument
* \param int size: dimension of vector
* \param int pre_devide: the scalar factor that X and Y devided by previously before product operation
* \return DOUBLE: scalar product <V,Y>
*/
static DOUBLE pre_div_vector_scalar(DOUBLE* V, DOUBLE* Y, int size)
{
	int i;
	DOUBLE sum = 0;
	for (i = 0; i < size; i++)
	{
		sum += V[i] * Y[i];
	}
	return sum;
}


/**
 * \brief Computes the euclidian norm of a vector
 * \param DOUBLE* V: pointer to vector
 * \return DOUBLE: euclidian norm ||v|| = sqrt(<V,V>)
 */
static DOUBLE euclid_norm(DOUBLE* V, int size)
{
	return sqrt(vector_scalar(V, V, size));
}

//// matrix related functions
/**
 * \brief frees all memory of a CSR-matrix struct
 * \param CSR_matrix** A: pointer to the pointer where the matrix is stored
 */
static void free_CSR_matrix(CSR_matrix** A)
{
	if (*A != NULL)
	{
		free(*A);
		*A = NULL;
	}
}

/**
 * \brief Frees all memory of a CSC-matrix struct
 * \param CSC_matrix** A: pointer to the pointer where the matrix is stored
 */
static void free_CSC_matrix(CSC_matrix** A)
{
	if (*A != NULL)
	{
		free(*A);
		*A = NULL;
	}
}

/**
 * \brief Computes the tranpose of a CSR-matrix as a CSC-matrix
 * \param CSR_matrix* A: pointer the the CSR-matrix to be tranposed
 * \param CSC_matrix* Res: pointer to the newly allocated CSC-matrix that is the transposed of A (very fast)
 */
static void transpose_CSR_to_CSC(CSR_matrix* A, CSC_matrix* Res)
{
	int n = A->rows;
	int N = A->row_start[n];

	Res->cols = n;
	//Res->col_start = zero_int_list(n + 1);
	//Res->indices = zero_int_list(N);
	//Res->elements = zero_vector(N);

	memmove((void*)Res->col_start, (void*)A->row_start, (n + 1) * sizeof(int));
	memmove((void*)Res->indices, (void*)A->indices, N * sizeof(int));
	memmove((void*)Res->elements, (void*)A->elements, N * sizeof(DOUBLE));
	return;
}

/**
 * \brief Solves the a linear equation A.X=b, when the LU-decomposition of A=L.U is given
 * \param CSC_matrix* L: pointer to the lower triangular matrix of the LU decompostion
 * \param CSC_matrix* U: pointer to the upper triangular matrix of the LU decompostion
 * \param DOUBLE* X: pointer to vector where the result is stored in
 * \param DOUBLE* b: pointer to the right side of the equation A.X=b, A=L.U
 *
 * This method solves the the equation A.X=b, when the LU decompostion A=L.U is known
 * via forward/backward substitution. Both matrices must be given in column-format (CSC),
 * in order to better parallelize the task later.
 */
static void CSC_triangular_LU_invert(CSC_matrix* L, CSC_matrix* U, DOUBLE* X, DOUBLE* b)
{
	int j, I, J, start, end;
	DOUBLE x;

	int n = L->cols;
	X = (DOUBLE*)memmove((void*)X, (void*)b, n * sizeof(DOUBLE));

	// L.Y=b
	for (I = 0; I < n; I++)
	{
		start = L->col_start[I];
		x = X[I] / L->elements[start++];
		X[I] = x;
		for (j = start; j < L->col_start[I + 1]; j++)
		{
			J = L->indices[j];
			X[J] -= L->elements[j] * x;
		}
	}

	// U.X=Y	
	for (I = n - 1; I >= 0; I--)
	{
		end = U->col_start[I + 1] - 1;
		x = X[I] / U->elements[end--];
		X[I] = x;
		for (j = end; j >= U->col_start[I]; j--)
		{
			J = U->indices[j];
			X[J] -= U->elements[j] * x;
		}
	}
}

/**
 * \brief Converts a CSR-matrix to the CSC storage format
 * \param CSR_matrix* A: pointer the the matrix in row-storage format (CSR) the will be converted
 * \param CSC_matrix* Res: copy of the CSR matrix in column-storage format (CSC)
 * \param int cols: column number of the matrix A
 * \return void: none
 *
 * Since the column number is not explicitly stored in the CSR format, it must be passed
 * to the method. For invertable matrices it is usually #rows = #colums, which makes it simple.
 */
static void CSR_to_CSC_matrix(CSR_matrix* A, CSC_matrix* Res, int cols)
{

	int i, j, col;

	int n = A->rows;
	int N = A->row_start[n];
	int mean_bandwidth = N / n + 1;
	int* col_ptr = zero_int_list(cols);
	int* mem_sizes = zero_int_list(cols);
	int** Col_indices = (int**)malloc(cols * sizeof(int*));
	DOUBLE** Col_elements = (DOUBLE**)malloc(cols * sizeof(DOUBLE*));
	for (i = 0; i < cols; i++)
	{
		mem_sizes[i] = mean_bandwidth;
		Col_indices[i] = new int[mean_bandwidth];
		Col_elements[i] = new DOUBLE[mean_bandwidth];
	}

	// construct (ordered) column lists
	for (i = 0; i < n; i++)
	{
		for (j = A->row_start[i]; j < A->row_start[i + 1]; j++)
		{
			col = A->indices[j];
			if (col >= cols)
			{
				printf("error in function %s in file %s: index %d exceeds given column number %d -> abort\n", __func__, __FILE__, col, cols);
				exit(EXIT_FAILURE);
			}
			// use index-ordered insert here 
			insert_int_list(Col_indices[col], Col_elements[col], col_ptr[col], i, A->elements[j]);
			col_ptr[col]++;
			if (col_ptr[col] == mem_sizes[col] - 1)
			{
				mem_sizes[col] += mean_bandwidth;
				Col_indices[col] = (int*)realloc(Col_indices[col], mem_sizes[col] * sizeof(int));
				Col_elements[col] = (DOUBLE*)realloc(Col_elements[col], mem_sizes[col] * sizeof(DOUBLE));
			}

		}

	}

	Res->cols = cols;

	// assumes ordered columns
	int Res_ptr = 0;
	for (i = 0; i < cols; i++)
	{
		for (j = 0; j < col_ptr[i]; j++)
		{
			Res->indices[Res_ptr] = Col_indices[i][j];
			Res->elements[Res_ptr] = Col_elements[i][j];
			Res_ptr++;
		}
		Res->col_start[i + 1] = Res_ptr;
	}

	// clean
	for (i = 0; i < cols; i++)
	{
		free(Col_indices[i]);
		free(Col_elements[i]);
	}
	free(Col_indices);
	free(Col_elements);
	free(mem_sizes);
	free(col_ptr);

	return;
}

/**
 * \brief Helper function for Cholesky decomposition
 * \param CSR_matrix* L: Pointer where the lower triangular factor of the decomposition is stored
 * \param int row1: index of first row
 * \param int row2: index of second row
 * \param int max_ind: index limiter for the row product
 * \return DOUBLE: row product inside matrix L: \sum_{k<max_ind} L_{row1,k}*L_{row2,k}
 */
static DOUBLE CSR_row_product(CSR_matrix* L, int row1, int row2, int max_ind)
{

	int start_i = L->row_start[row1];
	int end_i = L->row_start[row1 + 1];
	int start_j = L->row_start[row2];
	int end_j = L->row_start[row2 + 1];

	int i = start_i;
	int j = start_j;
	int I = L->indices[i];
	int J = L->indices[j];

	DOUBLE sum = 0;
	while (i < end_i)
	{
		I = L->indices[i];
		if (I >= max_ind) break;
		while (j < end_j)
		{
			J = L->indices[j];
			if (J >= I) break;
			j++;
		}
		if (J == I) sum += L->elements[i] * L->elements[j];
		i++;
	}
	return sum;
}

/**
 * \brief Gives a diagonal element of matrix A
 * \param CSR_matrix* A: pointer to matrix
 * \param int row: row index of the diagonal element
 * \return DOUBLE: diagonal element A_{row,row} or zero if row exceeds the row number of the matrix
 */
static DOUBLE CSR_get_diag_element(CSR_matrix* A, int row)
{
	if (row >= A->rows) return 0;

	int j = A->row_start[row];
	int end = A->row_start[row + 1];
	int J = A->indices[j];
	while (j < end)
	{
		J = A->indices[j];
		if (J >= row) break;
		j++;
	}
	if (J == row)
		return A->elements[j];
	else
		return 0;
}

/**
 * \brief Computes the incomplete Cholesky decomposition of a matrix
 * \param CSR_matrix* A: pointer to the matrix to be factorized
 * \param CSR_matrix* L: pointer where the resulting factor L.L^T=A is stored. Allocate in uninitialized CSR struct and pass the pointer.
 * \return void: none
 *
 * This method computes the incomplete Cholesky decomposition A=L.L^T. The matrix to be factorized must
 * be symmetric and positive definite to ensure existence of the factorization. For negative definete symmetric matrices
 * multiply by -1 to obtain a positive definite matrix before decomposition.
 */
void CSR_incompleteCholeski(CSR_matrix* A, CSR_matrix* L)
{
	int start, end, j, k, I, J;
	DOUBLE Lii, Lij;

	int n = A->rows;
	int N = A->row_start[n];

	// create pattern of L (lower diagonal)
	L->rows = n;
	//L->row_start = zero_int_list(n + 1);
	//L->indices = zero_int_list(N);
	//L->elements = zero_vector(N);
	L->row_start[0] = 0;

	for (I = 0; I < n; I++)
	{
		start = A->row_start[I];
		end = A->row_start[I + 1];

		j = start;
		k = L->row_start[I];
		while (j < end)
		{
			J = A->indices[j++];
			if (J > I) break;
			L->indices[k++] = J;
		}
		L->row_start[I + 1] = k;
	}
	int M = L->row_start[n];
	//L->indices = (int*)realloc(L->indices, M * sizeof(int));
	//L->elements = (DOUBLE*)realloc(L->elements, M * sizeof(DOUBLE));


	int* col_ptrs_L = zero_int_list(n);
	memmove(col_ptrs_L, L->row_start, n * sizeof(int));

	for (I = 0; I < n; I++)
	{
		Lii = CSR_get_diag_element(A, I) - CSR_row_product(L, I, I, I);
		if (Lii <= 0)
		{
			printf("error in function %s in file %s: improper values found at row %d-> abort\n Maybe you should check if your matrix is symmetric and positive-definite\n", __func__, __FILE__, I);
			return;
		}
		Lii = sqrt(Lii);
		L->elements[col_ptrs_L[I]++] = Lii;

		start = A->row_start[I];
		j = A->row_start[I + 1] - 1;
		while (j >= start)
		{
			J = A->indices[j];
			if (J <= I) break;
			Lij = (A->elements[j] - CSR_row_product(L, I, J, I)) / Lii;
			L->elements[col_ptrs_L[J]++] = Lij;
			j--;
		}
	}

	free(col_ptrs_L);
}

/**
 * \brief Computes the matrix-vector product
 * \param DOUBLE* R: pointer to vector where the product is stored. The memory must be allovcated before passing to the method.
 * \param CSR_matrix* A: pointer to the mapping matrix
 * \param DOUBLE* X: pointer to vector that is multiplied by A
 * \return void: none
 *
 * The memory where the result R is stored must be allocated before passed to this function.
 * The dimension of R must be equal the the row number of the matrix A
 */
static void CSR_matrix_mult(DOUBLE* R, CSR_matrix* A, DOUBLE* X)
{

	int* ptr_start = A->row_start;
	int* ptr_ind = A->indices;
	DOUBLE* ptr_mat = A->elements;
	DOUBLE* ptr_res = R;

	int i = 0;
	int n = A->row_start[A->rows];
	while (i < n)
	{
		ptr_start++;
		*ptr_res = 0;						// if removed, then A.x is added to R
		while (i < *ptr_start)
		{
			*ptr_res += (*ptr_mat)*X[*ptr_ind];
			ptr_ind++;
			ptr_mat++;
			i++;
		}
		ptr_res++;
	}
}

/**
 * \brief Multiplication routine
 * \param CSR_matrix* A: pointer to system matrix, which is a standard matrix in Compressed Row Storage (CSR) format
 * \param DOUBLE* R: pointer where the result of multiplicatione with the system matrix is stored
 * \param DOUBLE* X: pointer to vector that is multiplicated
 * \param int n: dimension of the vectors
 * \return void: none
 */
static void PCG_mult(CSR_matrix* A, DOUBLE* R, DOUBLE* X, int n)
{
	if (user_def_mult != NULL)
	{
		user_def_mult(R, X, n);
	}
	else if (A != NULL)
	{
		CSR_matrix_mult(R, A, X);
	}
	else
	{
		printf("error in function %s in file %s: no system matrix and no user-defined multiplication routine set -> abort", __func__, __FILE__);
		exit(EXIT_FAILURE);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////7////////////////////* preconditioners */////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Dummy preconditioner
 * \param CSR_matrix* A: pointer to the system matrix A, for which A.X=b will be solved
 * \param DOUBLE* R: pointer to memory where the result ist stored
 * \param DOUBLE* X: pointer to vector in which the preconditioner is applied
 * \param int n: dimension of vectors
 *
 * This is a dummy preconditioner, i.e. no preconditioning is applied.
 * The memory where R points to must be allocated before passing.
 */
void dummy_precon(CSR_matrix* A, DOUBLE* R, DOUBLE* X, int n)
{
	memmove((void*)R, (void*)X, n * sizeof(DOUBLE));
}

/**
 * \brief Simple Jacobi preconditioner
 * \param CSR_matrix* A: pointer to system matrix, which is a standard matrix in Compressed Row Storage (CSR) format
 * \param DOUBLE* R: pointer to memory where the result ist stored
 * \param DOUBLE* X: pointer to vector in which the preconditioner is applied
 * \param int n: dimension of vectors
 *
 * This is the simplest preconditioner M.R=X. The preconditioning operation X=M^{-1}.R is performed
 * by setting M=diagonal(A), where A is the system matrix. The system matrix must be stored in the
 * global variable "A". The memory where R points to must be allocated before passing.
 *
 */
void CSR_Jacobi_precon(CSR_matrix* A, DOUBLE* R, DOUBLE* X, int n)
{
	if (A != NULL)
	{
		int i, j;
		DOUBLE aii;

		for (i = 0; i < A->rows; i++)
		{
			for (j = A->row_start[i]; j < A->row_start[i + 1]; j++)
			{
				if (A->indices[j] == i)
				{
					aii = A->elements[j];
					if (aii != 0) R[i] = X[i] / aii;
				}
			}
		}
	}
}

/**
 * \brief Gauss-Seidel preconditioner
 * \param CSR_matrix* A: pointer to system matrix, which is a standard matrix in Compressed Row Storage (CSR) format
 * \param DOUBLE* R: pointer to memory where the result ist stored
 * \param DOUBLE* X: pointer to vector in which the preconditioner is applied
 * \param int n: dimension of vectors
 *
 * maybe useless
 */
void CSR_GausSeidel_precon(CSR_matrix* A, DOUBLE* R, DOUBLE* X, int n)
{
	if (A != NULL)
	{
		int i, j;
		DOUBLE sum;
		for (i = 0; i < A->rows; i++)
		{
			sum = X[i];
			for (j = A->row_start[i]; j < A->row_start[i + 1]; j++)
			{
				if (A->indices[j] < i) sum -= A->elements[j] * R[A->indices[j]];
				if (A->indices[j] == i)
				{
					R[i] = sum / A->elements[j];
					break;
				}
			}
		}
	}
}

/**
 * \brief Incomplete-Cholesky preconditioner
 * \param CSR_matrix* A: pointer to system matrix, which is a standard matrix in Compressed Row Storage (CSR) format
 * \param DOUBLE* R: pointer to memory where the result ist stored
 * \param DOUBLE* X: pointer to vector in which the preconditioner is applied
 * \param int n: dimension of vectors
 *
 * This preconditioner assumes M=L.L^{T}, where L is the incomplete Cholesky factor of the
 * system matrix. The preconditioning is basically forward/backward solving of a triangular system.
 * The factor L and its transpose L^{T} must be stored in the global variables IL and ILT, respectively.
 * The memory where R points to must be allocated before passing.
 */
void CSC_IC_precon(CSR_matrix* A, DOUBLE* R, DOUBLE* X, int n)
{								// incomplete Cholesky decomposition
	CSC_triangular_LU_invert(&IL, &ILT, R, X);
}

/**
 * \brief Sets the preconditioner
 * \param CSR_matrix* A: pointer to the system matrix A, for which A.X=b will be solved
 * \param PrecId mode: preconditioner-mode ID
 *
 * This method sets the preconditioner that will be used for the PCG algorithm. Since for some preconditioners
 * precomputations must be performed, the system matrix can be passed, too.
 * It is passed as a void pointer but make sure that the struct it points to, has the same structure as CSR_matrix
 * in external libraries.
 *
 * If System_matrix is set to NULL, the default matrix does not change. This can make sense, if one wants
 * to change the preconditioner method but not the system to be solved.
 *
 * If the System_matrix is not NULL the global variable A is set and it is not necessary to call
 * "PCG_set_default_matrix" any more.
 *
 * There are currently 4 preconditioner modes avialable:
 *
 * 1. mode==PCG_NONE: no preconditioner is used
 * 2. 		PCG_USER_DEFINED: user defined preconditioner
 * 							If this mode is used, the method "PCG_set_precon" must be called to
 * 							set the preconditioner.
 * 3. 		PCG_JACOBI: Jacobi preconditioner
 * 4.		PCG_ICHOL: Incomplete Cholesky preconditioner
 *
 */
void PCG_set_preconditioner_mode(CSR_matrix* A, PrecId mode)
{

	CSR_matrix* Row_L = NULL;
	precon_mode = mode;
	if (A != NULL) A = A;

	switch (precon_mode)
	{
	case PCG_NONE:
		precon = dummy_precon;
		break;
	case PCG_USER_DEFINED:
		precon = NULL;
		break;
	case PCG_JACOBI:
		precon = CSR_Jacobi_precon;
		break;
	case PCG_ICHOL:
		precon = CSC_IC_precon;
		if (A == NULL)
		{
			if (A != NULL) A = A;
			else
			{
				printf("warning: ILU preconditioner used but no system matrix defined -> skip\n");
				return;
			}
		}
		Row_L = (CSR_matrix*)malloc(sizeof(CSR_matrix));
		CSR_incompleteCholeski(A, Row_L);

		CSR_to_CSC_matrix(Row_L, &IL, A->rows);
		transpose_CSR_to_CSC(Row_L, &ILT);

		free_CSR_matrix(&Row_L);
		break;
	default:
		precon = dummy_precon;
		precon_mode = PCG_NONE;
		printf("warning: no valid preconditioner ID given -> NONE preconditioner set\n");
	}
}

/**
 * \brief Cleans internally allocated numerical objects used by the PCG algorithm
 *
 */
void PCG_clean()
{
	//if (IL != NULL) free_CSC_matrix(&IL);
	//if (ILT != NULL) free_CSC_matrix(&ILT);
}

/**
 * \brief Sets a user-defined preconditioner
 * \param void (*Precon)(CSR_matrix* A,DOUBLE* R,DOUBLE* X,int n): pointer to the preconditioner method.
 *
 * This function must be called when the preconditioner mode "USER_DEFINED" is set.
 * The Argument Precon must perform a preconditioner operation on X. The result M^{-1}.X must stored
 * at pointer R. n denotes the dimension of the vectors.
 */
void PCG_set_precon(void(*Precon)(CSR_matrix* A, DOUBLE* R, DOUBLE* X, int n))
{
	precon = Precon;
}

/**
 * \brief Sets a user-defined multiplicatione routine
 * \param void (*Mult)(DOUBLE* R,DOUBLE* X,int n): pointer to system matrix multiplication method
 *
 * There is a default matrix-multiplication routine implemented in the code but the user might want to
 * use its own, wich can be set with this function. If the preconditioner mode is also "USER_DEFINED", then
 * no explicit system matrix must be set.
 *
 * In the multiplicatione method the result is stored at pointer R and X represents the pointer to the vector that
 * is multiplied by the system matrix. n denotes the dimension of the system.
 */
void PCG_set_mult(void(*Mult)(DOUBLE* R, DOUBLE* X, int n))
{
	user_def_mult = Mult;
}

/**
* \brief caculate the absolute value of a Double value
* \param const Double value: Double value input
* \return Double: the absolute value output
*/
Double abs(const Double value)
{
	Double res;
	res.data = abs(value.data);

	return res;
}

/**
 * \brief Solves a linear equation with the PCG method.
 * \param CSR_matrix* A: pointer to the system matrix A, for which A.X=b will be solved
 * \param DOUBLE* X: pointer to the vector where the solution of A.X=b is stored.
 * \param DOUBLE* b: pointer to the vector representing the right-hand side if the equation A.X=b
 * \param int n: dimension of the system
 * \param DOUBLE tol: tolerance for the residuum reductioen r/r0
 * \param int max_iter: maximum number of iterations until termination
 * \return int: actual number of iterations for the PCG to reduce the residuum ratio to r/r0<tol
 *
 * This is the solution method for the linear system A.X=b with help of the Preconditioned Conjugated Gradient (PCG)
 * method. Before using this routine, one must set the preconditioner and the system matrix. Both can be done
 * by calling "PCG_set_preconditioner_mode". Alternatively, one can specify the matrix-mutiplcation and preconditioning
 * routine by hand if favored. In the latter case, one does not have to pass the system matrix explicitly.
 *
 * When solving an equation, an initial guess X0 must be pointed to by X. After the algorithm has converged the
 * solution is stored at X, and the initial guess is overwritten. Convergence is assumed when the ratio of the
 * current residuum r/r0 = ||b-A.X||/||b-A.X0||<tol. If this condition is not fulfilled within max_iter iterations
 * no convergence is assumed and the algorithm is terminated.
 *
 * Note that the PCG algorithm works only for symmetric and positive-definite matrices. The "ICHOL"
 * preconditioner might complain for negative-definite matrices. In this case (not for indefinite matrices),
 * simply multiply the matrix by -1 (i.e. solve (-A).x=-b).
 */
int PCG_solve(CSR_matrix* A, DOUBLE* X, DOUBLE* b, int n, DOUBLE tol, int max_iter)
{
	DOUBLE r, alpha, beta, s = 0, s_prev = 0;
	DOUBLE P[MAX_SIZE], Q[MAX_SIZE];

	DOUBLE R[MAX_SIZE];
	PCG_mult(A, R, X, n);
	vector_scale(R, -1., n);
	vector_addmult(R, b, 1., n);

	DOUBLE r0 = euclid_norm(R, n);
	if (r0 == (DOUBLE)0)
	{
		return 0;
	}

	DOUBLE Z[MAX_SIZE];

	int i = 0;
	do
	{
		(*precon)(A, Z, R, n);
		s_prev = s;
		s = vector_scalar(Z, R, n);

		if (i > 0)
		{
			beta = s / s_prev;
			vector_scale(P, beta, n);
			vector_addmult(P, Z, 1., n);
		}
		else
		{
			copy_vector(Z, P, n);
			zero_vector(Q, n);
		}
		(*PCG_mult)(A, Q, P, n);
		alpha = s / vector_scalar(P, Q, n);
		vector_addmult(X, P, alpha, n);
		vector_addmult(R, Q, -alpha, n);
		r = euclid_norm(R, n);
		i++;

		//printf("residuum ratio: %e\n",r/r0);	
		if (abs(r / r0) < tol)
			break;


	}
	while (i < max_iter);
	pcg_residuum = r;

	return i;
}
