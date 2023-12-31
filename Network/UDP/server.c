#include "config.h"

void dg_echo(int sockfd , struct sockaddr *pcliaddr , socklen_t clilen)
{
	int n;
	socklen_t len;
	char mesg[MAXLINE];

	while(1)
	{
		len = clilen;
		if((n = recvfrom(sockfd , mesg , MAXLINE , 0 , pcliaddr , &len))<0)
		{
			perror("recvfrom error");
			exit(1);
		}
        mesg[n] = '\0';
        fputs(mesg , stdout);

		if((n = sendto(sockfd , mesg , n , 0 , pcliaddr , len)) < 0)
		{
			perror("sendto error");
			exit(1);
		}
	}
}


int main(int argc,char **argv)
{
    printf("========= create the server =========\n");
    int server_fd;
    struct sockaddr_in server_address, client_address;
    memset(&server_address,0,sizeof(server_address));
    //create a UDP socket
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port =  htons(DEFAULT_PORT);
    server_address.sin_family = AF_INET;

    if(argc>=2 && inet_pton(AF_INET, argv[1], &server_address.sin_addr)<=0)
    {
        printf("WARNING:Invalid address/ Address:%s not supported\n",argv[1]);
        printf("Using Default IP: 0.0.0.0\n");
    }
    if(argc>=3)
    {
        int input_port = atoi(argv[2]);
        if(input_port>=0 && input_port<=65353) server_address.sin_port = htons(input_port);
        else printf("WARNING:Invalid port number:%d\n",input_port);
    }

    if((server_fd = socket(AF_INET, SOCK_DGRAM, 0))<0)
    {
        perror("socket error");
        exit(1);
    }

    if(bind(server_fd,(struct sockaddr *)&server_address, sizeof(server_address)))
    {
        perror("bind error");
        exit(1);
    }

    printf("========= Server:%s:%d start to listening =========\n",inet_ntoa(server_address.sin_addr), ntohs(server_address.sin_port));

    dg_echo(server_fd,(struct sockaddr *)&client_address, sizeof(client_address));

    close(server_fd);
    return 0;
}