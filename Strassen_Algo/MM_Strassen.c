#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>
#include<math.h>
#include <stdbool.h>

#define MAX 10
#define MIN 0
//For bigger size matrix value of epsilon should be big (2 ,3 ..)
//For integr entry only epsilon can be zero
#define EPSILON 0


float **allocate_matrix(int m, int n){
    float **matrix = (float **)malloc(m * sizeof(float *)); 
    for (int i = 0; i < m; i++) {
        matrix[i] = (float *)malloc(n * sizeof(float));
    }

    return matrix;
}
void free_matrix(float **mat, int m){
    for (int i = 0; i < m; i++) {
         free(mat[i]);
    }
    free(mat);
}

//Matrix multiplication of nXn matrix and nX1 matrix (School book approch)
//a is nXn matrix and b is nX1 matrix
//Used to check matrix multiplication is correct or not
static void MM_nn1(float **a, float *b, float *ab, int n){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            ab[i] += (a[i][j]*b[j]);
        }
    }

}

//Check wheather matrix multiplication is correct or not
// if a*b = c then a*b - c = 0 hence a*b*check - c*check = 0
// dimension of check is nX1 and dimension of a,b is nXn
//hence a*b*check and and c*check can be calculated in n^2 using naive approach
// At last compare and return the result
static int MM_check(float **a, float **b, float **c, int n){
    float *check, *bCheck, *cCheck, *abCheck;

    check = (float *)malloc(n*sizeof(float));

    for(int i=0; i<n; i++){
         check[i] = rand()%2;
    }

    bCheck = (float *)malloc(n*sizeof(float));
    memset(bCheck,0,n*sizeof(float));
    //Naive matrix multiplication of b and check
    MM_nn1(b,check,bCheck,n);

    cCheck = (float *)malloc(n*sizeof(float));
    memset(cCheck,0,n*sizeof(float));
    //Naive matrix multiplication of c and check
    MM_nn1(c,check,cCheck,n);

    abCheck = (float *)malloc(n*sizeof(float));
    memset(abCheck,0,n*sizeof(float));
    //Naive matrix multiplication of a and bCheck
    MM_nn1(a,bCheck,abCheck,n);

    int answer=1;

    //Comparing the result
    for(int i=0; i<n; i++){
        if(abs(abCheck[i] - cCheck[i]) > EPSILON ){
            answer=0;
            break;
        }
    }

    free(check);
    free(bCheck);
    free(cCheck);
    free(abCheck);

    return answer;

}


void MM_add(float **a, float **b, float **c, int N){
     for(int i = 0; i < N; ++i){
         for(int j = 0; j < N; ++j){
            c[i][j] = a[i][j] + b[i][j];
         }
     }
     return;
}

void MM_sub(float **a, float **b, float **c, int N){
     for(int i = 0; i < N; ++i){
         for(int j = 0; j < N; ++j){
             c[i][j] = a[i][j] - b[i][j];
         }
     }
     return;
}

void divide(float **a, float **a11, float **a12, float **a21, float **a22, int K){
    for(int i = 0; i < K; ++i){
        for(int j = 0; j < K; ++j){
            a11[i][j] = a[i][j];
            a12[i][j] = a[i][K+j];
            a21[i][j] = a[K+i][j];
            a22[i][j] = a[K+i][K+j];
        }
    }
}

void join(float **a, float **a11, float **a12, float **a21, float **a22, int K){
     for(int i=0; i<K; i++){
        for(int j=0; j<K; j++) {
            a[i][j] = a11[i][j];
            a[i][j+K] = a12[i][j];
            a[K+i][j] = a21[i][j];
            a[K+i][K+j] = a22[i][j];
        }
    }
}


void MM_Strassen(float **A, float **B, float **C, int N){
    //Base case
    if(N==1){
        C[0][0] = A[0][0]*B[0][0];
        return;
    }

    int K = N/2;

    //Creating 4 sub-matrix(a11,a12,a21,a22) from a 
    float **A11 = allocate_matrix(K,K);
    float **A12 = allocate_matrix(K,K);
    float **A21 = allocate_matrix(K,K);
    float **A22 = allocate_matrix(K,K);

    divide(A,A11,A12,A21,A22,K);

    //Creating 4 sub-matrix(b11,b12,b21,b22) from b.
    float **B11 = allocate_matrix(K,K);
    float **B12 = allocate_matrix(K,K);
    float **B21 = allocate_matrix(K,K);
    float **B22 = allocate_matrix(K,K);

    divide(B,B11,B12,B21,B22,K);

    //Allocating space for 7 strassen product(p1,p2,.....,p7)
    float **P1 = allocate_matrix(K,K);
    float **P2 = allocate_matrix(K,K);
    float **P3 = allocate_matrix(K,K);
    float **P4 = allocate_matrix(K,K);
    float **P5 = allocate_matrix(K,K);
    float **P6 = allocate_matrix(K,K);
    float **P7 = allocate_matrix(K,K);

    //Creating temp to store the add or sub result temporarily
    float **temp1 = allocate_matrix(K,K);
    float **temp2 = allocate_matrix(K,K);

    //P1 = A11*(B12-B22)
    MM_sub(B12,B22,temp1,K);
    MM_Strassen(A11, temp1,P1, K);

    //P2 = (A11+A12)*B22
    MM_add(A11,A12,temp1,K);
    MM_Strassen(temp1, B22,P2, K);

    //P3 = (A21+A22)*B11
    MM_add(A21,A22,temp1,K);
    MM_Strassen(temp1, B11,P3, K);

    //P4 = A22*(B21-B11)
    MM_sub(B21,B11,temp1,K);
    MM_Strassen(A22, temp1,P4, K);

    //P5 = (A11+A22)*(B11+B22)
    MM_add(A11,A22,temp1,K);
    MM_add(B11,B22,temp2,K);
    MM_Strassen(temp1, temp2,P5, K);

    //P6 = (A12-A22)*(B21+B22)
    MM_add(B21,B22,temp1,K);
    MM_sub(A12,A22,temp2,K);
    MM_Strassen(temp2, temp1,P6, K);

    //P7 = (A11-A21)*(B11+B12)
    MM_add(B11,B12,temp1,K);
    MM_sub(A11,A21,temp2,K);
    MM_Strassen(temp2, temp1,P7, K);

    //Allocating space for 4 sub-matrix(c11,c12,c21,c22) to store the respective result
    //Togather this 4 sub-matrix create matrix C (which is c=a*b)
    float **C11 = allocate_matrix(K,K);
    float **C12 = allocate_matrix(K,K);
    float **C21 = allocate_matrix(K,K);
    float **C22 = allocate_matrix(K,K);

    //c11 = p4+p5+p6-p2
    MM_add(P4,P5,temp1,K);
    MM_add(temp1,P6,temp2,K);
    MM_sub(temp2,P2,C11,K);

    //c12 = p1+p2
    MM_add(P1, P2,C12, K);

    //c21 = p3+p4
    MM_add(P3, P4,C21 ,K);

    //c22 = p1+p5-p3-p7
    MM_add(P1,P5,temp1,K);
    MM_sub(temp1,P3,temp2,K);
    MM_sub(temp2,P7,C22,K);

    //Combine c11,c12,c21,c22 matrix into c
    //Here row width of sub-matrix(c11,c12,c21,c22) is k and row width of matrix c is n.
    join(C,C11,C12,C21,C22,K);
   
    //Free the allocated space
    free_matrix(A11,K);
    free_matrix(A12,K);
    free_matrix(A21,K);
    free_matrix(A22,K);

    free_matrix(B11,K);
    free_matrix(B12,K);
    free_matrix(B21,K);
    free_matrix(B22,K);

    free_matrix(P1,K);
    free_matrix(P2,K);
    free_matrix(P3,K);
    free_matrix(P4,K);
    free_matrix(P5,K);
    free_matrix(P6,K);
    free_matrix(P7,K);

    free_matrix(C11,K);
    free_matrix(C12,K);
    free_matrix(C21,K);
    free_matrix(C22,K);

    free_matrix(temp1,K);
    free_matrix(temp2,K);

    return;
}

int main(void){
    struct timespec start,end;
    double runTime;
    int n;

    for(int i=1;i<10;i++){
         n=(1<<i);
        // n=4;

        float **a = allocate_matrix(n,n);
        float **b = allocate_matrix(n,n);
        float **c = allocate_matrix(n,n);

        
        // Generate random matrix a and b
        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                //This generates the random float number between MIN and MAX
                // a[i][j] = ( (float)rand() / (float)(RAND_MAX) ) * (MAX - MIN) + MIN;
                // b[i][j] = ( (float)rand() / (float)(RAND_MAX) ) * (MAX - MIN) + MIN;

                //This generates random int number 
                a[i][j] = rand();
                b[i][j] = rand();
            }
        }
        
        // Print Generated Matrix
        // for(int i=0; i<n; i++){
        //     for(int j=0; j<n; j++){
        //         printf("%0.2f ",a[i][j]);
        //     }
        //     printf("\n");
        // }
        // printf("\n");
        // for(int i=0; i<n; i++){
        //     for(int j=0; j<n; j++){
        //         printf("%0.2f ",b[i][j]);
        //     }
        //     printf("\n");
        // }
        // printf("\n");

        //Apply the algo on matrix a and b and note the time
        clock_gettime(CLOCK_MONOTONIC, &start);
        MM_Strassen(a,b,c,n);
        clock_gettime(CLOCK_MONOTONIC, &end);

        // Print the Result
        // for(int i=0; i<n; i++){
        //     for(int j=0; j<n; j++){
        //         printf("%0.2f ",c[i][j]);
        //     }
        //     printf("\n");
        // }
        // printf("\n");

        char result[2][10] = {{"Wrong"}, {"Correct"}};
        //Printint the result
        printf("Output is %s \n",result[MM_check(a,b,c,n)]);

        //Calculating the runtime of algorithm.
        runTime = (end.tv_sec - start.tv_sec) * 1e9; 
        runTime = (runTime + (end.tv_nsec - start.tv_nsec)) * 1e-9; 
        printf("Size = %d,  RunTime = %lf\n",i,runTime);
    }

    

}