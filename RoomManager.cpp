#include"RoomManager.h"

RoomManager::RoomManager() : RoomId(1) {}

Room* RoomManager::createRoom(std::string name) {
    Room* newRoom=new Room("Room" + to_string(++RoomId)+"-"+name );
    rooms.push_back(newRoom);
    return newRoom;
}

Room* RoomManager::getRoomByName(std::string name) {
    for (auto room : rooms) {
        if (room->getName() == name) {
            return room;
        }
    }
    return nullptr;
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
    for (auto it = rooms.begin(); it != rooms.end(); it++) {
        // 房间名被标记为空时，证明这个房间不再需要
        if ((*it)->getName() == "")
        {
            delete* it;
            rooms.erase(it);
            // bugfix：如果删除的是最后一个房间，就会跳转到非法区域
            if (it == rooms.end()) break;
        }
    }
    // 删除和遍历不要放到一起
    for (auto it = rooms.begin(); it != rooms.end(); it++) {
        result += (*it)->getName() + ": " + (*it)->showUsers() + "\n";
    }
    return result;
}