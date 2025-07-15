///////////////////////////////////////////* public types *//////////////////////////////////////////////////////////////////

/**
 * ID for preconditioner used in the PCG algorithm
 */
typedef enum PREC_ID{
	PCG_NONE,							/**< Preconditioner mode ID: no preconditioner used */ 
	PCG_JACOBI,							/**< Preconditioner mode ID: Jacobi preconditioner used */ 
	PCG_ICHOL,							/**< Preconditioner mode ID: incomplete Cholesky preconditioner used */ 
	PCG_USER_DEFINED					/**< Preconditioner mode ID: user-defined preconditioner used */ 
}PrecId;

//////////////////////////////////////* public functions */////////////////////////////////////////////////////////////

int PCG_solve(CSR_matrix* A, DOUBLE* X, DOUBLE* b,int n, DOUBLE tol,int max_iter);
void PCG_set_preconditioner_mode(CSR_matrix* A, PrecId mode);
void PCG_set_precon(void (*Precon)(CSR_matrix* A, DOUBLE* R, DOUBLE* X,int n));
void PCG_set_mult(void (*Mult)(DOUBLE* R, DOUBLE* X,int n));
void PCG_clean();
void CSR_incompleteCholeski(CSR_matrix* A,CSR_matrix* L);
