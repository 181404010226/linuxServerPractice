#ifndef EPOLL_H
#define EPOLL_H
#pragma once
#include <iostream>
#include <map>
#include"RoomManager.h"
using namespace std;

struct Client
{
	// 客户端唯一标识符
	int clientID;
	// 客户端发送的消息
	string message;
	// 用户信息
	User* user;
};
// 初始化epoll
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

