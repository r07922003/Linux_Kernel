# NetWork Socket about linux using by c

Prerequisite knowledge:
1. [More detail of network](https://hackmd.io/@nealson840123/network)
2. [Linux 核心設計: 針對事件驅動的 I/O 模型演化](https://hackmd.io/@sysprog/linux-io-model/https%3A%2F%2Fhackmd.io%2F%40sysprog%2Fevent-driven-server)
    - [I/O 事件驅動伺服器：原理和實例](https://hackmd.io/@sysprog/event-driven-server?fbclid=IwAR27OU4Aa35YqsPm5HU8-TgJh8oyqo50c8Y7b9yvsKEct4L5nQjRSybRVc0)
3. [The method to epoll’s madness](https://copyconstruct.medium.com/the-method-to-epolls-madness-d9d2d6378642)
4. [Async IO on Linux: select, poll, and epoll
](https://jvns.ca/blog/2017/06/03/async-io-on-linux--select--poll--and-epoll/)

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
- Simulate Event-driven Server e.g. **NGINX**
    - Design:
        ![design](https://www.nginx.com/wp-content/uploads/2015/06/infographic-Inside-NGINX_process-model.png)
    - worker 的 main loop 可參考  [ngx_epoll_module.c](https://github.com/nginx/nginx/blob/a64190933e06758d50eea926e6a55974645096fd/src/event/modules/ngx_epoll_module.c) 中的 ngx_epoll_process_events
    -  [seHTTPd](https://github.com/sysprog21/sehttpd) 是個高效的 web 伺服器，涵蓋並行處理、I/O 事件模型、epoll、React pattern，和 Web 伺服器在事件驅動架構的考量，可參見 高效 Web 伺服器開發。

