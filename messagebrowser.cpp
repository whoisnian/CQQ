#include "messagebrowser.h"

MessageBrowser::MessageBrowser(QWidget *parent)
{
    this->setParent(parent);
    this->setReadOnly(true);
    this->setOpenExternalLinks(true);
}
