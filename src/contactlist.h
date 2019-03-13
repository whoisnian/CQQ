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

    // 添加分类
    QTreeWidgetItem *addTopItem(QString text);
    // 添加分类下的子项，或者不属于任何分类的项目
    void addChildItem(QString text, QString toolTip, QString extra,
                      QTreeWidgetItem *parent, QString avatar);
};

#endif // CONTACTLIST_H
