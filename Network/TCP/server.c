#include "config.h"

ssize_t readline(int fd, char *vptr, size_t maxlen)
{
	ssize_t	n, rc;
	char	c, *ptr;

	ptr = vptr;
	for (n = 1; n < maxlen; n++) {
		if ( (rc = read(fd, &c,1)) == 1) {
			*ptr++ = c;
			if (c == '\n')
				break;	/* newline is stored, like fgets() */
		} else if (rc == 0) {
			*ptr = 0;
			return(n - 1);	/* EOF, n - 1 bytes were read */
		} else
			return(-1);		/* error, errno set by read() */
	}

	*ptr = 0;	/* null terminate like fgets() */
	return(n);
}

int main(int argc,char **argv){
    printf("========= create the server =========\n");

    int server_fd, new_socket;
    int opt = 1;
    struct sockaddr_in server_address, child_address;
    pid_t child_pid;

    socklen_t child_len = sizeof(child_address);

    char *hello = "Hello from server";

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(PORT);

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("create server socket descriptor failed");
        exit(EXIT_FAILURE);
    }
    if(bind(server_fd,(struct sockaddr *)&server_address, sizeof(server_address)))
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("========= Server:%s:%d start to listening =========\n",inet_ntoa(server_address.sin_addr), ntohs(server_address.sin_port));

    while(1){
        child_len = sizeof(child_address);
        if((new_socket = accept(server_fd,(struct sockaddr *)&child_address, &child_len))<0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        if((child_pid = fork())==0)
        {
            close(server_fd);
            
            ssize_t valread;
            char recvline[MAX_LINE];
            while((valread = readline(new_socket, recvline, MAX_LINE))>0)
            {
                if(fputs(recvline, stdout) == EOF)
                {
                    perror("fputs error");
                    exit(1);
                }
            } 
            exit(0);
        }
        close(new_socket);
    }

    close(server_fd);
    return 0;
}