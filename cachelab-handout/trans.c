// name : Kim Dong Seong
// ID : cs20150112

/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{	
	int i, j, tmp, diagonal;
	int ii, jj;
	int iii, jjj;
	int b, bb;
	
	if(M == 32){
		b = 256 / M;
	
		for (i = 0; i < N ; i += b){
			for (j = 0 ; j < M ; j += b){
				for (ii = i; ii < i + b ; ii++){
					for (jj = j; jj < j + b ; jj++){
						if (ii == jj){
							diagonal = A[ii][ii];
							continue;
						}
						else{
							tmp = A[ii][jj];
							B[jj][ii] = tmp;
						}
					}
					if (i == j){
//						tmp = A[ii][ii];
						B[ii][ii] = diagonal;
					}
				}
			}
		}
	}
	else if (M == 64){
		b = 8;
		bb = 4;

		for (i = 0; i < N ; i += b){
			for (j = 0 ; j < M ; j += b){
				if (i != j){
					for (ii = i ; ii < i + b; ii += bb){
						for (jj = j ; jj < j + b ; jj += bb){
							for (iii = ii ; iii < ii + bb ; iii++){
								for (jjj = jj ; jjj < jj + bb ; jjj++){
									tmp = A[iii][jjj];
									B[jjj][iii] = tmp;
								}
							}
						}
					}
				}
			}
			for (jj = i ; jj < i + b ; jj += bb){
				for(ii = i ; ii < i + b ; ii += bb){
					for (iii = ii ; iii < ii + bb ; iii++){
						for (jjj = jj ; jjj < jj + bb ; jjj++){
							if (iii == jjj){
								diagonal = A[iii][iii];
								continue;
							}
							else{
								tmp = A[iii][jjj];
								B[jjj][iii] = tmp;
							}
						}
						if (ii == jj)
							B[iii][iii] = diagonal;
					}
				}
			}
		}
	}
	else if(M == 61){
//		b = 4;
//		b = 8;
		b = 16;
//		b = 32;

		for (i = 0; i < N; i+=b){
			for (j = 0; j < M ; j += b){
				for (ii = i; ii < i + b ; ii++){
					if (ii >= N)
						break;
					for (jj = j; jj < j + b; jj++){
						if (jj >= M)
							break;
						else if(ii == jj){
							diagonal = A[ii][ii];
							continue;
						}
						else{
							tmp = A[ii][jj];
							B[jj][ii] = tmp;
						}
					}
					if (i == j){
//						tmp = A[ii][ii];
						B[ii][ii] = diagonal;
					}
				}
			}
		}
	}
	
 
}

/* 
 * You can define additional transp se functi ns below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

