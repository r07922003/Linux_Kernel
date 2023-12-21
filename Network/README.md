# NetWork Socket about linux using by c

Prerequisite knowledge:
1. More detail of network: [link](https://hackmd.io/@nealson840123/network)
2. I/O 事件驅動伺服器：原理和實例: [link](https://hackmd.io/@sysprog/event-driven-server?fbclid=IwAR27OU4Aa35YqsPm5HU8-TgJh8oyqo50c8Y7b9yvsKEct4L5nQjRSybRVc0)

complie the server.c and client.c
```
make
```
test server: open one terminal to start server
```
./server <IP> <Port>
```
test client: open one terminal to start client
```
./client <IP> <Port>
```

Testing Demo Vedio: [link](https://youtu.be/f8sakRxpbc8)

Futrue Work:
- [Handling out-of-band and in-band date with select and handle multiple clients by select without fork.](https://www.topcoder.com/thrive/articles/Linux%20Programming%20-%20Dive%20Into%20the%20Select%20Model)