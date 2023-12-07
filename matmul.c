#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_ROWS 100
#define MAX_COL 100

int mat1[MAX_ROWS][MAX_COL], mat2[MAX_ROWS][MAX_COL], res[MAX_ROWS][MAX_COL];
int r1, c1, r2, c2;

void inputMat(int [][MAX_COL], int, int);
void* matMul(void*);
void displayMat(int [][MAX_COL], int, int);

int main()
{
    printf("Enter the rows and columns of matrix 1:\n");
    scanf("%d %d", &r1, &c1);
    printf("Enter the elements of matrix 1:\n");
    inputMat(mat1, r1, c1);

    printf("Enter the rows and columns of matrix 2:\n");
    scanf("%d %d", &r2, &c2);
    printf("Enter the elements of matrix 2:\n");
    inputMat(mat2, r2, c2);

    if (c1 != r2)
    {
        printf("Invalid dimensions for matrix multiplication.\n");
        return 0;
    }

    pthread_t tid[r1];
    int i,tno[r1];
    for(i=0;i<r1;i++)
    {
            tno[i]=i;
            pthread_create(&tid[i],NULL,matMul,(void*)&tno[i]);
    }

    for(i=0;i<r1;i++)
    {
            pthread_join(tid[i],NULL);
    }

    printf("Matrix 1:\n");
    displayMat(mat1, r1, c1);

    printf("\nMatrix 2:\n");
    displayMat(mat2, r2, c2);

    printf("\nResultant matrix:\n");
    displayMat(res, r1, c2);

    return 0;
}

void inputMat(int mat[][MAX_COL], int r, int c)
{
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            scanf("%d", &mat[i][j]);
        }
    }
}

void* matMul(void* args)
{
    int t=*(int*)args;
    for (int i = t; i < t+1; i++)
    {
        for (int j = 0; j < c2; j++)
        {
            res[i][j] = 0;
            for (int k = 0; k < c1; k++)
            {
                res[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
}

void displayMat(int mat[][MAX_COL], int r, int c)
{
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            printf("%d\t", mat[i][j]);
        }
        printf("\n");
    }
}

