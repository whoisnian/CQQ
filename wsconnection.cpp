#include "wsconnection.h"

WSConnection::WSConnection(QString address, QString token, CacheManager *cacheManager, QObject *parent)
    : QObject(parent)
{
    this->address = address;
    this->token = token;
    this->cacheManager = cacheManager;

    loop = new QEventLoop;
    heartbeatTimer = new QTimer;

    connect(&wsAPI, &QWebSocket::connected, this, &WSConnection::wsAPIConnected);
    connect(&wsAPI, &QWebSocket::disconnected, this, &WSConnection::wsAPIDisconnected);
    connect(&wsEVENT, &QWebSocket::connected, this, &WSConnection::wsEVENTConnected);
    connect(&wsEVENT, &QWebSocket::disconnected, this, &WSConnection::wsEVENTDisconnected);

    connect(cacheManager, &CacheManager::getCardSignal, this, &WSConnection::updateCard);

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
    loop->exit();
}

bool WSConnection::isConnected()
{
    if(wsAPI.state() == QAbstractSocket::ConnectedState&&wsEVENT.state() == QAbstractSocket::ConnectedState)
        return true;
    return false;
}

void WSConnection::updateContacts(QTreeWidget *friends, QTreeWidget *groups, QLabel *avatar, QLabel *number, QLabel *nickname, QLabel *remark)
{
    qDebug() << "updating contacts";
    friendsTreeWidget = friends;
    groupsTreeWidget = groups;
    infoAvatarLabel = avatar;
    infoNumberLabel = number;
    infoNicknameLabel = nickname;
    infoRemarkLabel = remark;

    connect(friendsTreeWidget, &QTreeWidget::itemClicked, this, &WSConnection::showFriendInfo);
    connect(friendsTreeWidget, &QTreeWidget::itemDoubleClicked, this, &WSConnection::startPrivateChat);
    connect(groupsTreeWidget, &QTreeWidget::itemClicked, this, &WSConnection::showGroupInfo);
    connect(groupsTreeWidget, &QTreeWidget::itemDoubleClicked, this, &WSConnection::startGroupChat);

    connect(cacheManager, &CacheManager::getAvatarFinished, this, &WSConnection::updateAvatar);

    if(commandQueue.isEmpty())
    {
        addCommand("_get_friend_list");
        addCommand("get_group_list");
        newStart = true;
        startNextCommand();
    }
    else
    {
        addCommand("_get_friend_list");
        addCommand("get_group_list");
    }
}

void WSConnection::getMeInfo(QString *id, QString *nickname)
{
    selfId = id;
    selfNickname = nickname;
    if(commandQueue.isEmpty())
    {
        addCommand("get_login_info");
        newStart = true;
        startNextCommand();
    }
    else
    {
        addCommand("get_login_info");
    }
}

void WSConnection::setChats(Chats *ch, QTabWidget *main, QListWidget *recentChats, QTextBrowser *textBbrowser, QWidget *edit, QTextEdit *messageTextEdit)
{
    chats = ch;
    mainTabWidget = main;
    recentChatsListWidget = recentChats;
    messageTextBrowser = textBbrowser;
    editWidget = edit;
    this->messageTextEdit = messageTextEdit;
    curChatId = "";
    curChatType = 0;
    for(int i = 0;i < chats->chats.size();i++)
    {
        QListWidgetItem *item = new QListWidgetItem;
        if(chats->chats.at(i).type == Chat::Private)
            item->setIcon(QIcon(cacheManager->getAvatar(chats->chats.at(i).senderId, CacheManager::FriendAvatar)));
        else
            item->setIcon(QIcon(cacheManager->getAvatar(chats->chats.at(i).senderId, CacheManager::GroupAvatar)));
        item->setText(chats->chats.at(i).senderName);
        item->setToolTip(chats->chats.at(i).senderId);
        recentChatsListWidget->addItem(item);
    }
    connect(recentChatsListWidget, &QListWidget::itemClicked, this, &WSConnection::updateChat);
}

void WSConnection::wsAPIConnected()
{
    qDebug() << "wsAPI connected";
    connect(&wsAPI, &QWebSocket::textMessageReceived, this, &WSConnection::wsAPIReceived);
    loop->exit();
    heartbeatTimer->start(15000);
    connect(heartbeatTimer, &QTimer::timeout, this, &WSConnection::wsAPIHeartbeat);
}

void WSConnection::wsAPIHeartbeat()
{
    if(commandQueue.isEmpty())
    {
        addCommand("get_status");
        newStart = true;
        startNextCommand();
    }
    heartbeatTimer->start(15000);
}

void WSConnection::wsAPIDisconnected()
{
    qDebug() << "wsAPI disconnected";
    wsAPI.close();
    loop->exit();
}

void WSConnection::wsAPIReceived(QString message)
{
    qDebug() << "wsAPI received:" << message;
    jsonDocFromwsAPI = QJsonDocument::fromJson(message.toLocal8Bit().data());

    if(commandQueue.head() == "get_status")
    {
        if(!jsonDocFromwsAPI.isNull())
        {
            qDebug() << "wsAPI status: " << (jsonDocFromwsAPI.object().value("data").toObject().value("good").toBool()?"OK":"ERROR");
        }
    }
    else if(commandQueue.head() == "send_msg")
    {
        if(!jsonDocFromwsAPI.isNull())
        {
            qDebug() << "message id: " << jsonDocFromwsAPI.object().value("data").toObject().value("message_id").toVariant().toLongLong();
        }
    }
    else if(commandQueue.head() == "_get_friend_list")
    {
        qDebug() << "friends list got";
        if(!jsonDocFromwsAPI.isNull())
        {
            QJsonValue tempValue = jsonDocFromwsAPI.object().value("data");
            if(tempValue.isArray())
            {
                QJsonArray tempArray = tempValue.toArray();
                for(int i = 0;i < tempArray.size();i++)
                {
                    QTreeWidgetItem *friendsItem = new QTreeWidgetItem;
                    friendsItem->setText(0, tempArray.at(i).toObject().value("friend_group_name").toString());
                    friendsTreeWidget->addTopLevelItem(friendsItem);
                    for(int j = 0;j < tempArray.at(i).toObject().value("friends").toArray().size();j++)
                    {
                        QTreeWidgetItem *friendItem = new QTreeWidgetItem;
                        cacheManager->nicknameMap[QString::number(tempArray.at(i).toObject().value("friends").toArray().at(j).toObject().value("user_id").toVariant().toLongLong())] = tempArray.at(i).toObject().value("friends").toArray().at(j).toObject().value("remark").toString();
                        friendItem->setIcon(0, QIcon(cacheManager->getAvatar(QString::number(tempArray.at(i).toObject().value("friends").toArray().at(j).toObject().value("user_id").toVariant().toLongLong()), CacheManager::FriendAvatar, 100)));
                        if(tempArray.at(i).toObject().value("friends").toArray().at(j).toObject().value("remark").toString().isEmpty())
                        {
                            friendItem->setText(0, tempArray.at(i).toObject().value("friends").toArray().at(j).toObject().value("nickname").toString());
                            cacheManager->remarkMap[QString::number(tempArray.at(i).toObject().value("friends").toArray().at(j).toObject().value("user_id").toVariant().toLongLong())] = tempArray.at(i).toObject().value("friends").toArray().at(j).toObject().value("nickname").toString();
                        }
                        else
                        {
                            friendItem->setText(0, tempArray.at(i).toObject().value("friends").toArray().at(j).toObject().value("remark").toString());
                            cacheManager->remarkMap[QString::number(tempArray.at(i).toObject().value("friends").toArray().at(j).toObject().value("user_id").toVariant().toLongLong())] = tempArray.at(i).toObject().value("friends").toArray().at(j).toObject().value("remark").toString();
                        }
                        friendItem->setToolTip(0, tempArray.at(i).toObject().value("friends").toArray().at(j).toObject().value("nickname").toString());
                        friendItem->setToolTip(1, QString::number(tempArray.at(i).toObject().value("friends").toArray().at(j).toObject().value("user_id").toVariant().toLongLong()));
                        friendsItem->addChild(friendItem);
                    }
                }
            }
        }
        else
        {
            qDebug() << "broken friends json";
        }
    }
    else if(commandQueue.head() == "get_group_list")
    {
        qDebug() << "groups got";
        if(!jsonDocFromwsAPI.isNull())
        {
            QJsonValue tempValue = jsonDocFromwsAPI.object().value("data");
            if(tempValue.isArray())
            {
                QJsonArray tempArray = tempValue.toArray();
                for(int i = 0;i < tempArray.size();i++)
                {
                    QTreeWidgetItem *groupItem = new QTreeWidgetItem;
                    cacheManager->groupnameMap[QString::number(tempArray.at(i).toObject().value("group_id").toVariant().toLongLong())] = tempArray.at(i).toObject().value("group_name").toString();
                    groupItem->setIcon(0, QIcon(cacheManager->getAvatar(QString::number(tempArray.at(i).toObject().value("group_id").toVariant().toLongLong()), CacheManager::GroupAvatar, 100)));
                    groupItem->setText(0, tempArray.at(i).toObject().value("group_name").toString());
                    groupItem->setToolTip(0, tempArray.at(i).toObject().value("group_name").toString());
                    groupItem->setToolTip(1, QString::number(tempArray.at(i).toObject().value("group_id").toVariant().toLongLong()));
                    groupsTreeWidget->addTopLevelItem(groupItem);
                }
            }
        }
        else
        {
            qDebug() << "broken groups json";
        }
    }
    else if(commandQueue.head() == "get_login_info")
    {
        qDebug() << "me info got";
        if(!jsonDocFromwsAPI.isNull())
        {
            QJsonValue tempValue = jsonDocFromwsAPI.object().value("data");
            *selfId = QString::number(tempValue.toObject().value("user_id").toVariant().toLongLong());
            *selfNickname = tempValue.toObject().value("nickname").toString();
            emit getMeInfoFinished();
        }
        else
        {
            qDebug() << "broken me info json";
        }
    }
    else if(commandQueue.head().split(' ', QString::SkipEmptyParts).length() == 3&&commandQueue.head().split(' ', QString::SkipEmptyParts)[0] == "get_group_member_info")
    {
        qDebug() << "group member info got";
        if(!jsonDocFromwsAPI.isNull())
        {
            QJsonValue tempValue = jsonDocFromwsAPI.object().value("data");
            if(tempValue.toObject().value("card").toString().isEmpty())
                cacheManager->cardMap[commandQueue.head().split(' ', QString::SkipEmptyParts)[1]+"_"+commandQueue.head().split(' ', QString::SkipEmptyParts)[2]] = tempValue.toObject().value("nickname").toString();
            else
                cacheManager->cardMap[commandQueue.head().split(' ', QString::SkipEmptyParts)[1]+"_"+commandQueue.head().split(' ', QString::SkipEmptyParts)[2]] = tempValue.toObject().value("card").toString();
        }
        else
        {
            qDebug() << "broken group member info json";
        }
    }
    startNextCommand();
}

void WSConnection::wsEVENTConnected()
{
    qDebug() << "wsEVENT connected";
    connect(&wsEVENT, &QWebSocket::textMessageReceived, this, &WSConnection::wsEVENTReceived);
    loop->exit();
}

void WSConnection::wsEVENTDisconnected()
{
    qDebug() << "wsEVENT disconnected";
    wsEVENT.close();
    loop->exit();
}

void WSConnection::wsEVENTReceived(QString message)
{
    qDebug() << "wsEVENT received:" << message;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(message.toLocal8Bit().data());
    if(jsonDoc.isNull())
    {
        qDebug() << "broken message json";
        return;
    }
    //wsEVENT.sendTextMessage("");

    if(jsonDoc.object().value("post_type") == "message")
    {
        if(jsonDoc.object().value("message_type") == "private")
        {
            Message m;
            m.senderId = QString::number(jsonDoc.object().value("user_id").toVariant().toLongLong());
            m.senderName = cacheManager->remarkMap[QString::number(jsonDoc.object().value("user_id").toVariant().toLongLong())];
            m.haveRead = false;
            m.messageString = jsonDoc.object().value("message").toString();
            preloadCheck(m.messageString);
            m.time = QDateTime::fromTime_t(uint(jsonDoc.object().value("time").toVariant().toLongLong()));
            for(auto it = chats->chats.begin();it < chats->chats.end();it++)
            {
                if(it->type == Chat::Private&&it->senderId == m.senderId)
                {
                    it->sumNum += 1;
                    it->unreadNum += 1;
                    it->messages.push_back(m);
                    recentChatsListWidget->item(it-chats->chats.begin())->setText(it->senderName+"("+QString::number(it->unreadNum)+")");
                    if(curChatId == it->senderId&&curChatType == it->type)
                        updateChat();
                    return;
                }
            }

            Chat ch;
            ch.type = Chat::Private;
            ch.sumNum = 1;
            ch.unreadNum = 1;
            ch.senderId = m.senderId;
            ch.messages.push_back(m);
            if(jsonDoc.object().value("sub_type") == "friend")
            {
                ch.senderName = m.senderName;
                ch.subType = Chat::FriendSub;
            }
            else if(jsonDoc.object().value("sub_type") == "group")
            {
                ch.senderName = m.senderName+"(group)";
                ch.subType = Chat::GroupSub;
            }
            else if(jsonDoc.object().value("sub_type") == "discuss")
            {
                ch.senderName = m.senderName+"(discuss)";
                ch.subType = Chat::DiscussSub;
            }
            else if(jsonDoc.object().value("sub_type") == "other")
            {
                ch.senderName = m.senderName+"(other)";
                ch.subType = Chat::OtherSub;
            }
            else
            {
                qDebug() << "unknown private sub_type";
                return;
            }
            chats->chats.push_front(ch);
            QListWidgetItem *newItem = new QListWidgetItem;
            newItem->setIcon(QIcon(cacheManager->getAvatar(ch.senderId, CacheManager::FriendAvatar)));
            newItem->setText(ch.senderName+"("+QString::number(ch.unreadNum)+")");
            newItem->setToolTip(ch.senderId);
            recentChatsListWidget->insertItem(0, newItem);
            return;
        }
        else if(jsonDoc.object().value("message_type") == "group")
        {
            Message m;
            m.senderId = QString::number(jsonDoc.object().value("user_id").toVariant().toLongLong());
            if(jsonDoc.object().value("sender").toObject().value("card").toString().isEmpty())
                m.senderName = jsonDoc.object().value("sender").toObject().value("nickname").toString();
            else
                m.senderName = jsonDoc.object().value("sender").toObject().value("card").toString();
            m.haveRead = false;
            m.messageString = jsonDoc.object().value("message").toString();
            m.time = QDateTime::fromTime_t(uint(jsonDoc.object().value("time").toVariant().toLongLong()));
            QString groupId = QString::number(jsonDoc.object().value("group_id").toVariant().toLongLong());
            preloadCheck(m.messageString, groupId);
            cacheManager->cardMap[QString::number(jsonDoc.object().value("group_id").toVariant().toLongLong()) + "_" + QString::number(jsonDoc.object().value("user_id").toVariant().toLongLong())] = m.senderName;
            for(auto it = chats->chats.begin();it < chats->chats.end();it++)
            {
                if(it->type == Chat::Group&&it->senderId == groupId)
                {
                    it->sumNum += 1;
                    it->unreadNum += 1;
                    it->messages.push_back(m);
                    recentChatsListWidget->item(it-chats->chats.begin())->setText(it->senderName+"("+QString::number(it->unreadNum)+")");
                    if(curChatId == it->senderId&&curChatType == it->type)
                        updateChat();
                    return;
                }
            }

            Chat ch;
            ch.type = Chat::Group;
            ch.sumNum = 1;
            ch.unreadNum = 1;
            ch.senderId = groupId;
            ch.messages.push_back(m);
            ch.senderName = cacheManager->groupnameMap[groupId];
            if(jsonDoc.object().value("sub_type") == "normal")
            {
                ch.subType = Chat::normalSub;
            }
            else if(jsonDoc.object().value("sub_type") == "anonymous")
            {
                ch.subType = Chat::anonymousSub;
            }
            else if(jsonDoc.object().value("sub_type") == "notice")
            {
                ch.subType = Chat::noticeSub;
            }
            else
            {
                qDebug() << "unknown group sub_type";
                return;
            }
            chats->chats.push_front(ch);
            QListWidgetItem *newItem = new QListWidgetItem;
            newItem->setIcon(QIcon(cacheManager->getAvatar(ch.senderId, CacheManager::GroupAvatar)));
            newItem->setText(ch.senderName+"("+QString::number(ch.unreadNum)+")");
            newItem->setToolTip(ch.senderId);
            recentChatsListWidget->insertItem(0, newItem);
            return;
        }
        else if(jsonDoc.object().value("message_type") == "discuss")
        {
            Message m;
            m.senderId = QString::number(jsonDoc.object().value("user_id").toVariant().toLongLong());
            m.senderName = jsonDoc.object().value("sender").toObject().value("nickname").toString();
            m.haveRead = false;
            m.messageString = jsonDoc.object().value("message").toString();
            m.time = QDateTime::fromTime_t(uint(jsonDoc.object().value("time").toVariant().toLongLong()));
            QString discussId = QString::number(jsonDoc.object().value("discuss_id").toVariant().toLongLong());
            for(auto it = chats->chats.begin();it < chats->chats.end();it++)
            {
                if(it->type == Chat::Group&&it->senderId == discussId)
                {
                    it->sumNum += 1;
                    it->unreadNum += 1;
                    it->messages.push_back(m);
                    recentChatsListWidget->item(it-chats->chats.begin())->setText(it->senderName+"("+QString::number(it->unreadNum)+")");
                    if(curChatId == it->senderId&&curChatType == it->type)
                        updateChat();
                    return;
                }
            }

            Chat ch;
            ch.type = Chat::Group;
            ch.sumNum = 1;
            ch.unreadNum = 1;
            ch.senderId = discussId;
            ch.messages.push_back(m);
            ch.senderName = cacheManager->groupnameMap[discussId];
            ch.subType = Chat::normalSub;
            chats->chats.push_front(ch);
            QListWidgetItem *newItem = new QListWidgetItem;
            newItem->setIcon(QIcon(cacheManager->getAvatar(ch.senderId, CacheManager::GroupAvatar)));
            newItem->setText(ch.senderName+"("+QString::number(ch.unreadNum)+")");
            newItem->setToolTip(ch.senderId);
            recentChatsListWidget->insertItem(0, newItem);
            return;
        }
        else
        {
            qDebug() << "unknown message_type";
        }
    }
    else if(jsonDoc.object().value("post_type") == "notice")
    {
        qDebug() << "new notice" << jsonDoc.object().value("notice_type").toString();
    }
    else if(jsonDoc.object().value("post_type") == "request")
    {
        qDebug() << "new request" << jsonDoc.object().value("request_type").toString();
    }
    else if(jsonDoc.object().value("post_type") == "meta_event")
    {
        if(jsonDoc.object().value("meta_event_type") == "heartbeat")
        {
            qDebug() << "wsEVENT status: " << (jsonDoc.object().value("status").toObject().value("good").toBool()?"OK":"ERROR");
        }
    }
    else
    {
        qDebug() << "unknown post_type";
    }
}

void WSConnection::showFriendInfo(QTreeWidgetItem *item, int column)
{
    qDebug() << "clicked" << item->text(column);
    if(item->toolTip(1).isEmpty()||item->toolTip(1) == infoNumberLabel->text())
        return;
    infoAvatarLabel->setText("<img src=\""+cacheManager->getAvatar(item->toolTip(1), CacheManager::FriendAvatar, 100)+"\" />");
    infoNumberLabel->setText(item->toolTip(1));
    infoNicknameLabel->setText("昵称：" + item->toolTip(0));
    infoRemarkLabel->setText("备注：" + item->text(0));
}

void WSConnection::showGroupInfo(QTreeWidgetItem *item, int column)
{
    qDebug() << "clicked" << item->text(column);
    if(item->toolTip(1).isEmpty()||item->toolTip(1) == infoNumberLabel->text())
        return;
    infoAvatarLabel->setText("<img src=\""+cacheManager->getAvatar(item->toolTip(1), CacheManager::GroupAvatar, 100)+"\" />");
    infoNumberLabel->setText(item->toolTip(1));
    infoNicknameLabel->setText("名称：" + item->toolTip(0));
    infoRemarkLabel->setText("");
}

void WSConnection::startPrivateChat(QTreeWidgetItem *item, int column)
{
    qDebug() << "double clicked" << item->text(column);
    if(item->toolTip(1).isEmpty())
        return;
    for(auto it = chats->chats.begin();it < chats->chats.end();it++)
    {
        if(it->type == Chat::Private&&it->senderId == item->toolTip(1))
        {
            recentChatsListWidget->setCurrentRow(it - chats->chats.begin());
            mainTabWidget->setCurrentIndex(0);
            updateChat();
            return;
        }
    }

    Chat ch;
    ch.type = Chat::Private;
    ch.sumNum = 0;
    ch.unreadNum = 0;
    ch.subType = Chat::FriendSub;
    ch.senderId = item->toolTip(1);
    ch.senderName = item->text(0);
    ch.readyMessage = "";
    chats->chats.push_front(ch);

    QListWidgetItem *newItem = new QListWidgetItem;
    newItem->setIcon(QIcon(cacheManager->getAvatar(ch.senderId, CacheManager::FriendAvatar)));
    newItem->setText(ch.senderName);
    newItem->setToolTip(ch.senderId);
    recentChatsListWidget->insertItem(0, newItem);
    recentChatsListWidget->setCurrentRow(0);

    mainTabWidget->setCurrentIndex(0);
    updateChat();
}

void WSConnection::startGroupChat(QTreeWidgetItem *item, int column)
{
    qDebug() << "double clicked" << item->text(column);
    if(item->toolTip(1).isEmpty())
        return;
    for(auto it = chats->chats.begin();it < chats->chats.end();it++)
    {
        if(it->type == Chat::Group&&it->senderId == item->toolTip(1))
        {
            recentChatsListWidget->setCurrentRow(it - chats->chats.begin());
            mainTabWidget->setCurrentIndex(0);
            updateChat();
            return;
        }
    }

    Chat ch;
    ch.type = Chat::Group;
    ch.sumNum = 0;
    ch.unreadNum = 0;
    ch.subType = Chat::normalSub;
    ch.senderId = item->toolTip(1);
    ch.senderName = item->text(0);
    ch.readyMessage = "";
    chats->chats.push_front(ch);

    QListWidgetItem *newItem = new QListWidgetItem;
    newItem->setIcon(QIcon(cacheManager->getAvatar(ch.senderId, CacheManager::GroupAvatar)));
    newItem->setText(ch.senderName);
    newItem->setToolTip(ch.senderId);
    recentChatsListWidget->insertItem(0, newItem);
    recentChatsListWidget->setCurrentRow(0);

    mainTabWidget->setCurrentIndex(0);
    updateChat();
}

void WSConnection::updateAvatar()
{
    QTreeWidgetItemIterator it1(friendsTreeWidget);
    while(*it1)
    {
        if(!(*it1)->toolTip(1).isEmpty())
            (*it1)->setIcon(0, QIcon(cacheManager->getAvatar((*it1)->toolTip(1), CacheManager::FriendAvatar, 100)));
        it1++;
    }
    QTreeWidgetItemIterator it2(groupsTreeWidget);
    while(*it2)
    {
        if(!(*it2)->toolTip(1).isEmpty())
            (*it2)->setIcon(0, QIcon(cacheManager->getAvatar((*it2)->toolTip(1), CacheManager::GroupAvatar, 100)));
        it2++;
    }
}

void WSConnection::updateChat()
{
    if(editWidget->isHidden())
        editWidget->show();
    if(messageTextBrowser->isHidden())
        messageTextBrowser->show();
    if(chats->chats.at(recentChatsListWidget->currentRow()).unreadNum == 0&&chats->chats.at(recentChatsListWidget->currentRow()).senderId == curChatId&&chats->chats.at(recentChatsListWidget->currentRow()).type == curChatType)
        return;
    for(auto it = chats->chats.begin();it < chats->chats.end();it++)
    {
        if(it->senderId == curChatId&&it->type == curChatType)
        {
            it->readyMessage = messageTextEdit->toHtml();
            break;
        }
    }
    bool needScroll = false;
    if(chats->chats.at(recentChatsListWidget->currentRow()).senderId != curChatId||chats->chats.at(recentChatsListWidget->currentRow()).type != curChatType)
        needScroll = true;
    curChatId = chats->chats.at(recentChatsListWidget->currentRow()).senderId;
    curChatType = chats->chats.at(recentChatsListWidget->currentRow()).type;
    if(curChatType == Chat::Group)
        cacheManager->getCard(*selfId, curChatId);
    int oldValue = messageTextBrowser->verticalScrollBar()->value();
    int oldMaxValue = messageTextBrowser->verticalScrollBar()->maximum();
    QString temp = "";
    for(auto it = chats->chats.at(recentChatsListWidget->currentRow()).messages.begin();it < chats->chats.at(recentChatsListWidget->currentRow()).messages.end();it++)
    {
        if(chats->chats.at(recentChatsListWidget->currentRow()).unreadNum > 1&&it-chats->chats.at(recentChatsListWidget->currentRow()).messages.begin() == chats->chats.at(recentChatsListWidget->currentRow()).sumNum-chats->chats.at(recentChatsListWidget->currentRow()).unreadNum)
            temp.append("<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
                        "<a name=\"UnreadMessages\">⬤&nbsp;未读消息&nbsp;⬤</a>"
                        "</p><hr />");
        if(it->senderId == selfId)
        {
            temp.append("<p align=\"right\" style=\" margin-top:0px; margin-bottom:10px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#ffffff; background-color:#31363b;\">&nbsp;"
                                          + it->senderName +
                                          "&nbsp;</span><img src=\""
                                          + cacheManager->getAvatar(it->senderId, CacheManager::FriendAvatar) +
                                          "\" width=\"30\" height=\"30\" style=\"vertical-align: top;\" /></p>");
            temp.append("<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:80px; margin-right:40px; -qt-block-indent:0; text-indent:0px;\">"
                                          + CQCode::ParseMessageFromString(it->messageString, curChatId) +
                                          "</p>");
        }
        else
        {
            temp.append("<p style=\" margin-top:0px; margin-bottom:10px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><img src=\""
                                          + cacheManager->getAvatar(it->senderId, CacheManager::FriendAvatar) +
                                          "\" width=\"30\" height=\"30\" style=\"vertical-align: top;\" /><span style=\" color:#ffffff; background-color:#31363b;\">&nbsp;"
                                          + it->senderName +
                                          "&nbsp;</span></p>");
            temp.append("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:40px; margin-right:80px; -qt-block-indent:0; text-indent:0px;\">"
                                          + CQCode::ParseMessageFromString(it->messageString, curChatId) +
                                          "</p>");
        }
        temp.append("<hr />");
    }
    messageTextBrowser->setHtml(temp);
    if(needScroll)
    {
        if(chats->chats[recentChatsListWidget->currentRow()].unreadNum > 1)
            messageTextBrowser->scrollToAnchor("UnreadMessages");
        else
            messageTextBrowser->verticalScrollBar()->setValue(messageTextBrowser->verticalScrollBar()->maximum());
    }
    else
    {
        if(oldValue == oldMaxValue)
            messageTextBrowser->verticalScrollBar()->setValue(messageTextBrowser->verticalScrollBar()->maximum());
        else
            messageTextBrowser->verticalScrollBar()->setValue(oldValue);
    }
    chats->chats[recentChatsListWidget->currentRow()].unreadNum = 0;
    recentChatsListWidget->item(recentChatsListWidget->currentRow())->setText(chats->chats.at(recentChatsListWidget->currentRow()).senderName);
    messageTextEdit->setHtml(chats->chats.at(recentChatsListWidget->currentRow()).readyMessage);
}

void WSConnection::updateCard(QString user_id, QString group_id)
{
    if(commandQueue.isEmpty())
    {
        addCommand("get_group_member_info " + group_id + " " + user_id);
        newStart = true;
        startNextCommand();
    }
    else
    {
        addCommand("get_group_member_info " + group_id + " " + user_id);
    }
}

void WSConnection::endChat()
{
    for(auto it = chats->chats.begin();it < chats->chats.end();it++)
    {
        if(it->senderId == curChatId&&it->type == curChatType)
        {
            it->readyMessage = messageTextEdit->toHtml();
            break;
        }
    }
}

void WSConnection::preloadCheck(QString message, QString group_id)
{
    CQCode::ParseMessageFromString(message, group_id);
}

void WSConnection::sendTextMessage(QString textMessage, int messageType, QString id)
{
    qDebug() << textMessage << messageType << id;
    Message m;
    m.time = QDateTime::currentDateTime();
    m.senderId = *selfId;
    m.messageString = CQCode::EncodeToCQCodeText(textMessage);
    m.haveRead = true;
    chats->chats[recentChatsListWidget->currentRow()].sumNum += 1;
    chats->chats[recentChatsListWidget->currentRow()].unreadNum += 1;

    QString data = "";
    if(messageType== Chat::Private)
    {
        m.senderName = *selfNickname;
        data = "{\"action\":\"send_private_msg\",\"params\":{\"user_id\":" + id + ",\"message\":\"" + CQCode::EncodeToCQCodeJsonText(textMessage) + "\",\"auto_escape\":false}}";
    }
    else if(messageType == Chat::Group)
    {
        m.senderName = cacheManager->getCard(*selfId, id);
        data = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":" + id + ",\"message\":\"" + CQCode::EncodeToCQCodeJsonText(textMessage) + "\",\"auto_escape\":false}}";
    }
    chats->chats[recentChatsListWidget->currentRow()].messages.push_back(m);
    updateChat();

    if(commandQueue.isEmpty())
    {
        addCommand("send_msg");
        wsAPI.sendTextMessage(data);
        newStart = true;
        startNextCommand();
    }
    else
    {
        addCommand("send_msg");
        wsAPI.sendTextMessage(data);
    }
}

void WSConnection::sendImage(QString fileName, int messageType, QString id)
{
    qDebug() << fileName << messageType << id;
    Message m;
    m.time = QDateTime::currentDateTime();
    m.senderId = *selfId;
    m.messageString = "[发送图片]";
    m.haveRead = true;
    chats->chats[recentChatsListWidget->currentRow()].sumNum += 1;
    chats->chats[recentChatsListWidget->currentRow()].unreadNum += 1;

    QString data = "";
    if(messageType== Chat::Private)
    {
        m.senderName = *selfNickname;
        data = "{\"action\":\"send_private_msg\",\"params\":{\"user_id\":" + id + ",\"message\":\"" + CQCode::EncodeImageToCQCode(fileName) + "\",\"auto_escape\":false}}";
    }
    else if(messageType == Chat::Group)
    {
        m.senderName = cacheManager->getCard(*selfId, id);
        data = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":" + id + ",\"message\":\"" + CQCode::EncodeImageToCQCode(fileName) + "\",\"auto_escape\":false}}";
    }
    chats->chats[recentChatsListWidget->currentRow()].messages.push_back(m);
    updateChat();

    if(commandQueue.isEmpty())
    {
        addCommand("send_msg");
        wsAPI.sendTextMessage(data);
        newStart = true;
        startNextCommand();
    }
    else
    {
        addCommand("send_msg");
        wsAPI.sendTextMessage(data);
    }
}

void WSConnection::sendFace(QString faceId, int messageType, QString id)
{
    qDebug() << faceId << messageType << id;
    Message m;
    m.time = QDateTime::currentDateTime();
    m.senderId = *selfId;
    m.messageString = "[CQ:face,id="+faceId+"]";
    qDebug() << m.messageString;
    m.haveRead = true;
    chats->chats[recentChatsListWidget->currentRow()].sumNum += 1;
    chats->chats[recentChatsListWidget->currentRow()].unreadNum += 1;

    QString data = "";
    if(messageType== Chat::Private)
    {
        m.senderName = *selfNickname;
        data = "{\"action\":\"send_private_msg\",\"params\":{\"user_id\":" + id + ",\"message\":\"" + m.messageString + "\",\"auto_escape\":false}}";
    }
    else if(messageType == Chat::Group)
    {
        m.senderName = cacheManager->getCard(*selfId, id);
        data = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":" + id + ",\"message\":\"" + m.messageString + "\",\"auto_escape\":false}}";
    }
    chats->chats[recentChatsListWidget->currentRow()].messages.push_back(m);
    updateChat();

    if(commandQueue.isEmpty())
    {
        addCommand("send_msg");
        wsAPI.sendTextMessage(data);
        newStart = true;
        startNextCommand();
    }
    else
    {
        addCommand("send_msg");
        wsAPI.sendTextMessage(data);
    }
}

void WSConnection::addCommand(QString command)
{
    commandQueue.enqueue(command);
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

    QString command = commandQueue.head();
    if(command == "get_status")
    {
        wsAPI.sendTextMessage(QStringLiteral("{\"action\":\"get_status\",\"params\":{}}"));
    }
    else if(command == "_get_friend_list")
    {
        wsAPI.sendTextMessage(QStringLiteral("{\"action\":\"_get_friend_list\",\"params\":{\"flat\":false}}"));
    }
    else if(command == "get_group_list")
    {
        wsAPI.sendTextMessage(QStringLiteral("{\"action\":\"get_group_list\",\"params\":{}}"));
    }
    else if(command == "get_login_info")
    {
        wsAPI.sendTextMessage(QStringLiteral("{\"action\":\"get_login_info\",\"params\":{}}"));
    }
    else if(command.split(' ', QString::SkipEmptyParts).length() == 3&&command.split(' ', QString::SkipEmptyParts)[0] == "get_group_member_info")
    {
        wsAPI.sendTextMessage("{\"action\":\"get_group_member_info\",\"params\":{\"group_id\":" + command.split(' ', QString::SkipEmptyParts)[1] + ",\"user_id\":" + command.split(' ', QString::SkipEmptyParts)[2] + "}}");
    }
}
