#ifndef CHATLIST_H
#define CHATLIST_H

#include <QListWidget>
#include <QListWidgetItem>
#include <QMenu>
#include "chatmanager.h"

// 主界面上的聊天列表
class ChatList : public QListWidget
{
    Q_OBJECT

public:
    ChatList(QWidget *parent = nullptr);
    void addNewChatItem(QString avatar, QString chatID, QString chatName);

signals:
    void deleteItem(QListWidgetItem *item);

private slots:
    void showContextMenu(const QPoint &pos);
};

#endif // CHATLIST_H
