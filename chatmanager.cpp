#include "chatmanager.h"

int ChatManager::indexOf(QString chatID, Chat::Type type)
{
    for(auto it = this->chats.begin();it < this->chats.end();it++)
    {
        if(it->type == type&&it->chatID == chatID)
        {
            return it - this->chats.begin();
        }
    }
    return -1;
}

void ChatManager::addNewChat(QString chatID,
                             QString chatName,
                             Chat::Type type)
{
    Chat temp;
    temp.sumNum = 0;
    temp.unreadNum = 0;
    temp.type = type;
    temp.subType = Chat::SubFriend;
    temp.chatID = chatID;
    temp.chatName = chatName;
    temp.editingMessage = "";
    temp.messages.clear();
    this->chats.push_front(temp);
}

QDataStream& operator>>(QDataStream& input, Message& data)
{
    input >> data.senderID
          >> data.senderName
          >> data.messageString
          >> data.time;
    return input;
}

QDataStream& operator<<(QDataStream& output, const Message& data)
{
    output << data.senderID
           << data.senderName
           << data.messageString
           << data.time;
    return output;
}

QDataStream& operator>>(QDataStream& input, Chat::Type& data)
{
    quint32 temp;
    input >> temp;
    data = Chat::Type(temp);
    return input;
}

QDataStream& operator<<(QDataStream& output, const Chat::Type& data)
{
    output << quint32(data);
    return output;
}

QDataStream& operator>>(QDataStream& input, Chat::SubType& data)
{
    quint32 temp;
    input >> temp;
    data = Chat::SubType(temp);
    return input;
}

QDataStream& operator<<(QDataStream& output, const Chat::SubType& data)
{
    output << quint32(data);
    return output;
}

QDataStream& operator>>(QDataStream& input, Chat& data)
{
    input >> data.sumNum
          >> data.unreadNum
          >> data.type
          >> data.subType
          >> data.chatID
          >> data.chatName
          >> data.editingMessage
          >> data.messages;
    return input;
}

QDataStream& operator<<(QDataStream& output, const Chat& data)
{
    output << data.sumNum
           << data.unreadNum
           << data.type
           << data.subType
           << data.chatID
           << data.chatName
           << data.editingMessage
           << data.messages;
    return output;
}

QDataStream& operator>>(QDataStream& input, ChatManager& data)
{
    input >> data.selfID
          >> data.chats;
    return input;
}

QDataStream& operator<<(QDataStream& output, const ChatManager& data)
{
    output << data.selfID
           << data.chats;
    return output;
}
