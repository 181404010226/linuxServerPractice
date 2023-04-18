#pragma once
#ifndef RoomProtocolHandler_H
#define RoomProtocolHandler_H
#include <vector>
#include <string>
#include "RoomManager.h"

class RoomProtocolHandler {
public:
    const int Max_Room_users = 2;
    RoomProtocolHandler(RoomManager* roomManager);
    std::string handleProtocol(std::string protocol, User* user);

private:
    RoomManager* roomManager;
    std::vector<std::string> splitProtocol(std::string protocol);
};
#endif