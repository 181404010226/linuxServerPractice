using namespace std;
#ifndef ROOM_H
#define ROOM_H
#pragma once
#include <vector>
#include <string>
#include"user.h"

class User;
class Room {
public:
    Room(std::string name);
    std::string getName();
    void addUser(User* user);
    void removeUser(User* user);
    std::string showUsers();
    std::vector<User*> getUsers();
private:
    std::string name;
    std::vector<User*> users;
};
#endif