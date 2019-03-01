#include "chatlist.h"
#include <QDebug>

ChatList::ChatList(QWidget *parent)
{
    this->setParent(parent);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showContextMenu(const QPoint &)));
}

void ChatList::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = this->mapToGlobal(pos);
    QMenu contextMenu;
    contextMenu.addAction(QIcon::fromTheme("delete"), "删除");
    QAction *res = contextMenu.exec(globalPos);
    if(res&&res->text()=="删除")
    {
        emit deleteItem(this->itemAt(pos));
    }
}
