#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>
#include<math.h>
#include <stdbool.h>

#define MAX 10
#define MIN 1
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

//Here a and b are string of number
//This function return the multiplication string(char array)
//Returns a*b
char *multiply(char s1[], char s2[]){
	int a[200],b[200];
    int ans[400]={0};
    int i,j,tmp;
    
    int l1 = strlen(s1);
    int l2 = strlen(s2);
    for(i = l1-1,j=0;i>=0;i--,j++)
    {
        a[j] = s1[i]-'0';
    }
    for(i = l2-1,j=0;i>=0;i--,j++)
    {
        b[j] = s2[i]-'0';
    }
    for(i = 0;i < l2;i++)
    {
        for(j = 0;j < l1;j++)
        {
            ans[i+j] += b[i]*a[j];
        }
    }
    for(i = 0;i < l1+l2;i++)
    {
        tmp = ans[i]/10;
        ans[i] = ans[i]%10;
        ans[i+1] = ans[i+1] + tmp;
    }
    for(i = l1+l2; i>= 0;i--)
    {
        if(ans[i] > 0)
            break;
    }
    char *mul=(char *)malloc(200*sizeof(char));
    int index =0;
    for(;i >= 0;i--)
    {
    	mul[index++] = ans[i] + '0';
    }
    mul[index] = '\0';
    return mul;
}



//Matrix Multiplication with Integer entries
//Calculate c = a*b 
static void MM_Integer(int *a, int *b, int *c, int n){

    //mx is the max elements among both matrix
    int mx=0;

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

    char * temp_a[n];
    char * temp_b[n];

    //Now we are calculating the following mathematical product using sting because number is too large to stor in long long int 
    // for(i=0 to n-1) => for(j=0 to n-1)=> temp_a[i] += ((int)a[i*n+j])*1ll*pow(10,(n-1-j)*P); 
    //There is a pattern in above product so, we are using that pattern
    int starting=0;
    for(int i=0; i<n; i++){
        char* arr;
        starting=0;
        arr = (char*)malloc((n+1)*P);
        
        for(int j=0; j<n; j++){
            ll num = a[i*n+j];
            int digit = num_digits(num);
            char arr1[digit];
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
        temp_a[i] = arr;
    }

    //Now we are calculating the following mathematical product using sting because number is too large to stor in long long int 
    // for(i=0 to n-1) => for(j=0 to n-1)=> temp_b[i] += ((int)b[j*n+i])*1ll*pow(10,j*P);
    //There is a pattern in above product so, we are using that pattern
    for(int i=0; i<n; i++){
        char* b_arr;
        starting=0;
        b_arr = (char*)malloc((n+1)*P);
        
        for(int j=0; j<n; j++){
            ll num = b[(n-1-j)*n+i];
            int digit = num_digits(num);
            char b_arr1[digit];
            if(j!=0){
                for(int z=0;z<(P-digit);z++){
                    b_arr[starting+z] = '0';
                }
                starting += (P-digit);
            }
            
                int index = 0;
                while (num) {
                    // Separate last digit from
                    // the number and add ASCII
                    // value of character '0' is 48
                    b_arr1[++index] = num % 10 + '0';
 
                    // Truncate the last
                    // digit from the number
                    num /= 10;
                }
                // Reverse the array for result
                for (int z = 0; z < index; z++) {
                    b_arr[starting+z] = b_arr1[index - z];
                }
                starting += digit;
            

            // temp_a[i] += ((int)a[i*n+j])*1ll*pow(10,(n-1-j)*P);
            // temp_b[i] += ((int)b[j*n+i])*1ll*pow(10,j*P);
        }
        b_arr[starting] = '\0';
        temp_b[i] = b_arr;
    }

    // for(int i=0; i<n;i++){
    //     printf("%s  ",temp_a[i]);
    // }
    // printf("\n");
    // for(int i=0; i<n;i++){
    //     printf("%s  ",temp_b[i]);
    // }
    // printf("\n");

    char temp1[300],temp2[300];
    char *temp;
    char *ans;
    char *end;
    int char_len;

    
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            // c[i*n+j] = ( (temp_a[i]*temp_b[j])/(pow(10,(n-1)*P)) ) % ((ll)pow(10,P))
            // again number is too big hence we used user defined multiplication algo to multiply two big number as char array
            sprintf(temp1, "%s", temp_a[i]);
            sprintf(temp2, "%s", temp_b[j]);
            temp = multiply(temp1,temp2);
            // printf("mul = %s \n",temp);
            char_len = strlen(temp);
            // printf("%d   %d\n",char_len-n*P,P);
            ans = (temp+(char_len-n*P));
            ans[P] = '\0';
            // printf("anwser = %s \n",ans);

            //finally converting the char* into the interger number and storing into the answer matrix.
            c[i*n+j] = strtol(ans, &end, 10);
        }
    }
}

//School book method of MM.
static void MM_naive(int *a, int *b, int *c, int n) {
    int temp = 0;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            temp = 0; 
            for(int k=0; k<n; k++){
                temp += a[i*n + k]*b[k*n + j];
            }
            c[i*n + j] = temp;
        }
    }
}

int main()
{
    struct timespec start,end;
    double runTime;
    int n;

    int *a,*b,*c,*c1;

    //Genereting the different size images and applying the compute algorithm
    for(int n=2;n<=20;n++){
        // n=(1<<i);
        //Allocating space for matrix a,b,c
        a = (int *)malloc(n*n*sizeof(int));
        b = (int *)malloc(n*n*sizeof(int));
        c = (int *)malloc(n*n*sizeof(int));
        c1 = (int *)malloc(n*n*sizeof(int));


        // Generate random matrix a and b
        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                //This generates the random integer number between MIN and MAX
                a[i*n + j] = (int) (( rand() /(float) (RAND_MAX) ) * (MAX - MIN) + MIN);
                b[i*n + j] =  (int)(( rand() /(float) (RAND_MAX) ) * (MAX - MIN) + MIN);
            }
        }

        //Apply the algo on matrix a and b and note the time
        clock_gettime(CLOCK_MONOTONIC, &start);
        MM_Integer(a,b,c,n);
        clock_gettime(CLOCK_MONOTONIC, &end);

        MM_naive(a,b,c1,n);
    
        // for(int i=0; i<n; i++){
        //     for(int j=0; j<n; j++){
        //         printf("%d ",a[i*n + j]);
        //     }
        //     printf("\n");
        // }
        // printf("\n");

        // for(int i=0; i<n; i++){
        //     for(int j=0; j<n; j++){
        //         printf("%d ",b[i*n + j]);
        //     }
        //     printf("\n");
        // }
        // printf("\n");
        // printf("\n");
        
        // Print the Result
        // for(int i=0; i<n; i++){
        //     for(int j=0; j<n; j++){
        //         printf("%0.2f ",c[i*n + j]);
        //     }
        //     printf("\n");
        // }
        // printf("\n");

        //Comparing the results to check where matrix multiplication is true or not
        int flag =1;
         for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                if(c[i*n+j]!=c1[i*n+j]){
                    flag = 0;
                    break;
                }
            }
        }

        char result[2][10] = {{"Wrong"}, {"Correct"}};
        //Printint the result
        printf("Output is %s \n",result[flag]);

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