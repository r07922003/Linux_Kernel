#include "config.h"

void dg_cli(FILE *fp , int sockfd , const struct sockaddr *pservaddr , socklen_t servlen)
{
    // Exponential Backoff : wait_interval = base * multiplier^n .
	int n;
    int count_of_retry = 0;
    int cur_wait_interval = BASE;
    char sendline[MAXLINE] , recvline[MAXLINE+1];
    fd_set readfds;
    struct timeval tv;
	
    printf("input your message: ");
	while(fgets(sendline , MAXLINE , fp) == NULL) perror("message is empty please reinput");   
	
    while(count_of_retry<MAX_RETRY)
    {
        if(cur_wait_interval<MAX_WAIT_INTERVAL) cur_wait_interval = min(cur_wait_interval*MULTIPLIER,MAX_WAIT_INTERVAL);

        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);

        tv.tv_sec = cur_wait_interval/1000;
        tv.tv_usec = (cur_wait_interval % 1000)*1000;
        
        if(sendto(sockfd , sendline , strlen(sendline) , 0 , pservaddr ,  servlen) < 0)
        {
            perror("sendto error");
            exit(1);
        }
        
        if(select(sockfd+1, &readfds, NULL, NULL, &tv)<0){
            perror("select error");
            exit(1);
        }

        if(FD_ISSET(sockfd, &readfds))
        {
            if((n = recvfrom(sockfd , recvline , MAXLINE , 0 , NULL , NULL)) < 0)
            {
                perror("recvfrom error");
                exit(1);
            }

            recvline[n] = '\0';
            printf("reponse message from server: ");
            fputs(recvline , stdout);
            exit(0);
        }
        else printf("Timeout expired after %d milliseconds. Retrying: %d times\n",cur_wait_interval,++count_of_retry);
    }
    fprintf(stderr, "Reach Max retry count. Exiting...\n");
    exit(1);
}

int main(int argc,char **argv)
{
    int client_fd;
    struct sockaddr_in server_address;
    
    memset(&server_address,0,sizeof(server_address));

    //create a UDP socket
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port =  htons(DEFAULT_PORT);
    server_address.sin_family = AF_INET;

    // Convert IPv4 and IPv6 addresses from text to binary
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


    if((client_fd = socket(AF_INET, SOCK_DGRAM, 0))<0)
    {
        perror("socket error");
        exit(1);
    }

    printf("========= Client start to connecting %s:%d =========\n",inet_ntoa(server_address.sin_addr), ntohs(server_address.sin_port));

    dg_cli(stdin , client_fd , (struct sockaddr *)&server_address , sizeof(server_address));

    return 0;
}