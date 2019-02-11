#include "chats.h"

QDataStream& operator>>(QDataStream& input, Message& data)
{
    input >> data.senderId >> data.senderName >> data.messageString >> data.time >> data.haveRead;
    return input;
}

QDataStream& operator<<(QDataStream& output, const Message& data)
{
    output << data.senderId << data.senderName << data.messageString << data.time << data.haveRead;
    return output;
}

QDataStream& operator>>(QDataStream& input, Chat& data)
{
    input >> data.sumNum >> data.unreadNum >> data.type >> data.subType >> data.senderId >> data.senderName >> data.readyMessage >> data.messages;
    return input;
}

QDataStream& operator<<(QDataStream& output, const Chat& data)
{
    output << data.sumNum << data.unreadNum << data.type << data.subType << data.senderId << data.senderName << data.readyMessage << data.messages;
    return output;
}

QDataStream& operator>>(QDataStream& input, Chats& data)
{
    input >> data.id >> data.chats;
    return input;
}

QDataStream& operator<<(QDataStream& output, const Chats& data)
{
    output << data.id << data.chats;
    return output;
}
