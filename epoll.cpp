#include "epoll.h"
#include <cstdio>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>

epoll::epoll():manager(new RoomManager()), roomHander(new RoomProtocolHandler(manager))
{
    //创建监听的sockeL
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) perror("socket error");
    // 绑定本地ip和端口
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=htonl(INADDR_ANY);
    addr.sin_port = htons(9999);

    epollId = epoll_create1(0);
    int ret = bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    if (sockfd < 0) printf("bind error \n");
    // 监听客户端
    ret=listen(sockfd, 1024);
    if (ret < 0) printf("listen error \n");
    //将监听的socket加入epoll
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = sockfd;
    ret = epoll_ctl(epollId, EPOLL_CTL_ADD, sockfd, &ev);
    if (ret < 0) printf("epoll_ ctl error\n");
    //循环监听
    while (1)
    {
        struct epoll_event evs[MAX_COUNT];
        int n = epoll_wait(epollId, evs, MAX_COUNT, -1);
        if (n < 0)
        {
            printf("epoll wait error\n");
            break;
        }
        for (int i = 0; i < n; i++)
        {
            int id = evs[i].data.fd;
            //如果是监听的fd收到消息，那么则表示有客户端进行连接了
            if (id == sockfd)
            {
                struct sockaddr_in client_addr;
                socklen_t client_addr_len = sizeof(client_addr);
                int client_sockfd=accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_len);
                // 将客户端的sccket加入epoll
                struct epoll_event ev_client;
                ev_client.events=EPOLLIN; 
                //检测客户端有没有消息过来
                ev_client.data.fd = client_sockfd;
                ret = epoll_ctl(epollId, EPOLL_CTL_ADD,client_sockfd, &ev_client);
                if(ret < 0) printf("epoll ctl error\n");
                // 输出IP地址
                char ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &(client_addr.sin_addr), ip, INET_ADDRSTRLEN);
                printf("connecting...%s\n", ip);
                // 保存该客户端的信息
                Client client;
                client.clientID = client_sockfd;
                AllClients[client_sockfd] = client;
            }
            else
            {
                if (!HandleProtocol(id))
                {
                    printf("客户端协议出错,id:%d",id);
                    break;
                }
            }
        }
    }
    close(epollId);
    close(sockfd);
}

bool epoll::HandleProtocol(int sockid)
{
    char buffer[1024];
    int n = read(sockid, buffer, 1024);
    if (n < 0) return false;
    else if (n == 0)
    {
        // 客户端断开连接
        close(sockid);
        epoll_ctl(epollId, EPOLL_CTL_DEL, sockid, 0);
        // 离开房间并删除用户
        if (AllClients[sockid].user != nullptr)
        {
            printf("leaving...%s\n", AllClients[sockid].user->getName().c_str());
            manager->leaveRoom(AllClients[sockid].user);
            delete AllClients[sockid].user;
        }
        AllClients.erase(sockid);
    }
    else
    {
        string msg(buffer, n);
        // 如果该客户端name为空，说明该消息是这个客户端的用户名
        if (AllClients[sockid].message == "")
        {
            AllClients[sockid].message = msg;
            AllClients[sockid].user = new User(msg, AllClients[sockid].clientID);
        }
        // 登录后处理相关协议
        else
        {
            string rev = roomHander->handleProtocol(msg, AllClients[sockid].user);
            // 为空情况代表没有找到合法指令
            if (rev == "")
            {
                std::string name = AllClients[sockid].message;
                Room* room = AllClients[sockid].user->getRoom();
                if (room == nullptr) return true;
                vector<User*> users = room->getUsers();
                for (auto& c : users)
                {
                    // 只对同一个房间内的用户发送消息
                    if (c->getId() != sockid)
                    write(c->getId(),
                        ('[' + name + ']' + ":" + msg).c_str(), msg.size() + name.size() + 4);
                }
            }
            else
            {
                write(AllClients[sockid].clientID,
                    rev.c_str(), rev.size() + 4);
            }
        }
    }
    return true;
}
