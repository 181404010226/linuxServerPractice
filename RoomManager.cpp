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
        // �����������Ϊ��ʱ��֤��������䲻����Ҫ
        if ((*it)->getName() == "")
        {
            delete* it;
            rooms.erase(it);
            // bugfix�����ɾ���������һ�����䣬�ͻ���ת���Ƿ�����
            if (it == rooms.end()) break;
        }
    }
    // ɾ���ͱ�����Ҫ�ŵ�һ��
    for (auto it = rooms.begin(); it != rooms.end(); it++) {
        result += (*it)->getName() + ": " + (*it)->showUsers() + "\n";
    }
    return result;
}