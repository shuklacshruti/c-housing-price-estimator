#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/// Function prototypes
void transpose(double **matrix, double **result, int rows, int cols);
void multiply(double **matrixA, double **matrixB, double **result, int rowsA, int colsA, int colsB);
void multiply_with_vector(double **matrixA, double *vectorB, double *result, int rowsA, int colsA);
void invert(double **matrix, int n, double **result);
void predict(double **X, double *W, int m, int k);

// Safe allocation with error checking
void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if (!ptr) {
        perror("Memory allocation failed");
        exit(1);
    }
    return ptr;
}

int main(int argc, char *argv[]) {
    printf("Starting main function \n"); //to check

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <train_file> <data_file>\n", argv[0]);
        return 1;
    }

    FILE *train_file = fopen(argv[1], "r");
    FILE *data_file = fopen(argv[2], "r");
    if (!train_file || !data_file) {
        perror("Error opening file");
        return 1;
    }

    int k, n;
    fscanf(train_file, "%*s %d %d", &k, &n);
    printf("Read k=%d, n=%d from training file \n",k, n); //to check

    // Allocate X and Y
    double **X = (double **)safe_malloc(n * sizeof(double *));
    double *Y = (double *)safe_malloc(n * sizeof(double));

    for (int i = 0; i < n; i++) {
        X[i] = (double *)safe_malloc((k + 1) * sizeof(double));
        X[i][0] = 1.0;  // Bias term
    }

    // Read data into X and Y
    for (int i = 0; i < n; i++) {
        for (int j = 1; j <= k; j++) {
            fscanf(train_file, "%lf", &X[i][j]);
        }
        fscanf(train_file, "%lf", &Y[i]);
    }
    fclose(train_file);
    printf("Allocating & populating X & Y matrices \n"); //to check

     // Allocate transpose and result matrices
    double **X_T = (double **)safe_malloc((k + 1) * sizeof(double *));
    double **XTX = (double **)safe_malloc((k + 1) * sizeof(double *));
    double **XTX_inv = (double **)safe_malloc((k + 1) * sizeof(double *));
    double **XTY = (double **)safe_malloc((k + 1) * sizeof(double*));
    double *W = (double *)safe_malloc((k + 1) * sizeof(double));

    for (int i = 0; i <= k; i++) {
        X_T[i] = (double *)safe_malloc(n * sizeof(double));
        XTX[i] = (double *)safe_malloc((k + 1) * sizeof(double));
        XTX_inv[i] = (double *)safe_malloc((k + 1) * sizeof(double));
    }

    transpose(X, X_T, n, k + 1);
    printf(" transpose \n"); //checking
    multiply(X_T, X, XTX, k + 1, n, k + 1);
    printf(" multiplication x_T*X\n"); //checking
    multiply_with_vector(XTX_inv, XTY, W, k + 1, k + 1);
    printf("vetor multiplication \n"); //checking
    invert(XTX, k + 1, XTX_inv);
    printf("inversion"); //checking
    multiply(XTX_inv, &XTY, &W, k + 1, k + 1, 1);
    printf("multiplication for W"); //checking


   // Prediction setup
    int m;
    fscanf(data_file, "%*s %d %d", &k, &m);
    printf("prediction setup: m=%d, k=%d\n", m,k);

    double **X_prime = (double **)safe_malloc(m * sizeof(double *));
    for (int i = 0; i < m; i++) {
        X_prime[i] = (double *)safe_malloc((k + 1) * sizeof(double));
        X_prime[i][0] = 1.0;
        for (int j = 1; j <= k; j++) {
            fscanf(data_file, "%lf", &X_prime[i][j]);
        }
    }
    fclose(data_file);

    predict(X_prime, W, m, k);
    printf("cleaning up allocated memory \n");
    for (int i = 0; i < n; i++) {
        free(X[i]);
    }
    free(X);
    free(Y);
    for (int i = 0; i <= k; i++) {
        free(X_T[i]);
        free(XTX[i]);
        free(XTX_inv[i]);
    }
    free(X_T);
    free(XTX);
    free(XTX_inv);
    free(XTY);
    free(W);
    for (int i = 0; i < m; i++) {
        free(X_prime[i]);
    }   free(X_prime);
    printf("memory cleanup done \n");
        return 0; }


// Function definitions

// Transposes a matrix
void transpose(double **matrix, double **result, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j][i] = matrix[i][j];
        }}}

void multiply(double **matrixA, double **matrixB, double **result, int rowsA, int colsA, int colsB) {
    assert(matrixA != NULL && matrixB != NULL && result != NULL);
    assert(rowsA > 0 && colsA > 0 && colsB > 0);
     for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            result[i][j] = 0.0;
            for (int k = 0; k < colsA; k++) {
                 assert(i < rowsA && j < colsB && k < colsA);  
                result[i][j] += matrixA[i][k] * matrixB[k][j];
            }  } }}

// Multiplies a matrix with a vector
void multiply_with_vector(double **matrixA, double *vectorB, double *result, int rowsA, int colsA) {
    assert(matrixA != NULL && vectorB != NULL && result != NULL);
    for (int i = 0; i < rowsA; i++) {
        result[i] = 0.0;
        for (int j = 0; j < colsA; j++) {
            result[i] += matrixA[i][j] * vectorB[j];
        }}}

// Inverts a matrix using Gaussian elimination
void invert(double **matrix, int n, double **result) {
    double **temp = (double **)malloc(n * sizeof(double *));
    if (!temp) {
        perror("Memory allocation failed");
        exit(1);
    }
    for (int i = 0; i < n; i++) {
        result[i] = (double *)malloc(n * sizeof(double));
        if (!result[i]) {
            perror("Memory allocation failed");
            exit(1);
        }   }
    for (int i = 0; i < n; i++) {
        temp[i] = (double *)malloc(n * sizeof(double));
        if (!temp[i]) {
            perror("Memory allocation failed");
            exit(1); }
        for (int j = 0; j < n; j++) {
            temp[i][j] = matrix[i][j];
            result[i][j] = (i == j) ? 1.0 : 0.0; 
        } }

    // Forward elimination 
    for (int i = 0; i < n; i++) {
        double pivot = temp[i][i];
        if (pivot == 0.0) {
            fprintf(stderr, "Error: Matrix is singular or has zero pivot element.\n");
            exit(1);   }

        // Normalizing the pivot row
        for (int j = 0; j < n; j++) {
            temp[i][j] /= pivot;
            result[i][j] /= pivot;      }

        // Making all elements below the pivot in the current column zero
        for (int row = i + 1; row < n; row++) {
            double factor = temp[row][i];
            for (int col = 0; col < n; col++) {
                temp[row][col] -= factor * temp[i][col];
                result[row][col] -= factor * result[i][col];
            }       }    }

    // Back substitution to solve for the inverse
    for (int i = n - 1; i >= 0; i--) {
        for (int j = i - 1; j >= 0; j--) {
            double factor = temp[j][i];
            for (int col = 0; col < n; col++) {
                result[j][col] -= factor * result[i][col];
            } } }

    // Free the temporary matrix
    for (int i = 0; i < n; i++) {
        free(temp[i]); }
    free(temp);}

// Function to make predictions using the weight vector`
void predict(double **X, double *W, int m, int k) {
    for (int i = 0; i < m; i++) {
        double prediction = 0.0;
        for (int j = 0; j <= k; j++) {
            prediction += X[i][j] * W[j];
        }
        printf("Prediction for instance %d: %lf\n", i + 1, prediction);}} 