#include "user.h"
User::User(std::string name) : name(name), room(nullptr) {}

std::string User::getName() {
    return name;
}

void User::setRoom(Room* room) {
    this->room = room;
}

Room* User::getRoom() {
    return room;
}