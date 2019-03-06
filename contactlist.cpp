#include "contactlist.h"

ContactList::ContactList(QWidget *parent)
{
    this->setParent(parent);
    this->header()->hide();
}

QTreeWidgetItem *ContactList::addTopItem(QString text)
{
    QTreeWidgetItem *topItem = new QTreeWidgetItem;
    topItem->setText(0, text);
    this->addTopLevelItem(topItem);
    return topItem;
}

void ContactList::addChildItem(QString text, QString toolTip, QString extra,
                               QTreeWidgetItem *parent, QString avatar)
{
    QTreeWidgetItem *childItem = new QTreeWidgetItem;
    if(!avatar.isEmpty())
    {
        childItem->setIcon(0, QIcon(avatar));
    }
    childItem->setText(0, text);
    childItem->setToolTip(0, toolTip);
    childItem->setToolTip(1, extra);
    if(parent != nullptr)
    {
        parent->addChild(childItem);
    }
    else
    {
        this->addTopLevelItem(childItem);
    }
}
