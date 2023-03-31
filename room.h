#pragma once
#ifndef ROOM_H
#define ROOM_H
#include <vector>
#include <string>
#include"user.h"
using namespace std;
class User;
class Room {
public:
    Room(std::string name);
    std::string getName();
    void addUser(User* user);
    void removeUser(User* user);
    std::string showUsers();
private:
    std::string name;
    std::vector<User*> users;
};
#endif