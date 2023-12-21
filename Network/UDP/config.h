#pragma once 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

static const int DEFAULT_PORT = 8080;
static const int MAXLINE = 2048;
static const int MAX_RETRY = 10;
static const int MULTIPLIER = 2;
static const int BASE = 500; //millisecond
static const int MAX_WAIT_INTERVAL = 8000; //millisecond