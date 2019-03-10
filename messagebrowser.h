#ifndef MESSAGEBROWSER_H
#define MESSAGEBROWSER_H

#include <QTextBrowser>
#include "cachemanager.h"
#include "chatlist.h"
#include "chatmanager.h"
#include "cqcode.h"

// 聊天消息显示区域
class MessageBrowser : public QTextBrowser
{
    Q_OBJECT

public:
    MessageBrowser(QWidget *parent = nullptr);
    void setCacheManager(CacheManager *cacheManager);
    void setChatManager(ChatManager *chatManager);
    void setChatList(ChatList *chatList);
    void updateContent();

    QString curChatID;
    Chat::Type curChatType;

private:
    CacheManager *cacheManager;
    ChatManager *chatManager;
    ChatList *chatList;
};

#endif // MESSAGEBROWSER_H
