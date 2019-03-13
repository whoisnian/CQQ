#include "messageedit.h"

MessageEdit::MessageEdit(QWidget *parent)
{
    this->setParent(parent);
}

void MessageEdit::keyPressEvent(QKeyEvent *event)
{
    if((event->modifiers()&Qt::ControlModifier)
            &&(event->key() == Qt::Key_Enter||event->key() == Qt::Key_Return))
    {
        emit ctrlEnterPressed();
    }
    else
    {
        QPlainTextEdit::keyPressEvent(event);
    }
}
