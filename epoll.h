#pragma once
#include <iostream>
#include <map>
using namespace std;
struct Client
{
	int clientID;
	string message;
};

// ��ʼ��epoll
class epoll
{
public:
	epoll();
private:
	const int MAX_COUNT = 100;
	int epollId;
	map<int, Client> AllClients;
};

