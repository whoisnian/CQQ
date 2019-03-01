#ifndef MESSAGEBROWSER_H
#define MESSAGEBROWSER_H

#include <QTextBrowser>

// 聊天消息显示区域
class MessageBrowser : public QTextBrowser
{
public:
    MessageBrowser(QWidget *parent = nullptr);
};

#endif // MESSAGEBROWSER_H
