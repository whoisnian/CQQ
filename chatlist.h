#ifndef CHATLIST_H
#define CHATLIST_H

#include <QListWidget>
#include <QListWidgetItem>
#include <QMenu>

// 主界面上的聊天列表
class ChatList : public QListWidget
{
    Q_OBJECT

public:
    ChatList(QWidget *parent = nullptr);

signals:
    void deleteItem(QListWidgetItem *item);

private slots:
    void showContextMenu(const QPoint &pos);
};

#endif // CHATLIST_H
