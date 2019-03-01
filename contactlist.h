#ifndef CONTACTLIST_H
#define CONTACTLIST_H

#include <QHeaderView>
#include <QTreeWidget>
#include <QTreeWidgetItem>

// 主界面上的联系人列表
class ContactList : public QTreeWidget
{
public:
    ContactList(QWidget *parent = nullptr);
};

#endif // CONTACTLIST_H
