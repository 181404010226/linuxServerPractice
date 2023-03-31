#ifndef EPOLL_H
#define EPOLL_H
#pragma once
#include <iostream>
#include <map>
#include"RoomManager.h"
#include "RoomProtocolHandler.h"
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
// ��ʼ��epoll
class epoll
{
public:
	epoll();
private:
	// ����Э��
	bool HandleProtocol(int sockid);
	// ���������
	const int MAX_COUNT = 100;
	int epollId;
	map<int, Client> AllClients;
	RoomManager* manager;
	RoomProtocolHandler* roomHander;
};

#endif

