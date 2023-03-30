#pragma once
#include <vector>
#include <string>
#include"user.h"

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