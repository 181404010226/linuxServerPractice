#pragma once
#ifndef USER_H
#define USER_H
#include"room.h"
#include <vector>
#include <string>
using namespace std;
class Room;
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
#endif