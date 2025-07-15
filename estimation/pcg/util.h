void Laplace1D_3point(CSR_matrix *Res, int size);
void Laplace2D_5point(CSR_matrix* Res, int size);
void SourceTerm1D(DOUBLE* Res, int size, DOUBLE left_val, DOUBLE right_val);
void check_equ(double a, double b);
void SourceTerm2D(DOUBLE* Res, int size);
void Dense2CSR(const double* dense_mat, CSR_matrix* Res, int size);
void CSR2Dense(const CSR_matrix* Res, double* dense_mat);
void Print_Dense_Matrix(double* dense_mat, int size, char* file_name);
