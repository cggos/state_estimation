///////////////////////////////////////// public structs ////////////////////////////////////////////////////////////////////////

/**
* Matrix type in Compressesed Sparse Row (CSR) format
*
* see https://en.wikipedia.org/wiki/Sparse_matrix for details of the data format
*
*/
typedef struct CSR {
	int rows;											/**< number of rows */
	int row_start[MAX_SIZE + 1] = { { 0 } };			/**< indices that point to the beginning of each row in indices */
	int indices[MAX_ELEM] = { { 0 } };					/**< list of all indices */
	DOUBLE elements[MAX_ELEM] = { { 0 } };				/**< list of all values */
}CSR_matrix;

/**
* Matrix type in Compressesed Sparse Column (CSC) format
*
* see https://en.wikipedia.org/wiki/Sparse_matrix for details of the data format
*
*/
typedef struct CSC {
	int cols;											/**< number of columns */
	int col_start[MAX_SIZE + 1] = { { 0 } };			/**< indices that point to the beginning of each row in indices */
	int indices[MAX_ELEM] = { { 0 } };					/**< list of all indices */
	DOUBLE elements[MAX_ELEM] = { { 0 } };				/**< list of all values */
}CSC_matrix;

//////////////////////////////////////// public functions ///////////////////////////////////////////////////////////////////////

// print functions
void print_vector(DOUBLE* V, int n, const char* Name);
void print_list(int* V, int n, const char* Name);
void print_CSR(CSR_matrix* A, const char* Name);
void print_CSC(CSC_matrix* A, const char* Name);
void print_CSR_matrix_market(CSR_matrix* A, const char* Name);
void print_CSC_matrix_market(CSC_matrix* A, const char* Name);

// vector functions
void zero_vector(DOUBLE* V, int n);
void clear_vector(DOUBLE* V, int n);

// matrix functions
void free_CSR_matrix(CSR_matrix** const A);
void free_CSC_matrix(CSC_matrix** const A);
void CSR_scale_matrix(CSR_matrix* A, DOUBLE factor);
CSR_matrix* matrix_product_to_CSR(CSR_matrix* A, CSC_matrix* b);

// integer vector function
int* zero_int_list(int n);

