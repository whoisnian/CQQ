#include "messagebrowser.h"

MessageBrowser::MessageBrowser(QWidget *parent)
{
    this->setParent(parent);
    this->setReadOnly(true);
    this->setOpenLinks(false);
    curChatID = "";
    curChatType = Chat::Private;
    connect(this, SIGNAL(anchorClicked(const QUrl &)),
            this, SLOT(showClickedAnchor(const QUrl &)));
    connect(this, SIGNAL(highlighted(const QUrl &)),
            this, SLOT(showHighlighted(const QUrl &)));
}

void MessageBrowser::setCacheManager(CacheManager *cacheManager)
{
    this->cacheManager = cacheManager;
    connect(cacheManager, SIGNAL(getImageFinished(QString, QString)),
            this, SLOT(resizeImage(QString, QString)));
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

void MessageBrowser::resizeImage(QString, QString)
{
    this->setLineWrapColumnOrWidth(this->lineWrapColumnOrWidth());
}

void MessageBrowser::showClickedAnchor(const QUrl &link)
{
    qDebug() << "showClickedAnchor" << link;
    if(imageTooltip != nullptr)
    {
        imageTooltip->hide();
        delete imageTooltip;
        imageTooltip = nullptr;
    }
    QDesktopServices::openUrl(link);
}

void MessageBrowser::showHighlighted(const QUrl &link)
{
    qDebug() << "showHighlighted" << link;
    if(link.isEmpty())
    {
        if(imageTooltip != nullptr)
        {
            imageTooltip->hide();
            delete imageTooltip;
            imageTooltip = nullptr;
        }
        return;
    }
    if(!link.isLocalFile())
    {
        return;
    }
    if(!QFileInfo::exists(link.toLocalFile())
            ||!QFileInfo(link.toLocalFile()).isFile())
    {
        return;
    }
    int maxw, maxh;
    maxw = maxh = 500;
    QImage image;
    image.load(link.toLocalFile());
    if(image.width() < 64&&image.height() < 64)
    {
        return;
    }
    if(image.width() > maxw||image.height() > maxh)
    {
        if(image.width() * maxh < image.height() * maxw)
        {
            image = image.scaledToHeight(maxh, Qt::SmoothTransformation);
        }
        else
        {
            image = image.scaledToWidth(maxw, Qt::SmoothTransformation);
        }
    }
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap::fromImage(image)));
    imageTooltip = new QWidget(nullptr, Qt::ToolTip);
    imageTooltip->setPalette(palette);
    imageTooltip->setAutoFillBackground(true);
    imageTooltip->setGeometry(QCursor::pos().x(), QCursor::pos().y(),
                              image.width(), image.height());
    imageTooltip->show();
}
