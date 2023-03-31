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
    //����������sockeL
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) perror("socket error");
    // �󶨱���ip�Ͷ˿�
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=htonl(INADDR_ANY);
    addr.sin_port = htons(9999);

    epollId = epoll_create1(0);
    int ret = bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    if (sockfd < 0) printf("bind error \n");
    // �����ͻ���
    ret=listen(sockfd, 1024);
    if (ret < 0) printf("listen error \n");
    //��������socket����epoll
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = sockfd;
    ret = epoll_ctl(epollId, EPOLL_CTL_ADD, sockfd, &ev);
    if (ret < 0) printf("epoll_ ctl error\n");
    //ѭ������
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
            //����Ǽ�����fd�յ���Ϣ����ô���ʾ�пͻ��˽���������
            if (id == sockfd)
            {
                struct sockaddr_in client_addr;
                socklen_t client_addr_len = sizeof(client_addr);
                int client_sockfd=accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_len);
                // ���ͻ��˵�sccket����epoll
                struct epoll_event ev_client;
                ev_client.events=EPOLLIN; 
                //���ͻ�����û����Ϣ����
                ev_client.data.fd = client_sockfd;
                ret = epoll_ctl(epollId, EPOLL_CTL_ADD,client_sockfd, &ev_client);
                if(ret < 0) printf("epoll ctl error\n");
                // ���IP��ַ
                char ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &(client_addr.sin_addr), ip, INET_ADDRSTRLEN);
                printf("connecting...%s\n", ip);
                // ����ÿͻ��˵���Ϣ
                Client client;
                client.clientID = client_sockfd;
                AllClients[client_sockfd] = client;
            }
            else
            {
                if (!HandleProtocol(id))
                {
                    printf("�ͻ���Э�����,id:%d",id);
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
        // �ͻ��˶Ͽ�����
        close(sockid);
        epoll_ctl(epollId, EPOLL_CTL_DEL, sockid, 0);
        // �뿪���䲢ɾ���û�
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
        // ����ÿͻ���nameΪ�գ�˵������Ϣ������ͻ��˵��û���
        if (AllClients[sockid].message == "")
        {
            AllClients[sockid].message = msg;
            AllClients[sockid].user = new User(msg, AllClients[sockid].clientID);
        }
        // ��¼�������Э��
        else
        {
            string rev = roomHander->handleProtocol(msg, AllClients[sockid].user);
            // Ϊ���������û���ҵ��Ϸ�ָ��
            if (rev == "")
            {
                std::string name = AllClients[sockid].message;
                Room* room = AllClients[sockid].user->getRoom();
                if (room == nullptr) return true;
                vector<User*> users = room->getUsers();
                for (auto& c : users)
                {
                    // ֻ��ͬһ�������ڵ��û�������Ϣ
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
