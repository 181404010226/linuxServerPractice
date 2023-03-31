#include "user.h"
User::User(std::string name,int id) : name(name),id(id), room(nullptr) {}

std::string User::getName() {
    return name;
}

void User::setRoom(Room* room) {
    this->room = room;
}

Room* User::getRoom() {
    return room;
}

int User::getId()
{
    return id;
}
