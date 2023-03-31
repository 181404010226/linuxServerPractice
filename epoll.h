#include <iostream>
#include <map>
#include"RoomManager.h"
using namespace std;

struct Client
{
	// �ͻ���Ψһ��ʶ��
	int clientID;
	// �ͻ��˷��͵���Ϣ
	string message;
	// �û���Ϣ
	User* user;
};

#ifndef EPOLL_H
#define EPOLL_H
// ��ʼ��epoll
class epoll
{
public:
	epoll();
private:
	const int MAX_COUNT = 100;
	int epollId;
	map<int, Client> AllClients;
	RoomManager* manager;
};

#endif

