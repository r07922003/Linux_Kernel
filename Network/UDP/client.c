#include "config.h"

void dg_cli(FILE *fp , int sockfd , const struct sockaddr *pservaddr , socklen_t servlen)
{
	int n;
	char sendline[MAXLINE] , recvline[MAXLINE+1];
	
	while(fgets(sendline , MAXLINE , fp) != NULL)
	{
		if(sendto(sockfd , sendline , strlen(sendline) , 0 , pservaddr ,  servlen) < 0)
		{
			perror("sendto error");
			exit(1);
		}

		if( ( n = recvfrom(sockfd , recvline , MAXLINE , 0 , NULL , NULL)) < 0)
		{
			perror("recvfrom error");
			exit(1);
		}

		recvline[n] = '\0';
		fputs(recvline , stdout);
	}
}

int main(int argc,char **argv)
{
    int client_fd;
    struct sockaddr_in server_address;
    memset(&server_address,0,sizeof(server_address));

    //create a UDP socket
    server_address.sin_port =  htons(PORT);
    server_address.sin_family = AF_INET;

    // Convert IPv4 and IPv6 addresses from text to binary
    if(inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr)<=0){
        printf("\n!!! Invalid address/ Address not supported !!!\n");
        exit(1);
    }

    if((client_fd = socket(AF_INET, SOCK_DGRAM, 0))<0)
    {
        perror("socket error");
        exit(1);
    }

    dg_cli(stdin , client_fd , (struct sockaddr *)&server_address , sizeof(server_address));

    return 0;
}