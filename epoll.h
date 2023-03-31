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
	// 处理协议
	bool HandleProtocol(int sockid);
	// 最大连接数
	const int MAX_COUNT = 100;
	int epollId;
	map<int, Client> AllClients;
	RoomManager* manager;
	RoomProtocolHandler* roomHander;
};

#endif

