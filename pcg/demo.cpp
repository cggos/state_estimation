#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "double.h"
#include "matrix.h"
#include "pcg.h"
#include "util.h"

/*
 * This is a demo program which illustrates the usage of the PCG algorithm
 * It computes the solution for a Poisson equation in 2D on a square domain
 * with a point source in the middle. The output is a textfile with the
 * iteration number and the computation time (using C's clock()-function).
 * This is done for all available preconditioners except for the user-defined.
 * Copy "PCG.c, pcg.h, matrix.c, matrix.h" in the folder where the demo
 * program is compiled. Then run 'make'.
 */

int main(int argc, char* argv[])
{
	const int max_iter = 100;			// maximum number of allowed iterations 
	const DOUBLE tol = TOLERANCE;		// required residuum ratio reduction

	int max_size, mode, iter, size;
	char line[512] = "\0";
	char strval[512] = "\0";

	clock_t t, t0;

	max_size = MAX_SIZE;

	printf("\nstart\n");
	fflush(stdout);

	size = max_size;

	// Creates a Poisson-equation system in 2D with Dirichlet boundary conditions X(bound)=0 and a point source in the center
	// refer to http://zhuanlan.zhihu.com/p/130808947 for details
	CSR_matrix A;
	Laplace2D_5point(&A, size);

	double a1[MAX_SIZE][MAX_SIZE] = { {0} };
	CSR2Dense(&A, a1[0]);
	CSR_matrix a2;
	Dense2CSR(a1[0], &a2, MAX_SIZE);

	CSR_scale_matrix(&A, -1.);
	DOUBLE X[MAX_SIZE];
	zero_vector(X, size);
	DOUBLE b[MAX_SIZE];
	SourceTerm2D(b, size);

	char filename[256];
	sprintf(filename, "E:\\A.txt", 1);
	Print_Dense_Matrix(a1[0], size, filename);
	sprintf(filename, "E:\\A.mtx", 1);
	print_CSR_matrix_market(&A, filename);
	sprintf(filename, "E:\\b.txt", 1);
	print_vector(b, size, filename);

	printf("solving with %d nodes ...\n", size);
	fflush(stdout);
	for (mode = PCG_NONE; mode <= PCG_ICHOL; mode++)
	{
		if ((PrecId)mode != PCG_USER_DEFINED)
		{
			PCG_set_preconditioner_mode(&A, (PrecId)mode);
			t0 = clock();
			clear_vector(X, size);
			iter = PCG_solve(&A, X, b, size, tol, max_iter);
			t = clock();
			if (iter >= max_iter)
			{
				printf("Precond no. %d did not converge after %d iters with tolerance %e\n", mode, max_iter, (double)tol);
			}
			sprintf(strval, "%d:%f\t", iter, (float)(t - t0) / CLOCKS_PER_SEC);
			strcat(line, strval);
			sprintf(filename, "E:\\X%d.txt", mode);
			print_vector(X, size, filename);
		}
	}
	printf("%s\n", line);
	PCG_clean();

	printf("\nfinished -> test successfull !\n");

	getchar();
	return 0;
}
