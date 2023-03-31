#include "epoll.h"
#include <cstdio>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

epoll::epoll():manager(new RoomManager())
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
                char buffer[1024];
                int n = read(id, buffer, 1024);
                if (n < 0) break;
                else if (n == 0)
                {
                    // 客户端断开连接
                    close(id);
                    epoll_ctl(epollId, EPOLL_CTL_DEL, id, 0);
                    // 离开房间并删除用户
                    if (AllClients[id].user != nullptr)
                    {
                        printf("leaving...%s\n", AllClients[id].user->getName().c_str());
                        manager->leaveRoom(AllClients[id].user);
                        delete AllClients[id].user;
                    }
                    AllClients.erase(id);
                }
                else
                {
                    string msg(buffer, n);
                    // 如果该客户端name为空，说明该消息是这个客户端的用户名
                    if (AllClients[id].message == "")
                    {
                        AllClients[id].message = msg;
                        AllClients[id].user = new User(msg);
                     }
                    // 未加入房间情况
                    else if (AllClients[id].user->getRoom() == nullptr)
                    {
                        // 创建房间
                        if (msg.find("CreateRoom"))
                        {
                            Room* room1 = manager->createRoom();
                            write(AllClients[id].clientID,
                                manager->showRooms().c_str(), manager->showRooms().size() + 4);
                        }
                    }
                    else
                    {
                        if (msg.find("LeaveRoom"))
                        {                        
                            manager->leaveRoom(AllClients[id].user);
                            write(AllClients[id].clientID,
                                manager->showRooms().c_str(), manager->showRooms().size() + 4);
                        }
                        else {
                            //否则是聊天消息
                            std::string name = AllClients[id].message;
                            for (auto& c : AllClients)
                                if (c.first != id)
                                    write(c.first,
                                        ('[' + name + ']' + ":" + msg).c_str(), msg.size() + name.size() + 4);
                        }

                    }
                }

            }
        }
    }
    close(epollId);
    close(sockfd);
}
