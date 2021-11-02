#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>
#include<math.h>
#include <stdbool.h>

#define MAX 10
#define MIN 0
#define DECIMAL 0
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

//Matrix multiplication of nXn matrix and nX1 matrix (School book approch)
//a is nXn matrix and b is nX1 matrix
//Used to check matrix multiplication is correct or not
static void MM_nn1(float *a, float *b, float *ab, int n){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            ab[i] += (a[i*n+j]*b[j]);
        }
    }

}

static void M_add(float *a,float *b, int n){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            a[i*n+j] = a[i*n+j] + b[i*n+j];
        }
    }
    return;
}

static void M_sub(float *a,float *b,float *c, int n){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            c[i*n+j] = a[i*n+j] - b[i*n+j];
        }
    }
    return;
}

//Check wheather matrix multiplication is correct or not
// if a*b = c then a*b - c = 0 hence a*b*check - c*check = 0
// dimension of check is nX1 and dimension of a,b is nXn
//hence a*b*check and and c*check can be done in n^2 using naive approach
// At last compare and return the result
static int MM_check(float *a, float *b, float *c, int n){
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
        if(abs(abCheck[i] - cCheck[i]) > 0.1 ){
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

//Matrix Multiplication with Integer entries 
static void MM_Integer(float *a, float *b, float *c, int n){
    ll *temp_a, *temp_b;

    temp_a = (ll *)malloc(n*sizeof(ll));
    temp_b = (ll *)malloc(n*sizeof(ll));

    //Initialise with zeros
    memset(temp_a,0,n*sizeof(ll));
    memset(temp_b,0,n*sizeof(ll));

    //mx is the max elements among both matrix
    ll mx=0;

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(a[i*n+j] > mx){
                mx = (int)a[i*n+j];
            }
            if(b[i*n+j] > mx){
                mx = (int)b[i*n+j];
            }
        }
    }

    //M is the number of digits in the max elements.
    int M = num_digits(mx);

    //P is the possible max number of digits in the answer matrix
    int P = num_digits(n*(pow(10,2*M)-1));
    // printf("p = %d\n",P);

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            temp_a[i] += ((int)a[i*n+j])*1ll*pow(10,(n-1-j)*P);
            temp_b[i] += ((int)b[j*n+i])*1ll*pow(10,j*P);
        }
    }

    // for(int i=0; i<n; i++){
    //     printf("%lld ",temp_a[i]);
    // }
    // printf("\n-------------\n");
    // for(int i=0; i<n; i++){
    //     printf("%lld ",temp_b[i]);
    // }
    // printf("\n");

    ll temp;

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            temp = (ll) ( (temp_a[i]*temp_b[j])/(pow(10,(n-1)*P)) ) % ((ll)pow(10,P));
            if(temp<0){
                temp += pow(10,P);
            }
            //Converting to the float and then storing the result into the answer matrix.
            c[i*n+j] = temp/(float)(pow(10,2*DECIMAL));
        }
    }

    free(temp_a);
    free(temp_b);
}

//Matrix Multiplication 
// c = a*b
// n is the number of rows and colmns in the matrix
static void MM(float *a, float *b, float *c, int n) {

    //converting the floats into integer
    //Here we are considering the DECIMAL digits afrer the decimal 
    //If DECIMAL=3 then we are considering only 3 digits after the decimal and ingnoring the other digits
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            a[i*n+j] = (int)(a[i*n+j]*pow(10,DECIMAL));
            b[i*n+j] = (int)(b[i*n+j]*pow(10,DECIMAL));
        }
    }

    int contd = 0;
    
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(a[i*n+j]<0 || b[i*n+j]<0){
                contd = 1;
                break;
            }
        }
    }

    if(contd==0){
        MM_Integer(a,b,c,n);
    }
    //Handling the for negative entry 
    else{
        float *a1,*a2,*b1,*b2;

        a1 = (float *)malloc(n*n*sizeof(float));
        a2 = (float *)malloc(n*n*sizeof(float));
        b1 = (float *)malloc(n*n*sizeof(float));
        b2 = (float *)malloc(n*n*sizeof(float));

        //Constucting a1,a2 and b1,b2 
        //Such that all elements of a1,a2,b1,b2 are non negative 
        //and a=a1-a2 and b=b1-b2
        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                if(a[i*n+j]<0){
                    a1[i*n+j] = 0;
                    a2[i*n+j] = abs(a[i*n+j]);
                }
                else{
                    a1[i*n+j] = a[i*n+j];
                    a2[i*n+j] = 0;
                }

                if(b[i*n+j]<0){
                    b1[i*n+j] = 0;
                    b2[i*n+j] = abs(b[i*n+j]);
                }
                else{
                    b1[i*n+j] = b[i*n+j];
                    b2[i*n+j] = 0;
                }
            }
        }

        float *a1b1, *a2b2, *a2b1, *a1b2;
        a1b1 = (float *)malloc(n*n*sizeof(float));
        a2b2 = (float *)malloc(n*n*sizeof(float));
        a2b1 = (float *)malloc(n*n*sizeof(float));
        a1b2 = (float *)malloc(n*n*sizeof(float));

        MM_Integer(a1,b1,a1b1,n);
        MM_Integer(a2,b2,a2b2,n);
        MM_Integer(a2,b1,a2b1,n);
        MM_Integer(a1,b2,a1b2,n);

        // a=a1-a2 and b=b1-b2
        // So, ab = (a1-a2)*(b1-b2)
        // ab = (a1*b1 + a2*b2) - (a2*b1 + a1*b2)
        M_add(a1b1,a2b2,n);
        M_add(a2b1,a1b2,n);
        M_sub(a1b1,a2b1,c,n);

    }

    

    
    //Converting the result into float
    // for(int i=0; i<n; i++){
    //     for(int j=0; j<n; j++){
    //         c[i*n+j] = c[i*n+j]/pow(10,2*DECIMAL);
    //     }
    // }

}

int main()
{
    struct timespec start,end;
    double runTime;
    int n;

    float *a,*b,*c;

    //Genereting the different size images and applying the compute algorithm
    for(int n=1;n<=4;n++){
        // n=(1<<i);
        //Allocating space for matrix a,b,c
        a = (float *)malloc(n*n*sizeof(float));
        b = (float *)malloc(n*n*sizeof(float));
        c = (float *)malloc(n*n*sizeof(float));

        // Generate random matrix a and b
        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                //This generates the random float number between MIN and MAX
                a[i*n + j] = ( (float)rand() / (float)(RAND_MAX) ) * (MAX - MIN) + MIN;
                b[i*n + j] = ( (float)rand() / (float)(RAND_MAX) ) * (MAX - MIN) + MIN;
            }
        }

        //Apply the algo on matrix a and b and note the time
        clock_gettime(CLOCK_MONOTONIC, &start);
        MM(a,b,c,n);
        clock_gettime(CLOCK_MONOTONIC, &end);

        //Converting to the float
        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                a[i*n + j] = a[i*n+j]/(float)(pow(10,DECIMAL));
                b[i*n + j] = b[i*n+j]/(float)(pow(10,DECIMAL));
            }
        }

    
        // for(int i=0; i<n; i++){
        //     for(int j=0; j<n; j++){
        //         printf("%0.2f ",a[i*n + j]);
        //     }
        //     printf("\n");
        // }
        // printf("\n");

        // for(int i=0; i<n; i++){
        //     for(int j=0; j<n; j++){
        //         printf("%0.2f ",b[i*n + j]);
        //     }
        //     printf("\n");
        // }
        // printf("\n");
        // printf("\n");
        
        // // Print the Result
        // for(int i=0; i<n; i++){
        //     for(int j=0; j<n; j++){
        //         printf("%0.2f ",c[i*n + j]);
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
        printf("Size = %d,  RunTime = %lf\n",n,runTime);

        free(a);
        free(b);
        free(c);
    }
    return 0;
}