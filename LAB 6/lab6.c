/*
    Write a Multithreaded C program which can simultaneously in (different threads) perform following Matrix Operations: 
    a) Addition, 
    b) Subtraction, 
    c) Multiplication, 
    d) Trace of a matrix (Sum of all diagonal elements only), 
    e) Check AB = BA
*/

#include <stdio.h>
#include <stdlib.h>
#include<malloc.h>
#include<pthread.h>



/*
    Declaring a structure to pass multiple arguments to the thread function
*/


struct params{
    int** A;
    int** B;
    int n1;
    int m1;
    int n2;
    int m2;
};


/*
    Function to add two matrices
    @param: struct params* p
    @return: void*
*/

void* add(void*p1){
    struct params *p = (struct params *)p1;
    int** A = p->A;
    int** B = p->B;
    int n1 = p->n1;
    int m1 = p->m1;
    int n2 = p->n2;
    int m2 = p->m2;
    if(n1!=n2 || m1!=m2){
        printf("\nAddition not possible!\n");
        pthread_exit(NULL);
    }
    int** C = (int**)malloc(n1*sizeof(int*));
    for(int i=0; i<n1; i++){
        C[i] = (int*)malloc(m1*sizeof(int));
    }
    for(int i=0; i<n1; i++){
        for(int j=0; j<m1; j++){
            C[i][j] = A[i][j] + B[i][j];
        }
    }
    pthread_exit(C);
}

void* sub(void*p1){
    struct params *p = (struct params *)p1;
    int** A = p->A;
    int** B = p->B;
    int n1 = p->n1;
    int m1 = p->m1;
    int n2 = p->n2;
    int m2 = p->m2;
    if(n1!=n2 || m1!=m2){
        printf("\nSubtraction not possible!\n");
        pthread_exit(NULL);
    }
    int** C = (int**)malloc(n1*sizeof(int*));
    for(int i=0; i<n1; i++){
        C[i] = (int*)malloc(m1*sizeof(int));
    }
    for(int i=0; i<n1; i++){
        for(int j=0; j<m1; j++){
            C[i][j] = A[i][j] - B[i][j];
        }
    }
    pthread_exit(C);
}

void* mul(void*p1){
    struct params *p = (struct params *)p1;
    int** A = p->A;
    int** B = p->B;
    int n1 = p->n1;
    int m1 = p->m1;
    int n2 = p->n2;
    int m2 = p->m2;
    if(m1!=n2){
        printf("\nMultiplication not possible!\n");
        pthread_exit(NULL);
    }
    int** C = (int**)malloc(n1*sizeof(int*));
    for(int i=0; i<n1; i++){
        C[i] = (int*)malloc(m2*sizeof(int));
    }
    for(int i=0;i<n1;i++){
        for(int j=0;j<m2;j++){
            C[i][j] = 0;
            for(int k=0;k<m1;k++){
                C[i][j]+=A[i][k]*B[k][j];
            }
        }
    }
    pthread_exit(C);
}

void* trace(void*p1){
    struct params *p = (struct params *)p1;
    int** A = p->A;
    int n1 = p->n1;
    int m1 = p->m1;
    int** B = p->B;
    int n2 = p->n2;
    int m2 = p->m2;
    int sum = 0;
    if(n1!=m1){
        printf("\nTrace not possible!\n");
    }
    else{
        for(int i=0;i<n1;i++){
            sum+=A[i][i];
        }
    }
    printf("\nTrace of the A is: %d\n", sum);
    if(n2!=m2){
        printf("\nTrace not possible!\n");
        pthread_exit(NULL);
    }
    sum=0;
    for(int i=0;i<n2;i++){
        sum+=B[i][i];
    }
    printf("\nTrace of the B is: %d\n", sum);
    pthread_exit(NULL);
}

void* check(void*p2){
    struct params *p = (struct params *)p2;
    int** A = p->A;
    int** B = p->B;
    int n1 = p->n1;
    int m1 = p->m1;
    int n2 = p->n2;
    int m2 = p->m2;

    if(m1!=n2 || m2!=n1){
        printf("\nMultiplication not possible!\n");
        pthread_exit(NULL);
    }
    int** C = (int**)malloc(n1*sizeof(int*));
    for(int i=0; i<n1; i++){
        C[i] = (int*)malloc(m2*sizeof(int));
    }
    int** D = (int**)malloc(n2*sizeof(int*));
    for(int i=0; i<n2; i++){
        D[i] = (int*)malloc(m1*sizeof(int));
    }
    pthread_t tid1;
    pthread_t tid2;
    struct params* p1 = (struct params*)malloc(sizeof(struct params));
    p1->A = B;
    p1->B = A;
    p1->n1 = n2;
    p1->m1 = m2;
    p1->n2 = n1;
    p1->m2 = m1;
    pthread_create(&tid1, NULL, mul, p);
    pthread_create(&tid2, NULL, mul, p1);
    pthread_join(tid1, (void**)&C);
    pthread_join(tid2, (void**)&D);
    int flag = 1;

    for(int i=0;i<n1;i++){
        for(int j=0;j<m2;j++){
            if(C[i][j]!=D[i][j]){
                flag = 0;
                break;
            }
        }
    }
    if(flag){
        printf("\nThe matrices are commute!\n");
    }
    else{
        printf("\nThe matrices are not commute!\n");
    }
    pthread_exit(NULL);
}

int main(){

    /* Input region */
    /*---------------------------------------------------------------------------------------------------------*/
    int n1, m1;
    int n2,m2;
    printf("\nEnter the dimensions of matrix 1: ");
    scanf("%d %d", &n1, &m1);
    printf("\nEnter the dimensions of matrix 2: ");
    scanf("%d %d", &n2, &m2);
    int** A = (int**)malloc(n1*sizeof(int*));
    for(int i=0; i<n1; i++){
        A[i] = (int*)malloc(m1*sizeof(int));
    }
    int** B = (int**)malloc(n2*sizeof(int*));
    for(int i=0; i<n2; i++){
        B[i] = (int*)malloc(m2*sizeof(int));
    }
    printf("\nEnter the elements of matrix 1: \n");
    for(int i=0;i<n1;i++){
        for(int j=0;j<m1;j++){
            printf("\nEnter value of A[%d][%d]: ", i, j);
            scanf("%d", &A[i][j]);
        }
    }
    printf("\nEnter the elements of matrix 2: \n");
    for(int i=0;i<n2;i++){
        for(int j=0;j<m2;j++){
            printf("\nEnter value of B[%d][%d]: ", i, j);
            scanf("%d", &B[i][j]);
        }
    }
    /*---------------------------------------------------------------------------------------------------------*/

    /* Declaring and initializing parameters for matrix operations */

    /*---------------------------------------------------------------------------------------------------------*/
    struct params *p = (struct params*)malloc(sizeof(struct params));
    p->A = A;
    p->B = B;
    p->n1 = n1;
    p->m1 = m1;
    p->n2 = n2;
    p->m2 = m2;

    /*---------------------------------------------------------------------------------------------------------*/

    /* Creating a thread to add the matrices */
    pthread_t tid[5]; // {add, sub, mul, trace, check}

    printf("---------------------------------------------------------------------------------");
    pthread_create(&tid[0], NULL, add, p);
    pthread_create(&tid[1], NULL, sub, p);
    pthread_create(&tid[2], NULL, mul, p);
    pthread_create(&tid[3], NULL, trace, p);
    pthread_create(&tid[4], NULL, check, p);  
    int** C;
    pthread_join(tid[0], (void**)&C);
    if(C!=NULL){
        printf("\nThe sum of the matrices is: \n");
        for(int i=0; i<n1; i++){
            for(int j=0; j<m1; j++){
                printf("%d ", C[i][j]);
            }
            printf("\n");
        }
    }
    pthread_join(tid[1], (void**)&C);
    if(C!=NULL){
        printf("\nThe difference of the matrices is: \n");
        for(int i=0; i<n1; i++){
            for(int j=0; j<m1; j++){
                printf("%d ", C[i][j]);
            }
            printf("\n");
        }
    }
    pthread_join(tid[2], (void**)&C);
    if(C!=NULL){
        printf("\nThe product of the matrices is: \n");
        for(int i=0; i<n1; i++){
            for(int j=0; j<m2; j++){
                printf("%d ", C[i][j]);
            }
            printf("\n");
        }
    }
    pthread_join(tid[3], NULL);
    pthread_join(tid[4], NULL);
    printf("---------------------------------------------------------------------------------");
    return 0;
}