#include "messagebrowser.h"

MessageBrowser::MessageBrowser(QWidget *parent)
{
    this->setParent(parent);
    this->setReadOnly(true);
    this->setOpenExternalLinks(true);
    curChatID = "";
    curChatType = Chat::Private;
}

void MessageBrowser::setCacheManager(CacheManager *cacheManager)
{
    this->cacheManager = cacheManager;
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
    QString lastChatID = curChatID;
    Chat::Type lastChatType = curChatType;
    curChatID = chatManager->chatAt(chatList->currentRow())->chatID;
    curChatType = chatManager->chatAt(chatList->currentRow())->type;
    if(lastChatID == curChatID&&lastChatType == curChatType)
    {
        return;
    }
    QString selfID = chatManager->selfID;
    QString temp = "";
    for(auto it : chatManager->chatAt(chatList->currentRow())->messages)
    {
        if(it.senderID == selfID)
        {
            temp.append("<p align=\"right\" style=\" margin-top:0px; margin-bottom:10px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#ffffff; background-color:#31363b;\">&nbsp;"
                                          + it.senderName +
                                          "&nbsp;</span><img src=\""
                                          + cacheManager->getAvatar(it.senderID, CacheManager::Friend, 100) +
                                          "\" width=\"30\" height=\"30\" style=\"vertical-align: top;\" /></p>");
            temp.append("<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:80px; margin-right:40px; -qt-block-indent:0; text-indent:0px;\">"
                                          + CQCode::ParseMessageFromString(it.messageString, curChatID) +
                                          "</p>");
        }
        else
        {
            temp.append("<p style=\" margin-top:0px; margin-bottom:10px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><img src=\""
                                          + cacheManager->getAvatar(it.senderID, CacheManager::Friend, 100) +
                                          "\" width=\"30\" height=\"30\" style=\"vertical-align: top;\" /><span style=\" color:#ffffff; background-color:#31363b;\">&nbsp;"
                                          + it.senderName +
                                          "&nbsp;</span></p>");
            temp.append("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:40px; margin-right:80px; -qt-block-indent:0; text-indent:0px;\">"
                                          + CQCode::ParseMessageFromString(it.messageString, curChatID) +
                                          "</p>");
        }
        temp.append("<hr />");
    }
    this->setHtml(temp);
}
