#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

static const int PORT = 8080;
static const int MAXLINE = 2048;
static const int multiplier = 2;
static const int base = 500; //millisecond
static const int max_wait_interval = 8; //second