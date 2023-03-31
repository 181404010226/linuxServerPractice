#include"RoomManager.h"

RoomManager::RoomManager() : RoomId(1) {}

Room* RoomManager::createRoom(std::string name) {
    Room* newRoom = new Room(name+to_string(++RoomId));
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
    for (auto it = rooms.begin(); it != rooms.end(); it++) {
        // �����������Ϊ��ʱ��֤��������䲻����Ҫ
        if ((*it)->getName() == "")
        {
            delete* it;
            rooms.erase(it);
        }
    }
    // ɾ���ͱ�����Ҫ�ŵ�һ��
    for (auto it = rooms.begin(); it != rooms.end(); it++) {
        result += (*it)->getName() + ": " + (*it)->showUsers() + "\n";
    }
    return result;
}