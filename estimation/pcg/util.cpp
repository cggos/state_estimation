#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "double.h"
#include "matrix.h"
#include "pcg.h"

FOR_DEBUG void Laplace1D_3point(CSR_matrix *Res, int size)
{
	int i;

	Res->rows = size;

	int pos = 0;
	for (i = 0; i < size; i++)
	{
		Res->row_start[i] = pos;

		if (i > 0)
		{
			Res->indices[pos] = i - 1;
			Res->elements[pos++] = 1.0;
		}
		Res->indices[pos] = i;
		Res->elements[pos++] = -2.;
		if (i < size - 1)
		{
			Res->indices[pos] = i + 1;
			Res->elements[pos++] = 1.0;
		}
	}
	Res->row_start[i] = pos;

	return;
}

FOR_DEBUG void Laplace2D_5point(CSR_matrix* Res, int size)
{
	int i, j;

	int width = (int)round(sqrt(size));

	Res->rows = size;

	int pos = 0;
	int var = 0;
	for (i = 0; i < width; i++)
	{
		for (j = 0; j < width; j++)
		{
			Res->row_start[var] = pos;

			if (i > 0)
			{
				Res->indices[pos] = var - width;
				Res->elements[pos++] = 1.;
			}

			if (j > 0)
			{
				Res->indices[pos] = var - 1;
				Res->elements[pos++] = 1.;
			}

			Res->indices[pos] = var;
			Res->elements[pos++] = -4.;

			if (j < width - 1)
			{
				Res->indices[pos] = var + 1;
				Res->elements[pos++] = 1.;
			}

			if (i < width - 1)
			{
				Res->indices[pos] = var + width;
				Res->elements[pos++] = 1.;
			}

			var++;
		}
	}
	Res->row_start[var] = pos;

	return;
}

FOR_DEBUG void SourceTerm1D(DOUBLE* Res, int size, DOUBLE left_val, DOUBLE right_val)
{
	int i;

	int n1 = size / 4;
	int n2 = 3 * n1;

	Res[n1] = 1.;
	Res[n2] = -5.;

	for (i = 0; i < size; i++) Res[i] /= (DOUBLE)(size - 1)*(DOUBLE)(size - 1);

	Res[0] = -left_val;
	Res[size - 1] = -right_val;

	return;
}

FOR_DEBUG void check_equ(double a, double b)
{
	if (fabs(a - b) > 1e-6)
	{
		int debug = 1;
	}
	else
	{
		int debug = 0;
	}
}

FOR_DEBUG void SourceTerm2D(DOUBLE* Res, int size)
{
	int i, j;
	DOUBLE x, y, f, k, p, q;
	//double xx, yy, ff, kk, pp, qq;
	DOUBLE bound_val = 0;
	double res;

	int width = (int)sqrt(size);

	for (i = 0; i < width; i++)
	{
		x = (DOUBLE)i / (DOUBLE)(width - 1);
		//xx = (double)i / (double)(width - 1);
		//check_equ(x, xx);
		for (j = 0; j < width; j++)
		{
			y = (DOUBLE)j / (DOUBLE)(width - 1);
			//yy = (double)j / (double)(width - 1);
			//check_equ(y, yy);
			p = ((x - (DOUBLE)0.5)*(x - (DOUBLE)0.5));
			//pp = ((xx - (double)0.5)*(xx - (double)0.5));
			//check_equ(p, pp);
			q = ((y - (DOUBLE)0.5)*(y - (DOUBLE)0.5));
			//qq = ((yy - (double)0.5)*(yy - (double)0.5));
			//check_equ(q, qq);
			k = (p + q);
			//kk = (pp + qq);
			//check_equ(k, kk);
			f = exp(-20.*k);
			//ff = exp(-20.*kk);
			//check_equ(f, ff);
			Res[i*width + j] = 1e3 * (DOUBLE)f / (DOUBLE)((width - 1)*(width - 1));
			//res = 1e3 * (double)f / (double)((width - 1)*(width - 1));
			//check_equ(Res[i*width + j], res);
		}
	}

	for (i = 0; i < width; i++)
	{
		Res[i] += bound_val;
		Res[i*width] += bound_val;
		Res[(i + 1)*width - 1] += bound_val;
		Res[width*(width - 1) + i] += bound_val;
	}

	return;
}

FOR_DEBUG void Dense2CSR(const double* dense_mat, CSR_matrix* Res, int size)
{
	Res->rows = size;

	int pos = 0;
	int index = 0;
	for (int i = 0; i < size; i++)
	{
		Res->row_start[i] = pos;
		for (int j = 0; j < size; j++)
		{
			if (dense_mat[index] != 0)
			{
				Res->indices[pos] = j;
				Res->elements[pos] = dense_mat[index];
				pos++;
			}
			index++;
		}
	}
	Res->row_start[size] = pos;
}

FOR_DEBUG void CSR2Dense(const CSR_matrix* Res, double* dense_mat)
{
	int size = Res->rows;

	int index = 0;
	for (int i = 0; i < size; i++)
	{
		int pos_start = Res->row_start[i];
		int pos_end = Res->row_start[i + 1];
		double *data = dense_mat + i*size;
		for (int j = pos_start; j < pos_end; j++)
		{
			data[Res->indices[j]] = Res->elements[j];
		}
	}
}

FOR_DEBUG void Print_Dense_Matrix(double* dense_mat, int size, char* file_name)
{
	if (dense_mat != NULL)
	{
		FILE* file = fopen(file_name, "wt");
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				fprintf(file, "%.10e\t", dense_mat[i*size + j]);
			}
			fprintf(file, "\r\n");
		}
	}
}
