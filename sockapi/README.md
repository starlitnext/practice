###Ҫ��
* ����ʵ����򵥵�socket�ͻ��˺ͷ������ˣ�ʵ�ֻ����Ŀͻ��˸���������ʲô���ݣ��������˾ͻظ�ʲô���ݣ�ʹ�õ����漸������������
    - �������ˣ�
        - socket
        - bind
        - setsockopt��setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))
        - listen
        - accept
        - read��write
        - close
    - �ͻ��ˣ�
        - socket
        - connect
        - read�� write
        - close
* ���ʱ�������������
    - ʵ��p2p���죺
        - �ͻ��ˣ���connect֮��forkһ���ӽ��̣������̺��ӽ���һ����������Ϣ��һ��������Ϣ
        - �������ˣ���acceptһ������֮��forkһ���ӽ��̣������̺��ӽ���һ����������Ϣ��һ��������Ϣ
        - ���ʱ����Ҫע�ⲻҪ�����¶����̺ͽ�ʬ���̣����ʱ����Ҫע���ź���ʵ��
        - ע���ڸ��ӽ����йرղ���Ҫ��socket��������close�������ü����ķ�ʽ
    - �÷�������֧�ֶ�ͻ������ӣ�
        - fork�ӽ��̵ķ�ʽ
        - select IO���ã����߽�����·IO��
* fork�ӽ��̵ķ�ʽʵ�ֶ�ͻ������ӣ�
    - ѭ���ĵ���accept��������ȡһ��conn֮��forkһ���ӽ��̺Ϳͻ��˽���ͨ��
    - ע���ӽ���һ���ȸ������Ƚ�����Ҫ����SIGCHLD�ź�
    - ����SIGCHLD��ʱ��ע�����ͻ���ͬʱ�رյĳ���
        - while ( (pid = waitpid(0, NULL, WNOHANG)) > 0)��
    - ����SIGPIPE:����Է�socket�ѹرգ��Եȷ��ٷ�д���ݣ�������SIGPIPE�źţ�Ĭ�϶���Ϊterminal
* ����ճ�����⣺
    - readn, writen
    - ������
    - ��β����\r\n ��ftp��:recv_peek��recv(sockfd, buf, len, MSG_PEEK)��, readline
    - ��ͷ���ϰ��峤��
    - �����ӵ�Ӧ�ò�Э��
* select�����ֳ�����
    - ��select��װ��ʱ��connect��accept��read��write����ע��connect��socketĬ������ģʽ����Ҫ����Ϊ���������ٵ���connect��select
    - ��select�Ż��ͻ��ˣ���fileno(stdin)��sockd������������
    - �������ˣ�����������һ��listenfd�����ɸ�connfd��
* �������˺Ϳͻ���API���