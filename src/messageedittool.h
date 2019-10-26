#ifndef MESSAGEEDITTOOL_H
#define MESSAGEEDITTOOL_H

#include <QApplication>
#include <QClipboard>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>
#include "facedialog.h"
#include "unifiedicon.h"

// 聊天消息编辑工具栏
class MessageEditTool : public QWidget
{
    Q_OBJECT

public:
    MessageEditTool(QWidget *parent = nullptr);

signals:
    void insertFace(QString face);
    void sendImage(QString fileName);
    void sendScreenshot(QString fileName);
    void sendMessage();

private slots:
    void facePushButtonClicked();
    void facePushButtonFinished(QString face);
    void imagePushButtonClicked();
    void screenshotPushButtonClicked();
    void sendMessagePushButtonClicked();

private:
    QPushButton *facePushButton;
    QPushButton *imagePushButton;
    QPushButton *screenshotPushButton;
    QPushButton *sendMessagePushButton;
};

#endif // MESSAGEEDITTOOL_H
