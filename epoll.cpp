#include "epoll.h"
#include <cstdio>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

epoll::epoll():manager(new RoomManager())
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
                char buffer[1024];
                int n = read(id, buffer, 1024);
                if (n < 0) break;
                else if (n == 0)
                {
                    // �ͻ��˶Ͽ�����
                    close(id);
                    epoll_ctl(epollId, EPOLL_CTL_DEL, id, 0);
                    // �뿪���䲢ɾ���û�
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
                    // ����ÿͻ���nameΪ�գ�˵������Ϣ������ͻ��˵��û���
                    if (AllClients[id].message == "")
                    {
                        AllClients[id].message = msg;
                        AllClients[id].user = new User(msg);
                     }
                    // δ���뷿�����
                    else if (AllClients[id].user->getRoom() == nullptr)
                    {
                        // ��������
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
                            //������������Ϣ
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
