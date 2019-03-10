#include "messagebrowser.h"

MessageBrowser::MessageBrowser(QWidget *parent)
{
    this->setParent(parent);
    this->setReadOnly(true);
    this->setOpenExternalLinks(true);
    curChatID = "";
    curChatType = Chat::Private;
}

void MessageBrowser::setChatManager(ChatManager *chatManager)
{
    this->chatManager = chatManager;
}

void MessageBrowser::setChatList(ChatList *chatList)
{
    this->chatList = chatList;
}

void MessageBrowser::updateContent()
{

}
