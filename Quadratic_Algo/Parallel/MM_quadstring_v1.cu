#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>
#include<math.h>
#include <stdbool.h>
#include <sys/resource.h>
#include <errno.h>
#include <unistd.h>
#include <cuda.h>

#define BLOCK_SIZE 32
#define MAX 10
#define MIN 1
#define ll long long int


//Return the number of digits in the num
int num_digits(ll num){
    int len = 0;
    int n = (int)num;
    while(n > 0){
        len++;
        n = n/10;
    }
    return len;
}


//N is the number of rows
__global__ void matrix_simp1(int *a, char* temp_a, int M, int P, int N) {         
    int starting =0;

    int idx =  blockDim.x * blockIdx.x + threadIdx.x;
    
    char arr[10];
    if (idx < N) {
        for(int j=0; j<N; j++){
            ll num = a[idx*N+j];

            //Calculating length of a num
            int len = 0;
            int no = (int)num;
            while(no > 0){
                len++;
                no = no/10;
            }
            int digit = len;

            
            // char* arr1;
            char arr1[5];
            // cudaMalloc((void**)&arr1, digit*sizeof(char));
            if(j!=0){
                for(int z=0;z<(P-digit);z++){
                    arr[starting+z] = '0';
                }
                starting += (P-digit);
            }
            int index = 0;
            while (num) {
                // Separate last digit from
                // the number and add ASCII
                // value of character '0' is 48
                arr1[++index] = num % 10 + '0';

                // Truncate the last
                // digit from the number
                num /= 10;
            }
            // Reverse the array for result
            for (int z = 0; z < index; z++) {
                arr[starting+z] = arr1[index - z];
            }
            starting += digit;
        }
        arr[starting] = '\0';
        printf("arr: %s\n",arr);
        for(int i=0;i<starting;i++){
                temp_a[idx*(N+1)*P+i] = arr[i];
        }
        
    }

    // free(arr);

    return;
}

__global__ void matrix_simp2(int *b, char* temp_b, int M, int P, int N) { 
    int starting =0;

    int idx =  blockDim.x * blockIdx.x + threadIdx.x;
    
    char arr[10];
    if (idx < N) {

        for(int j=0; j<N; j++){
            ll num = b[(N-1-j)*N+idx];

            //Calculating length of a num
            int len = 0;
            int no = (int)num;
            while(no > 0){
                len++;
                no = no/10;
            }
            int digit = len;

            
            // char* arr1;
            char arr1[5];
            // cudaMalloc((void**)&arr1, digit*sizeof(char));
            if(j!=0){
                for(int z=0;z<(P-digit);z++){
                    arr[starting+z] = '0';
                }
                starting += (P-digit);
            }
            int index = 0;
            while (num) {
                // Separate last digit from
                // the number and add ASCII
                // value of character '0' is 48
                arr1[++index] = num % 10 + '0';

                // Truncate the last
                // digit from the number
                num /= 10;
            }
            // Reverse the array for result
            for (int z = 0; z < index; z++) {
                arr[starting+z] = arr1[index - z];
            }
            starting += digit;
        }
        arr[starting] = '\0';
        printf("arr: %s\n",arr);
        for(int i=0;i<starting;i++){
            // if(arr[i]!='\0')
                temp_b[idx*(N+1)*P+i] = arr[i];  
        }
        
    }

    return;
}

__global__ void matrix_mul(char* temp_a, char* temp_b, int* temp_c, int P, int N){
    //rows
    int idx_i = blockDim.x * blockIdx.x + threadIdx.x; 
    //cols
    int idx_j = blockDim.y * blockIdx.y + threadIdx.y; 

    int num_size= (N+1)*P;

    if(idx_i < N && idx_j < N){
        //find the last index of the number
        int l1=0,l2=0;
        for(int i=0;i<num_size;i++){
            if(temp_a[idx_i*num_size + i]=='\u0000') break;
            else l1++;
        }
        for(int i=0;i<num_size;i++){
            if(temp_b[idx_j*num_size + i]=='\u0000') break;
            else l2++;
        }
        
        printf("%d %d %d %d\n",idx_i,idx_j,l1,l2);

        for(int i=0;i<l2;i++){
            for(int j=0;j<l1;j++){
                //a will start from ones place, ||ly b
                int index1,index2;
                index1 = idx_i*num_size + l1 - j - 1;
                index2 = idx_j*num_size + l2 - i - 1;
                int a = temp_a[index1] - '0';
                int b = temp_b[index2] - '0';
                // printf("\na: %d , b: %d",a,b);   
                temp_c[(idx_i*N + idx_j)*(2*num_size) + i+j] += a*b;
            }
        }
        //we will handle carry overs in the CPU
    }
}
int main(){

    // struct timespec start,end;
    // double runTime;
    int n=3;
    // int N=n*n;

    int *a,*b,*c,*c1;

    struct rusage r_usage;

    //Allocating space for matrix a,b,c
    a = (int *)malloc(n*n*sizeof(int));
    b = (int *)malloc(n*n*sizeof(int));
    c = (int *)malloc(n*n*sizeof(int));
    // c1 = (int *)malloc(n*n*sizeof(int));


    int mx=0;
    // Generate random matrix a and b
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            //This generates the random integer number between MIN and MAX
            a[i*n + j] = (int) (( rand() /(float) (RAND_MAX) ) * (MAX - MIN) + MIN);
            b[i*n + j] =  (int)(( rand() /(float) (RAND_MAX) ) * (MAX - MIN) + MIN);

            if(mx<a[i*n+j]) mx=a[i*n+j];
            if(mx<b[i*n+j]) mx=b[i*n+j];

            printf("%d ", a[i*n+j]);
        }
        printf("\n");
    }
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            printf("%d ", b[i*n+j]);
        }
        printf("\n");
    }

    //M is the number of digits in the max elements.
    int M = num_digits(mx);
    
    //P is the possible max number of digits in the answer matrix
    int P = num_digits(n*(pow(10,2*M)-1));

    //array of string = temp_a
    char* temp_a = (char*)malloc(n*(n+1)*P*sizeof(char)); 
    char* temp_b = (char*)malloc(n*(n+1)*P*sizeof(char)); 
    int* temp_c = (int*)malloc(2*n*n*(n+1)*P*sizeof(int)); 


    //device variables
    int* d_a;
    char* d_temp_a;
    int* d_b;
    char* d_temp_b;
    int *d_temp_c;
  


    // Alloc space for device copies
    cudaMalloc((void**)&d_a, n*n*sizeof(int));
    cudaMalloc((void**)&d_temp_a,n*(n+1)*P*sizeof(char));
    cudaMalloc((void**)&d_b, n*n*sizeof(int));
    cudaMalloc((void**)&d_temp_b,n*(n+1)*P*sizeof(char));
    cudaMalloc((void**)&d_temp_c,2*n*n*(n+1)*P*sizeof(int));

    

    // Copy to device
    cudaMemcpy(d_a, a, n*n*sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, n*n*sizeof(int), cudaMemcpyHostToDevice);


    printf("Before Launching the kernel:\n");
    // Launch kernel on GPU
    // int ceilB = (n+BLOCK_SIZE-1)/BLOCK_SIZE;
    // ceilB, BLOCK_SIZE
    matrix_simp1<<<1,n>>>(d_a, d_temp_a, M,P,n);
    matrix_simp2<<<1,n>>>(d_b, d_temp_b, M,P,n);
    
    // Copy result back to host
    cudaMemcpy(temp_a, d_temp_a, n*(n+1)*P*sizeof(char), cudaMemcpyDeviceToHost);
    cudaMemcpy(temp_b, d_temp_b, n*(n+1)*P*sizeof(char), cudaMemcpyDeviceToHost);
    
    
    // Print result
    printf("temp_a : ");
    // int* arr1 = (int*)malloc
    for(int i=0;i<n*(n+1)*P;i++){
        // if(temp_a[i] !='\u0000')
        printf("%c ", temp_a[i]);
    }
    
    printf("\n");
    
    printf("temp_b : ");
    for(int i=0;i<n*(n+1)*P;i++){
        printf("%c ", temp_b[i]);
    }
    
    printf("\n");
    
    /********************MULTIPLY************************/
    dim3 gridShape = dim3(1,1);
    dim3 blockShape = dim3(n,n);
    matrix_mul<<<gridShape,blockShape>>>(d_temp_a,d_temp_b,d_temp_c,P,n);
    printf("GPU process completed:\n");
    
    cudaMemcpy(temp_c, d_temp_c, 2*n*n*(n+1)*P*sizeof(int), cudaMemcpyDeviceToHost);
    
    printf("\ntemp_c : ");
    for(int i=0;i<2*n*n*(n+1)*P;i++){
        printf("%d ", temp_c[i]);
    }

    // Cleanup
    free(a);
    free(b); 
    free(temp_a);
    free(temp_b);
    free(temp_c);

    cudaFree(d_a); 
    cudaFree(d_temp_a);
    cudaFree(d_b);
    cudaFree(d_temp_b);
    cudaFree(d_temp_c);


    return 0;
}

// 9 8 7 6 5 4 3 2 1
// 9 6 3 8 5 2 7 4 1

// 1 2 3
// 4 5 6
// 7 8 9

// a= \u0000

/********
A
8 8 9 
4 3 5 
4 9 6
B
4 8 2 
7 5 6 
5 9 7
arr: 8008009
arr: 4003005
arr: 4009006
arr: 5007004
arr: 9005008
arr: 7006002

4,00,96,13,30,95,036    7,21,12,18,51,09,072    
2,00,43,06,20,47,020

40 0 0 96 0 0 133 0 0 95 0 0 36 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 72 0 0 112 0 0 185 0 0 109 0 0 72 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
56 0 0 104 0 0 127 0 0 70 0 0 38 0 0 43 0 0 62 0 0 47 0 0 20 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 36 0 0 47 0 0 92 0 0 49 0 0 40 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 28 0 0 45 0 0 61 0 0 36 0 0 30 0 0 73 0 0 109 0 0 78 0 0 24 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 36 0 0 101 0 0 131 0 0 102 0 0

*/