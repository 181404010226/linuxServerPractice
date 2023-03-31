#include "RoomProtocolHandler.h"

RoomProtocolHandler::RoomProtocolHandler(RoomManager* roomManager) {
    this->roomManager = roomManager;
}

std::string RoomProtocolHandler::handleProtocol(std::string protocol,User* user) {
    std::vector<std::string> protocolParts = splitProtocol(protocol);
    std::string command = protocolParts[0];
    std::string response = "";

    if (command == "createRoom") {
        // 已有房间时退出
        Room* room = user->getRoom();
        if (room != nullptr) {
            response = "Already Joined Room";
            return response;
        }
        // 新建一个房间
        std::string roomName = "";
        if (protocolParts.size() > 1) {
            roomName = protocolParts[1];
        }
        Room* newRoom = roomManager->createRoom(roomName);
        response = "Room created with ID " + newRoom->getName();
        roomManager->joinRoom(newRoom, user);
        response += "\n"+roomManager->showRooms();
    }
    else if (command == "joinRoom") {
        if (protocolParts.size() < 2) {
            response = "Invalid protocol. Usage: joinRoom <room_id> ";
        }
        else {
            string roomName = protocolParts[1];
            Room* room = user->getRoom();
            if (room != nullptr) {
                response = "Already Joined Room";
            }
            else {
                room=roomManager->getRoomByName("Room" + roomName);
                if (room == nullptr)
                {
                    response = "No room has name:" + roomName;
                }
                else
                {
                    roomManager->joinRoom(room, user);
                    response = "User " + user->getName() + " joined room " + room->getName();
                }
            }
        }
    }
    else if (command == "leaveRoom") {
        if (user->getRoom() == nullptr) {
            response = "Not In Any Room";
        }
        else {
            roomManager->leaveRoom(user);
            response = "User " + user->getName() + " left the room";
        }
    }
    else if (command == "showRooms") {
        response = roomManager->showRooms();
    }
    return response;
}

std::vector<std::string> RoomProtocolHandler::splitProtocol(std::string protocol) {
    std::vector<std::string> parts;
    std::string delimiter = " ";
    size_t pos = 0;
    std::string token;
    while ((pos = protocol.find(delimiter)) != std::string::npos) {
        token = protocol.substr(0, pos);
        parts.push_back(token);
        protocol.erase(0, pos + delimiter.length());
    }
    parts.push_back(protocol);
    return parts;
}