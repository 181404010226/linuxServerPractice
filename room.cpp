#include "room.h"


Room::Room(std::string name) : name(name) {}

std::string Room::getName() {
    return name;
}

void Room::addUser(User* user) {
    users.push_back(user);
    user->setRoom(this);
}

void Room::removeUser(User* user) {
    for (auto it = users.begin(); it != users.end(); it++) {
        if (*it == user) {
            users.erase(it);
            user->setRoom(nullptr);
            // ±ê¼ÇÎª¿Õ
            if (users.empty()) name = "";
            break;
        }
    }
}

std::string Room::showUsers() {
    std::string result;
    for (auto user : users) {
        result += user->getName() + ", ";
    }
    result = result.substr(0, result.length() - 2); // remove the last comma and space
    return result;
}

