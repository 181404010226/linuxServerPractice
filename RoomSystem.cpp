// User.cpp
#include"RoomSystem.h"

RoomManager::RoomManager() : RoomId(1) {}

Room* RoomManager::createRoom(std::string name) {
    Room* newRoom = new Room(name);
    rooms.push_back(newRoom);
    return newRoom;
}

void RoomManager::joinRoom(Room* room, User* user) {
    room->addUser(user);
}

void RoomManager::leaveRoom(User* user) {
    Room* room = user->getRoom();
    if (room) {
        room->removeUser(user);
    }
}

std::string RoomManager::showRooms() {
    std::string result;
    for (auto room : rooms) {
        result += room->getName() + ": " + room->showUsers() + "\n";
    }
    return result;
}