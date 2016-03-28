###要点
* 首先实现最简单的socket客户端和服务器端，实现基本的客户端给服务器发什么内容，服务器端就回复什么内容，使用到下面几个基础函数：
    - 服务器端：
        - socket
        - bind
        - setsockopt：setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))
        - listen
        - accept
        - read，write
        - close
    - 客户端：
        - socket
        - connect
        - read， write
        - close
* 这个时候提出两个需求
    - 实现p2p聊天：
        - 客户端：在connect之后fork一个子进程，父进程和子进程一个负责收消息，一个负责发消息
        - 服务器端：在accept一个连接之后，fork一个子进程，父进程和子进程一个负责收消息，一个负责发消息
        - 这个时候需要注意不要产生孤儿进程和僵尸进程，这个时候需要注册信号来实现
        - 注意在父子进程中关闭不必要的socket描述符，close采用引用计数的方式
    - 让服务器端支持多客户端连接：
        - fork子进程的方式
        - select IO复用（或者叫做多路IO）
* fork子进程的方式实现多客户端连接：
    - 循环的调用accept函数，获取一个conn之后fork一个子进程和客户端进行通信
    - 注意子进程一般会比父进程先结束，要处理SIGCHLD信号
    - 处理SIGCHLD的时候注意多个客户端同时关闭的场景
        - while ( (pid = waitpid(0, NULL, WNOHANG)) > 0)；
    - 处理SIGPIPE:如果对方socket已关闭，对等方再发写数据，则会产生SIGPIPE信号，默认动作为terminal
* 处理粘包问题：
    - readn, writen
    - 定长包
    - 包尾加上\r\n （ftp）:recv_peek（recv(sockfd, buf, len, MSG_PEEK)）, readline
    - 包头加上包体长度
    - 更复杂的应用层协议
* select的三种场景：
    - 用select封装超时（connect、accept、read、write），注意connect，socket默认阻塞模式，需要设置为非阻塞，再调用connect和select
    - 用select优化客户端（有fileno(stdin)和sockd两个描述符）
    - 服务器端（服务器端有一个listenfd和若干个connfd）
* 服务器端和客户端API框架