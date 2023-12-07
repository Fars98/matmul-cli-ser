#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include<pthread.h>

#define MAX_ROWS 100
#define MAX_COL 100

int mat1[MAX_ROWS][MAX_COL],mat2[MAX_ROWS][MAX_COL],res[MAX_ROWS][MAX_COL];
int r1,c1,r2,c2;

int check(int,const char*);
void displayMat(int[][MAX_COL],int,int);
void* matMul(void*);
void* cli_fun(void*);

int main()
{
	int sersock=0,clisock=0;

	check(sersock=socket(AF_INET,SOCK_STREAM,0),"Failed to create socket");

	int enable = 1;
	check(setsockopt(sersock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)),"setsockopt error");
	struct sockaddr_in serv_addr;

	memset(&serv_addr,'0',sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(8000);
	serv_addr.sin_addr.s_addr=INADDR_ANY;

	check(bind(sersock,(struct sockaddr *)&serv_addr,sizeof(serv_addr)),"Bind failed");

	check(listen(sersock,5),"Listen failed");

	pthread_t tid1;
	while(1)
	{
		check(clisock=accept(sersock,NULL,NULL),"Accept failed");
		printf("Client connected. Creating a new thread...\n");
		check(pthread_create(&tid1,NULL,cli_fun,(void*)&clisock),"Failed to create client thread");
	}

	pthread_join(tid1,NULL);
	close(sersock);
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

void displayMat(int mat[][MAX_COL],int r,int c)
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


void* cli_fun(void* cli_sock)
{
	int clisock=*(int*)cli_sock;
	check(recv(clisock,&r1,sizeof(r1),0),"failed to receive r1");
        check(recv(clisock,&c1,sizeof(c1),0),"failed to receive c1");
        printf("Received dimensions of matrix 1: %d x %d\n", r1, c1);
        check(recv(clisock,mat1,sizeof(mat1),0),"failed to receive mat1");
        printf("Received matrix 1 from client\n");
        displayMat(mat1,r1,c1);

        check(recv(clisock,&r2,sizeof(r2),0),"failed to receive r2");
        check(recv(clisock,&c2,sizeof(c2),0),"failed to receive c2");
        printf("Received dimensions of matrix 2: %d x %d\n", r2, c2);
        check(recv(clisock,mat2,sizeof(mat2),0),"failed to receive mat2");
        printf("Received matrix 2 from client\n");
        displayMat(mat2,r2,c2);

	printf("Resultant Matrix\n");
        pthread_t tid[r1];
        int i,tno[r1];
        for(i=0;i<r1;i++)
        {
        	tno[i]=i;
              	check(pthread_create(&tid[i],NULL,matMul,(void*)&tno[i]),"failed to create matrix thread");
        }

        for(i=0;i<r1;i++)
        {
        	pthread_join(tid[i],NULL);
        }

        displayMat(res,r1,c2);

        check(send(clisock,res,sizeof(res),0),"failed to send resultant matrix");
        printf("Resultant matrix send to client\n");
        close(clisock);

}














