#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include<signal.h>

#define MAX_ROWS 100
#define MAX_COL 100

int mat1[MAX_ROWS][MAX_COL], mat2[MAX_ROWS][MAX_COL], res[MAX_ROWS][MAX_COL];
int r1, c1, r2, c2;

int check(int,const char*);
void inputMat(int [][MAX_COL], int, int);
void displayMat(int [][MAX_COL], int, int);
void handleSignal(int);
int clisock;

int main()
{
	int connstatus=0;
	signal(SIGINT, handleSignal);

	check(clisock=socket(AF_INET,SOCK_STREAM,0),"Failed to create socket");

	struct timeval timeout;
    	timeout.tv_sec = 5;  // 5 seconds
    	timeout.tv_usec = 0;
    	check(setsockopt(clisock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)),"sersockopt error");
	struct sockaddr_in serv_addr;
        serv_addr.sin_family=AF_INET;
        serv_addr.sin_port=htons(8000);
        serv_addr.sin_addr.s_addr=INADDR_ANY;

	check(connstatus=connect(clisock,(struct sockaddr *)&serv_addr,sizeof(serv_addr)),"Connection failed");

	printf("Enter the rows and columns of matrix 1:\n");
    	scanf("%d %d",&r1, &c1);
	check(send(clisock,&r1,sizeof(r1),0),"failed to send r1");
        check(send(clisock,&c1,sizeof(c1),0),"failed to send c1");

    	printf("Enter the elements of matrix 1:\n");
    	inputMat(mat1, r1, c1);
	printf("Matrix 1:\n");
	displayMat(mat1,r1,c1);

	check(send(clisock,mat1,sizeof(mat1),0),"failed to send mat1");
	printf("Matrix 1 send to server\n");

    	printf("Enter the rows and columns of matrix 2:\n");
    	scanf("%d %d", &r2, &c2);
	check(send(clisock,&r2,sizeof(r2),0),"failed to send r2");
        check(send(clisock,&c2,sizeof(c2),0),"failed to send c2");

    	printf("Enter the elements of matrix 2:\n");
    	inputMat(mat2, r2, c2);
	printf("Matrix 2:\n");
	displayMat(mat2,r2,c2);

	check(send(clisock,mat2,sizeof(mat2),0),"failed to send mat2");
        printf("Matrix 2 send to server\n");

    	if (c1 != r2)
    	{
        	printf("Invalid dimensions for matrix multiplication.\n");
        	return 0;
    	}

	check(recv(clisock,res,sizeof(res),0),"failed to recieve resultant matrix");
        printf("Resultant matrix received from server\n");
	displayMat(res,r1,c2);

	close(clisock);
}

int check(int exp,const char *msg)
{
        if(exp==-1)
        {
                perror(msg);
                exit(1);
        }

        return exp;
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

void handleSignal(int signal)
{
        close(clisock);
        printf("Server terminated. Cleaning up and exiting...\n");
        exit(0);
}

