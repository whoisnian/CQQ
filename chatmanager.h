#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include <QDataStream>
#include <QDateTime>
#include <QList>
#include <QString>

// 聊天数据存储
class Message
{
public:
    Message(){}

    friend QDataStream& operator>>(QDataStream& input,
                                   Message& data);
    friend QDataStream& operator<<(QDataStream& output,
                                   const Message& data);

    QString senderID;
    QString senderName;
    QString messageString;
    QDateTime time;
};

class Chat
{
public:
    enum Type
    {
        Private,
        Group
    };

    enum SubType
    {
        SubFriend,
        SubGroup,
        SubDiscuss,
        SubOther,

        SubNormal,
        SubAnonymous,
        SubNotice
    };

    Chat(){}

    friend QDataStream& operator>>(QDataStream& input,
                                   Chat& data);
    friend QDataStream& operator<<(QDataStream& output,
                                   const Chat& data);

    int sumNum;
    int unreadNum;
    Type type;
    SubType subType;
    QString chatID;
    QString chatName;
    QString editingMessage;
    QList<Message> messages;
};

class ChatManager
{
public:
    ChatManager(){}
    int indexOf(QString chatID, Chat::Type type);
    void addNewChat(QString chatID, QString chatName, Chat::Type type);

    friend QDataStream& operator>>(QDataStream& input,
                                   ChatManager& data);
    friend QDataStream& operator<<(QDataStream& output,
                                   const ChatManager& data);

    QString selfID;
    QList<Chat> chats;
};

#endif // CHATMANAGER_H
