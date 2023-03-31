#pragma once
#include <vector>
#include <string>
#include "room.h"
#include"user.h"
using namespace std;
#ifndef ROOMMANAGER_H
#define ROOMMANAGER_H


class RoomManager {
public:
    RoomManager();
    Room* createRoom(std::string name = "");
    Room* getRoomByName(std::string name);
    void joinRoom(Room* room, User* user);
    void leaveRoom(User* user);
    std::string showRooms();
private:
    std::vector<Room*> rooms;
    int RoomId;
};
#endif
