#ifndef MESSAGEBROWSER_H
#define MESSAGEBROWSER_H

#include <QApplication>
#include <QDateTime>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QScrollBar>
#include <QTextBrowser>
#include "cachemanager.h"
#include "chatlist.h"
#include "chatmanager.h"
#include "cqcode.h"
#include "unifiedicon.h"

// 聊天消息显示区域
class MessageBrowser : public QTextBrowser
{
    Q_OBJECT

public:
    MessageBrowser(QMainWindow *mainWindow, QWidget *parent = nullptr);
    void setColor(QString backgroundColor, QString foregroundColor);
    void setCacheManager(CacheManager *cacheManager);
    void setChatManager(ChatManager *chatManager);
    void setChatList(ChatList *chatList);

    QString curChatID;
    Chat::Type curChatType;
    QString foregroundColor;
    QString backgroundColor;

    enum ScrollType
    {
        ScrollToOldPosition,
        ScrollToUnreadAnchor,
        ScrollToEndAnchor
    };

public slots:
    void updateContent();

private slots:
    // 加载完图片之后重置图片宽度
    void resizeImage(QString filePath, QString url);
    // 鼠标点击图片或链接后在对应程序内打开
    void showClickedAnchor(const QUrl &link);
    // 鼠标移到图片上后显示预览图
    void showHighlighted(const QUrl &link);
    // 锁定并隐藏横向滚动条
    void disableQScrollBar(int value);
    // 判断并确定是否显示滚动到底部按钮
    void displayGoDownButton(int value);
    // 滚动到底部
    void scrollToBottom();

private:
    QMainWindow *mainWindow;
    QPushButton *goDownButton;
    CacheManager *cacheManager;
    ChatManager *chatManager;
    ChatList *chatList;
    QWidget *imageTooltip = nullptr;
};

#endif // MESSAGEBROWSER_H
