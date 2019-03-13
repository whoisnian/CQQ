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
    // 添加新聊天对象
    void addNewChatItem(QString avatar, QString chatID, QString chatName);

signals:
    // 删除聊天对象
    void deleteItem(QListWidgetItem *item);

private slots:
    // 右键菜单
    void showContextMenu(const QPoint &pos);
};

#endif // CHATLIST_H
