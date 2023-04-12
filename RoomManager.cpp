#include"RoomManager.h"
#include <unistd.h>

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
    // ����Ҽ��뷿��ʱ֪ͨ�������������ˢ�·���
    for (auto otherUser : room->getUsers())
    {
        if (otherUser->getId() != user->getId())
        {
            string temp = showRooms();
            write(otherUser->getId(),
                temp.c_str(), temp.size() + 4);
        }
    }
}

void RoomManager::leaveRoom(User* user) {
    Room* room = user->getRoom();
    if (room) {
        room->removeUser(user);
        // ������뿪����ʱ֪ͨ�������������ˢ�·���
        for (auto otherUser : room->getUsers())
        {
            if (otherUser->getId() != user->getId())
            {
                string temp = showRooms();
                write(otherUser->getId(),
                    temp.c_str(), temp.size() + 4);
            }
        }
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