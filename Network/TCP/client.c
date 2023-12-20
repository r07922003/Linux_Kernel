#include "config.h"

int main(int argc, char const* argv[]){
    printf("========= create the client =========\n");

    int client_fd;
    struct sockaddr_in client_address;

    if((client_fd = socket(AF_INET, SOCK_STREAM, 0))<0){
        perror("create client socket descriptor failed");
        return -1;
    }

    memset(&client_address, 0, sizeof(client_address));
    client_address.sin_family = AF_INET;
    client_address.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary
    if(inet_pton(AF_INET, "127.0.0.1", &client_address.sin_addr)<=0){
        printf("\n!!! Invalid address/ Address not supported !!!\n");
        return -1;
    }

    if(connect(client_fd,(struct sockaddr *)&client_address, sizeof(client_address))<0){
        printf("\n!!! Connection Failed !!!\n");
        return -1;
    }


    char sendline[MAX_LINE];
    char recvline[MAX_LINE];
    memset(sendline,0,sizeof(sendline));
    
    while(fgets(sendline, MAX_LINE, stdin) != NULL)
    {
        write(client_fd, sendline ,strlen(sendline));
    }

    close(client_fd);
    return 0;
}