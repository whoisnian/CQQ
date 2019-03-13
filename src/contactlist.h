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

    QTreeWidgetItem *addTopItem(QString text);
    void addChildItem(QString text, QString toolTip, QString extra,
                      QTreeWidgetItem *parent, QString avatar);
};

#endif // CONTACTLIST_H
