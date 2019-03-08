#include "wsconnection.h"

WSConnection::WSConnection(QString address,
                           QString token,
                           CacheManager *cacheManager,
                           ChatManager *chatManager,
                           ChatList *chatList,
                           QObject *parent)
{
    this->address = address;
    this->token = token;
    this->cacheManager = cacheManager;
    this->chatManager = chatManager;
    this->chatList = chatList;
    this->setParent(parent);
    this->loop = new QEventLoop(this);
    this->heartbeatTimer = new QTimer(this);

    connect(&wsAPI, SIGNAL(connected()),
            this, SLOT(wsAPIConnected()));
    connect(&wsAPI, SIGNAL(disconnected()),
            this, SLOT(wsAPIDisconnected()));
    connect(&wsEVENT, SIGNAL(connected()),
            this, SLOT(wsEVENTConnected()));
    connect(&wsEVENT, SIGNAL(disconnected()),
            this, SLOT(wsEVENTDisconnected()));
    wsAPI.open(QUrl("ws://"+address+"/api/?access_token="+token));
    loop->exec();
    wsEVENT.open(QUrl("ws://"+address+"/event/?access_token="+token));
    loop->exec();
}

WSConnection::~WSConnection()
{
    qDebug() << "all disconnected";
    wsAPI.close();
    wsEVENT.close();
}

bool WSConnection::isConnected()
{
    if(wsAPI.state() == QAbstractSocket::ConnectedState
            &&wsEVENT.state() == QAbstractSocket::ConnectedState)
        return true;
    return false;
}

void WSConnection::addCommand(CommandType type, QString content)
{
    Command temp;
    temp.content = content;
    temp.type = type;
    if(commandQueue.isEmpty())
    {
        commandQueue.enqueue(temp);
        newStart = true;
        startNextCommand();
    }
    else
    {
        commandQueue.enqueue(temp);
    }
}

void WSConnection::startNextCommand()
{
    if(!newStart)
    {
        commandQueue.dequeue();
    }
    newStart = false;

    if(commandQueue.isEmpty())
    {
        return;
    }

    Command temp = commandQueue.head();
    wsAPI.sendTextMessage(temp.content);
}

void WSConnection::getLoginInfo()
{
    QString content = "{\"action\":\"get_login_info\",\"params\":{}}";
    addCommand(CommandType::get_login_info, content);
}

void WSConnection::getFriendList(ContactList *friendList)
{
    this->friendList = friendList;
    QString content = "{\"action\":\
                            \"_get_friend_list\",\
                            \"params\":{\"flat\":false}\
                       }";
    addCommand(CommandType::_get_friend_list, content);
}

void WSConnection::getGroupList(ContactList *groupList)
{
    this->groupList = groupList;
    QString content = "{\"action\":\"get_group_list\",\"params\":{}}";
    addCommand(CommandType::get_group_list, content);
}

void WSConnection::getGroupMemberInfo(QString groupID, QString userID)
{
    QString content = "{\"action\":\
                            \"get_group_member_info\",\
                            \"params\":{\
                                \"group_id\":" + groupID + ",\
                                \"user_id\":" + userID + "\
                            }\
                       }";
    addCommand(CommandType::get_group_member_info, content);
}

void WSConnection::wsAPIConnected()
{
    qDebug() << "wsAPI connected";
    connect(&wsAPI, SIGNAL(textMessageReceived(const QString &)),
            this, SLOT(wsAPIReceived(const QString &)));
    loop->exit();
    heartbeatTimer->start(15000);
    connect(heartbeatTimer, SIGNAL(timeout()),
            this, SLOT(sendHeartbeat()));
}

void WSConnection::wsAPIDisconnected()
{
    qDebug() << "wsAPI disconnected";
    loop->exit();
}

void WSConnection::wsAPIReceived(const QString message)
{
    qDebug() << "wsAPI received:" << message;
    QJsonDocument jsonDoc;
    jsonDoc = QJsonDocument::fromJson(message.toLocal8Bit().data());
    if(jsonDoc.isNull())
    {
        qDebug() << "broken wsAPI message";
        return;
    }

    if(commandQueue.head().type == CommandType::get_status)
    {
        qDebug() << "wsAPI status: "
                 << (jsonDoc.object().value("data")
                     .toObject().value("good")
                     .toBool()?"OK":"ERROR");
    }
    else if(commandQueue.head().type == CommandType::get_login_info)
    {
        qDebug() << "get_login_info";
        QString ID, Nickname;
        ID = QString::number(jsonDoc.object().value("data")
                             .toObject().value("user_id")
                             .toVariant().toLongLong());
        Nickname = jsonDoc.object().value("data")
                   .toObject().value("nickname").toString();
        emit getLoginInfoFinished(ID, Nickname);
    }
    else if(commandQueue.head().type == CommandType::_get_friend_list)
    {
        qDebug() << "_get_friend_list";
        if(jsonDoc.object().value("data").isArray())
        {
            QJsonArray data = jsonDoc.object().value("data").toArray();
            for(int i = 0;i < data.size();i++)
            {
                QTreeWidgetItem *topItem;
                QString friendGroupName;
                friendGroupName = data.at(i)
                        .toObject().value("friend_group_name")
                        .toString();
                topItem = friendList->addTopItem(friendGroupName);
                QJsonArray friends = data.at(i)
                        .toObject().value("friends")
                        .toArray();
                for(int j = 0;j < friends.size();j++)
                {
                    QString userID, nickname, remark, avatar;
                    userID = QString::number(friends.at(j)
                                             .toObject().value("user_id")
                                             .toVariant().toLongLong());
                    nickname = friends.at(j)
                            .toObject().value("nickname")
                            .toString();
                    remark = friends.at(j)
                            .toObject().value("remark")
                            .toString();
                    avatar = cacheManager->getAvatar(userID,
                                                     CacheManager::Friend,
                                                     100);
                    if(remark.isEmpty())
                    {
                        remark = nickname;
                    }
                    friendList->addChildItem(remark, nickname, userID,
                                             topItem, avatar);
                }
            }
        }
    }
    else if(commandQueue.head().type == CommandType::get_group_list)
    {
        qDebug() << "get_group_list";
        if(jsonDoc.object().value("data").isArray())
        {
            QJsonArray data = jsonDoc.object().value("data").toArray();
            for(int i = 0;i < data.size();i++)
            {
                QString groupID, groupName, avatar;
                groupID = QString::number(data.at(i)
                                          .toObject().value("group_id")
                                          .toVariant().toLongLong());
                groupName = data.at(i)
                        .toObject().value("group_name")
                        .toString();
                avatar = cacheManager->getAvatar(groupID,
                                                 CacheManager::Group,
                                                 100);
                groupList->addChildItem(groupName, groupName, groupID,
                                         nullptr, avatar);
            }
        }
    }
    else if(commandQueue.head().type == CommandType::get_group_member_info)
    {
        qDebug() << "get_group_member_info";
        if(jsonDoc.object().value("data").isObject())
        {
            QJsonObject data = jsonDoc.object().value("data").toObject();
            QString groupID, userID, nickname, card;
            groupID = QString::number(data.value("group_id")
                                      .toVariant().toLongLong());
            userID = QString::number(data.value("user_id")
                                     .toVariant().toLongLong());
            nickname = data.value("nickname").toString();
            card = data.value("card").toString();
            // NeedToBeDone: 存储群名片
        }
    }

    startNextCommand();
}

void WSConnection::wsEVENTConnected()
{
    qDebug() << "wsEVENT connected";
    connect(&wsEVENT, SIGNAL(textMessageReceived(const QString &)),
            this, SLOT(wsEVENTReceived(const QString &)));
    loop->exit();
}

void WSConnection::wsEVENTDisconnected()
{
    qDebug() << "wsEVENT disconnected";
    loop->exit();
}

void WSConnection::wsEVENTReceived(const QString message)
{
    qDebug() << "wsEVENT received:" << message;
    QJsonDocument jsonDoc;
    jsonDoc = QJsonDocument::fromJson(message.toLocal8Bit().data());
    if(jsonDoc.isNull())
    {
        qDebug() << "broken wsEVENT message";
        return;
    }

    if(jsonDoc.object().value("post_type") == "meta_event")
    {
        if(jsonDoc.object().value("meta_event_type") == "heartbeat")
        {
            qDebug() << "wsEVENT status: "
                     << (jsonDoc.object().value("status")
                         .toObject().value("good")
                         .toBool()?"OK":"ERROR");
        }
    }
    else if(jsonDoc.object().value("post_type") == "message")
    {
        if(jsonDoc.object().value("message_type") == "private")
        {
            QString userID, remark, messageString;
            QDateTime time;
            userID = QString::number(jsonDoc.object().value("user_id")
                                     .toVariant().toLongLong());
            // NeedToBeDone: 根据QQ号获取备注
            remark = "remark";
            messageString = jsonDoc.object().value("message")
                    .toString();
            time = QDateTime::fromTime_t(
                        uint(jsonDoc.object().value("time")
                             .toVariant().toLongLong()));
            int index = chatManager->indexOf(userID, Chat::Private);
            if(index == -1)
            {
                QString chatID, chatName, avatar;
                Chat::SubType subType = Chat::SubFriend;
                chatID = userID;
                chatName = remark;
                if(jsonDoc.object().value("sub_type") == "friend")
                {
                    chatName = remark;
                    subType = Chat::SubFriend;
                }
                else if(jsonDoc.object().value("sub_type") == "group")
                {
                    chatName = remark + "(group)";
                    subType = Chat::SubGroup;
                }
                else if(jsonDoc.object().value("sub_type") == "discuss")
                {
                    chatName = remark + "(discuss)";
                    subType = Chat::SubDiscuss;
                }
                else if(jsonDoc.object().value("sub_type") == "other")
                {
                    chatName = remark + "(other)";
                    subType = Chat::SubOther;
                }
                else
                {
                    qDebug() << "unknown private sub_type";
                }
                chatManager->addNewChat(chatID,
                                        chatName,
                                        Chat::Private,
                                        subType);
                chatManager->chatAt(0)->addNewMessage(userID,
                                                      remark,
                                                      messageString,
                                                      time);
                avatar = cacheManager->getAvatar(chatID,
                                                 CacheManager::Friend,
                                                 100);
                chatList->addNewChatItem(avatar,
                                         chatID,
                                         chatName + "(1)");
            }
            else
            {
                chatManager->chatAt(index)->addNewMessage(userID,
                                                          remark,
                                                          messageString,
                                                          time);
                QString chatName, unreadNum;
                chatName = chatManager->chatAt(index)->chatName;
                unreadNum = QString::number(
                            chatManager->chatAt(index)->unreadNum);
                chatList->item(index)->setText(chatName +
                                               "(" + unreadNum + ")");
            }
        }
        else if(jsonDoc.object().value("message_type") == "group")
        {
            QString chatID, userID, card, messageString;
            QDateTime time;
            chatID = QString::number(jsonDoc.object().value("group_id")
                                     .toVariant().toLongLong());
            userID = QString::number(jsonDoc.object().value("user_id")
                                     .toVariant().toLongLong());
            card = jsonDoc.object().value("sender")
                    .toObject().value("card")
                    .toString();
            if(card.isEmpty())
            {
                card = jsonDoc.object().value("sender")
                        .toObject().value("nickname")
                        .toString();
            }
            messageString = jsonDoc.object().value("message")
                    .toString();
            time = QDateTime::fromTime_t(
                        uint(jsonDoc.object().value("time")
                             .toVariant().toLongLong()));
            int index = chatManager->indexOf(chatID, Chat::Group);
            if(index == -1)
            {
                QString chatName, avatar;
                Chat::SubType subType = Chat::SubNormal;
                // NeedToBeDone: 根据群号查询群名称
                chatName = "group";
                if(jsonDoc.object().value("sub_type") == "normal")
                {
                    subType = Chat::SubNormal;
                }
                else if(jsonDoc.object().value("sub_type") == "anonymous")
                {
                    subType = Chat::SubAnonymous;
                }
                else if(jsonDoc.object().value("sub_type") == "notice")
                {
                    subType = Chat::SubNotice;
                }
                else
                {
                    qDebug() << "unknown group sub_type";
                }
                chatManager->addNewChat(chatID,
                                        chatName,
                                        Chat::Group,
                                        subType);
                chatManager->chatAt(0)->addNewMessage(userID,
                                                      card,
                                                      messageString,
                                                      time);
                avatar = cacheManager->getAvatar(chatID,
                                                 CacheManager::Group,
                                                 100);
                chatList->addNewChatItem(avatar,
                                         chatID,
                                         chatName + "(1)");
            }
            else
            {
                chatManager->chatAt(index)->addNewMessage(userID,
                                                          card,
                                                          messageString,
                                                          time);
                QString chatName, unreadNum;
                chatName = chatManager->chatAt(index)->chatName;
                unreadNum = QString::number(
                            chatManager->chatAt(index)->unreadNum);
                chatList->item(index)->setText(chatName +
                                               "(" + unreadNum + ")");
            }
        }
        else if(jsonDoc.object().value("message_type") == "discuss")
        {
            QString chatID, userID, nickname, messageString;
            QDateTime time;
            chatID = QString::number(jsonDoc.object().value("discuss_id")
                                     .toVariant().toLongLong());
            userID = QString::number(jsonDoc.object().value("user_id")
                                     .toVariant().toLongLong());
            nickname = jsonDoc.object().value("sender")
                    .toObject().value("nickname")
                    .toString();
            messageString = jsonDoc.object().value("message")
                    .toString();
            time = QDateTime::fromTime_t(
                        uint(jsonDoc.object().value("time")
                             .toVariant().toLongLong()));
            int index = chatManager->indexOf(chatID, Chat::Group);
            if(index == -1)
            {
                QString chatName, avatar;
                Chat::SubType subType = Chat::SubNormal;
                // NeedToBeDone: 根据讨论组号查询名称
                chatName = "discuss";
                chatManager->addNewChat(chatID,
                                        chatName,
                                        Chat::Group,
                                        subType);
                chatManager->chatAt(0)->addNewMessage(userID,
                                                      nickname,
                                                      messageString,
                                                      time);
                avatar = cacheManager->getAvatar(chatID,
                                                 CacheManager::Group,
                                                 100);
                chatList->addNewChatItem(avatar,
                                         chatID,
                                         chatName + "(1)");
            }
            else
            {
                chatManager->chatAt(index)->addNewMessage(userID,
                                                          nickname,
                                                          messageString,
                                                          time);
                QString chatName, unreadNum;
                chatName = chatManager->chatAt(index)->chatName;
                unreadNum = QString::number(
                            chatManager->chatAt(index)->unreadNum);
                chatList->item(index)->setText(chatName +
                                               "(" + unreadNum + ")");
            }
        }
        else
        {
            qDebug() << "unknown message_type";
        }
    }
    else if(jsonDoc.object().value("post_type") == "notice")
    {
        qDebug() << "new notice"
                 << jsonDoc.object().value("notice_type")
                    .toString();
    }
    else if(jsonDoc.object().value("post_type") == "request")
    {
        qDebug() << "new request"
                 << jsonDoc.object().value("request_type")
                    .toString();
    }
    else
    {
        qDebug() << "unknown post_type";
    }
}

void WSConnection::sendHeartbeat()
{
    if(commandQueue.isEmpty())
    {
        QString content = "{\"action\":\"get_status\",\"params\":{}}";
        addCommand(CommandType::get_status, content);
    }
    heartbeatTimer->start(15000);
}
