#ifndef MESSAGEBROWSER_H
#define MESSAGEBROWSER_H

#include <QTextBrowser>
#include "chatlist.h"
#include "chatmanager.h"

// 聊天消息显示区域
class MessageBrowser : public QTextBrowser
{
    Q_OBJECT

public:
    MessageBrowser(QWidget *parent = nullptr);
    void setChatManager(ChatManager *chatManager);
    void setChatList(ChatList *chatList);
    void updateContent();

private:
    ChatManager *chatManager;
    ChatList *chatList;
    QString curChatID;
    Chat::Type curChatType;
};

#endif // MESSAGEBROWSER_H
