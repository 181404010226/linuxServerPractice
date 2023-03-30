#pragma once
#include <vector>
#include <string>
#include "room.h"
#include"user.h"

class RoomManager {
public:
    RoomManager();
    Room* createRoom(std::string name = "Room");
    void joinRoom(Room* room, User* user);
    void leaveRoom(User* user);
    std::string showRooms();
private:
    std::vector<Room*> rooms;
    int RoomId;
};
