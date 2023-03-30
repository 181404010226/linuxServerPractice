#pragma once
#include"room.h"
#include <vector>
#include <string>

class User {
public:
    User(std::string name);
    std::string getName();
    void setRoom(Room* room);
    Room* getRoom();
private:
    std::string name;
    Room* room;
};