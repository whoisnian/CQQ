#ifndef MESSAGEEDIT_H
#define MESSAGEEDIT_H

#include <QKeyEvent>
#include <QPlainTextEdit>

// 聊天消息文本编辑框
class MessageEdit : public QPlainTextEdit
{
    Q_OBJECT

public:
    MessageEdit(QWidget *parent = nullptr);

signals:
    void ctrlEnterPressed();

protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // MESSAGEEDIT_H
