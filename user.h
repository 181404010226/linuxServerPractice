#ifndef USER_H
#define USER_H
#pragma once
#include"room.h"
#include <vector>
#include <string>
using namespace std;
class Room;
class User {
public:
    User(std::string name,int id);
    std::string getName();
    void setRoom(Room* room);
    Room* getRoom();
    int getId();
private:
    std::string name;
    Room* room;
    int id;
};
#endif