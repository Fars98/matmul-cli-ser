#include<stdio.h>
#include<stdlib.h>

#define MAX_ROWS 100
#define MAX_COL 100

void display(int,int,int[][MAX_COL]);
void matMul(int,int, int[][MAX_COL],int,int, int[][MAX_COL],int[][MAX_COL]);

int main()
{
	int r1,c1,r2,c2;
	int mat1[MAX_ROWS][MAX_COL],mat2[MAX_ROWS][MAX_COL],res[MAX_ROWS][MAX_COL];
	int i,j;

	printf("Enter the rows and columns of matrix 1:\n");
	scanf("%d %d",&r1,&c1);

        printf("Enter the elements of matrix 1:\n");
        for(i=0;i<r1;i++)
        {
                for(j=0;j<c1;j++)
                {
                        scanf("%d",&mat1[i][j]);
                }
        }

	printf("Enter the rows and columns of matrix 2:\n");
	scanf("%d %d",&r2,&c2);
	printf("Enter the elements of matrix 2:\n");
        for(i=0;i<r2;i++)
        {
                for(j=0;j<c2;j++)
                {
                        scanf("%d",&mat2[i][j]);
                }
        }


	printf("Matrix 1 is:\n");
	display(r1,c1,mat1);

        printf("Matrix 2 is:\n");
        display(r2,c2,mat2);

	printf("Resultant matrix is:\n");
	matMul(r1,c1,mat1,r2,c2,mat2,res);
        display(r1,c2,res);
}

void display(int r,int c,int mat[][MAX_COL])
{
        int i,j;
	for(i=0;i<r;i++)
        {
                for(j=0;j<c;j++)
                {
                        printf("%d\t",mat[i][j]);
                }
                printf("\n");
        }
}

void matMul(int r1,int c1, int mat1[][MAX_COL],int r2,int c2, int mat2[][MAX_COL],int res[][MAX_COL])
{
    for (int i = 0; i < r1; i++)
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

