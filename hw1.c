/*
Assignment 1: NxM Matrix Multiplication
CSCI-2500 Fall 2021
*/

/******************************************************************************/
/* Usual suspects to include  */
/******************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>

/******************************************************************************/
/* Function Calls */
/******************************************************************************/

typedef struct{
  double** data;
  int rows;
  int cols;
} matrix;

int mm_alloc(matrix* mat);
int mm_free (matrix* mat);
int mm_print(matrix* mat);
int mm_read (char* filename, matrix* mat);
matrix* mm_matrix_mult(matrix* mat1, matrix* mat2);


int mm_alloc(matrix* mat) {
/*
Notes:
  Allocate an array of pointers to a double
  Then, allocate an row/array of doubles and assign each double pointer that row
  Make sure you're allocating using mat->data in the matrix struct
  You can use calloc for allocation and zeroing out all the bytes
  - otherwise, you'd want to manually set all arrays to 0 (good practice)
  If your malloc/calloc returns null, the allocation failed
*/
    //ALLOCATES AN ARRAY OF POINTERS TO A DOUBLE
    mat->data = (double** )malloc(mat->rows*sizeof(double*));
    if (mat->data == NULL) {
        printf("mm_alloc: allocation failed.");
        exit(-1);
    }
        int i, x, y;
        for (i = 0; i < mat->rows; i++) {
            //ALLOCATES AN ARRAY OF DOUBLES TO EACH ROW
            mat->data[i] = (double* )malloc(mat->cols*sizeof(double));
            if (mat->data[i] == NULL) {
                printf("mm_alloc: allocation failed.");
                exit(-1);
            }
        }
                //ZEROES OUT EACH ARRAY
                for (x = 0; x < mat->rows; x++) {
                    for (y = 0; y < mat->cols; y++) {
                        mat->data[x][y] = 0.00;
                    }
                }
    return 0;
}

int mm_free(matrix* mat) {
/*
Notes:
  Reverse the allocation process, but you'll be calling free() instead
*/
    int i;
    for (i = 0; i < mat->rows; i++) {
        free(mat->data[i]);
    }
    free(mat->data);
    return 0;
}

int mm_print(matrix* mat) {
/*
Notes:
  Print all the data values in your matrix
  Go row by row
  At the end of each row, print a newline
  Between each data value, insert a tab
  Use 2 digits of precision after the decimal (use "%10.2lf\t" as format string)
  See output.txt for expected output formatting
*/
    printf("\n/******** START of 2-D Matrix **********************************/\n");
    int i, j;
    for (i = 0; i < mat->rows; i++) {
        for (j = 0; j < mat->cols; j++) {
            printf("%10.2lf\t", mat->data[i][j]);
            if (j == mat->cols - 1) {
                printf("\n");
            }
        }
    }
    printf("/******** END of 2-D Matrix ************************************/\n");
    return 0;
}

int mm_read(char* filename, matrix* mat) {
/*
Notes:
  Use fopen to open the matrix file, then use fgets or getline to read lines
  The first line of the matrix contains integers for "#rows #columns"
  - Use these to allocate your arrays for matrix data
  Each subsequent line contains a single data value
  - You can use sscanf to parse each matrix entry
  - First value is data[0][0], next is data[0][1] ... data[#rows-1][#columns-1]
*/
//OPENS FILE
    FILE *info = fopen(filename, "r");
    if (info == NULL) {
        printf("mm_read: failed to open file.\n");
        exit(-1);
    }
        char buffer[256];
        //READS THE FIRST LINE
        if (fgets(buffer, 256, info) == NULL) {
            printf("mm_read: failed to read from file.\n");
            exit(-1);
        }
            //STORE THE MATRIX DIMENSIONS
            if (2 != sscanf(buffer, "%d %d", &mat->rows, &mat->cols)) {
                printf("mm_read: failed to read matrix dimensions.\n");
                exit(-1);
            }
                //READ THE REMAINING LINES AND STORE THEM INTO THE MATRIX DATA ARRAY
                int i, j;
                int count = 0;
                mm_alloc(mat);
                while(fgets(buffer, 256, info) != NULL) {
                    for (i = 0; i < mat->rows; i++) {
                        for (j = 0; j < mat->cols; j++) {
                            double value = 0.0;
                            sscanf(buffer, "%lf", &value);
                            if (i == mat->rows || j == mat->cols) {
                                break;
                            }
                            mat->data[i][j] = value;
                            fgets(buffer, 256, info);
                            count++;
                        }
                    }
                }
                //CHECK THAT ENOUGH VALUES WERE READ INTO MAT->DATA
                if (count < mat->rows * mat->cols) {
                    printf("mm_read: failed to read matrix values.\n");
                    exit(-1);
                }
            fclose(info);
  return 0;
}

matrix* mm_matrix_mult(matrix* mat1, matrix* mat2)
{
/*
Notes:
  First check that it is actually possible to multiply the two inputs
  If so, construct a result matrix
  Then perform the multiplication
  Then return a pointer to your result matrix
*/
    matrix* result_matrix = (matrix* )malloc(sizeof(matrix));
    //CHECK IF ITS POSSIBLE TO MULTIPLY THE TWO MATRICES
    if (mat1->cols != mat2->rows) {
        printf("mm_matrix_mult: dimension mismatch between matrices.");
        exit(-1);
    }
        result_matrix->rows = mat1->rows;
        result_matrix->cols = mat2->cols;
        mm_alloc(result_matrix);
        int i,j;
        //MULTPLICATION FOR IF THE DIMENSIONS FOR BOTH MATRICES ARE THE SAME
        if (mat1->rows == mat2->cols) {
            for (i = 0; i < mat1->rows; i++) {
                for (j = 0; j < mat1->cols; j++) {
                    result_matrix->data[i][j] = mat1->data[i][j] * mat2->data[i][j];
                }
            }
        }
            int x,y,z;
            for (x = 0; x < result_matrix->rows; x++) {
                for (y = 0; y < result_matrix->cols; y++) {
                    double dot = 0.0;
                    for (z = 0; z < mat1->cols; z++) {
                        dot += mat1->data[x][z] * mat2->data[z][y];
                    }
                    result_matrix->data[x][y] = dot;
                }
            }
    return result_matrix;
}

int main()
{
  /*
   * you must keep this function AS IS.
   * you cannot modify it!
   */
  char filename_A[256];
  char filename_B[256];
  matrix* A = (matrix*)malloc(sizeof(matrix));
  matrix* B = (matrix*)malloc(sizeof(matrix));
  matrix* C = NULL;

  printf("Please enter the matrix A data file name: ");
  if (1 != scanf("%s", filename_A)) {
    printf("main: scanf failed to read filename\n");
    exit(-1);
  }
  mm_read(filename_A, A);
  mm_print(A);

  printf("Please enter the matrix B data file name: ");
  if (1 != scanf("%s", filename_B)) {
    printf("main: scanf failed to read filename\n");
    exit(-1);
  }
  mm_read(filename_B, B);
  mm_print(B);

  C = mm_matrix_mult(A, B);
  mm_print(C);

  mm_free(A);
  mm_free(B);
  mm_free(C);
  free(A);
  free(B);
  free(C);

  return 0;
}
