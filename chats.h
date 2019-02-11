#ifndef CHATS_H
#define CHATS_H

#include <QDateTime>
#include <QList>
#include <QDataStream>

class Message
{
public:
    Message(){}

    friend QDataStream& operator>>(QDataStream& input, Message& data);
    friend QDataStream& operator<<(QDataStream& output, const Message& data);

    QString senderId;
    QString senderName;
    QString messageString;
    QDateTime time;
    bool haveRead;
};

class Chat
{
public:
    Chat(){}

    friend QDataStream& operator>>(QDataStream& input, Chat& data);
    friend QDataStream& operator<<(QDataStream& output, const Chat& data);

    int sumNum;
    int unreadNum;
    int type;
    int subType;
    QString senderId;
    QString senderName;
    QString readyMessage;
    QList<Message> messages;

    // type
    const static int Private = 1;
    const static int Group = 2;

    // sub_type
    const static int FriendSub = 1;
    const static int GroupSub = 2;
    const static int DiscussSub = 3;
    const static int OtherSub = 4;
    const static int normalSub = 5;
    const static int anonymousSub = 6;
    const static int noticeSub = 7;
};

class Chats
{
public:
    Chats(){}

    friend QDataStream& operator>>(QDataStream& input, Chats& data);
    friend QDataStream& operator<<(QDataStream& output, const Chats& data);

    QString id;
    QList<Chat> chats;
};

#endif // CHATS_H
