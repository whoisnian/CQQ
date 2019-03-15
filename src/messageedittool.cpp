#include "messageedittool.h"

MessageEditTool::MessageEditTool(QWidget *parent)
{
    this->setParent(parent);
    QHBoxLayout *messageEditToolLayout = new QHBoxLayout(this);
    messageEditToolLayout->setMargin(0);
    messageEditToolLayout->setSpacing(0);

    facePushButton = new QPushButton(this);
    facePushButton->setFlat(true);
    facePushButton->setIcon(QIcon::fromTheme("symbols"));
    facePushButton->setIconSize(QSize(24, 24));
    facePushButton->setToolTip("插入表情");
    connect(facePushButton, SIGNAL(clicked()),
            this, SLOT(facePushButtonClicked()));
    messageEditToolLayout->addWidget(facePushButton);

    imagePushButton = new QPushButton(this);
    imagePushButton->setFlat(true);
    imagePushButton->setIcon(QIcon::fromTheme("emblem-photos-symbolic"));
    imagePushButton->setIconSize(QSize(24, 24));
    imagePushButton->setToolTip("发送图片");
    connect(imagePushButton, SIGNAL(clicked()),
            this, SLOT(imagePushButtonClicked()));
    messageEditToolLayout->addWidget(imagePushButton);

    screenshotPushButton = new QPushButton(this);
    screenshotPushButton->setFlat(true);
    screenshotPushButton->setIcon(QIcon::fromTheme("edit-cut-symbolic"));
    screenshotPushButton->setIconSize(QSize(24, 24));
    screenshotPushButton->setToolTip("发送剪切版中的图片");
    connect(screenshotPushButton, SIGNAL(clicked()),
            this, SLOT(screenshotPushButtonClicked()));
    messageEditToolLayout->addWidget(screenshotPushButton);

    messageEditToolLayout->addStretch(1);

    sendMessagePushButton = new QPushButton(this);
    sendMessagePushButton->setIcon(QIcon::fromTheme("document-send"));
    sendMessagePushButton->setText("发送");
    sendMessagePushButton->setToolTip("发送（Ctrl + Enter）");
    connect(sendMessagePushButton, SIGNAL(clicked()),
            this, SLOT(sendMessagePushButtonClicked()));
    messageEditToolLayout->addWidget(sendMessagePushButton);

    this->setLayout(messageEditToolLayout);
}

void MessageEditTool::facePushButtonClicked()
{
    FaceDialog *dialog = new FaceDialog(this);
    dialog->open();
    connect(dialog, SIGNAL(dialogFinished(QString)),
            this, SLOT(facePushButtonFinished(QString)));
}

void MessageEditTool::facePushButtonFinished(QString face)
{
    emit insertFace(face);
}

void MessageEditTool::imagePushButtonClicked()
{
    QString fileName = QFileDialog::getOpenFileName(
                this,
                "选择要发送的图片",
                QDir::home().path(),
                "Images (*.png *.jpeg *.jpg *.gif)");
    if(!fileName.isEmpty())
    {
        if(QFileInfo::exists(fileName)&&QFileInfo(fileName).isFile())
        {
            emit sendImage(fileName);
        }
    }
}

void MessageEditTool::screenshotPushButtonClicked()
{
    QClipboard *systemClipboard = QApplication::clipboard();
    QString fileName = systemClipboard->text();
    if(fileName.left(7) == "file://")
        fileName = fileName.mid(7);
    if(!fileName.isEmpty())
    {
        if(QFileInfo::exists(fileName)&&QFileInfo(fileName).isFile())
        {
            emit sendScreenshot(fileName);
        }
    }
}

void MessageEditTool::sendMessagePushButtonClicked()
{
    emit sendMessage();
}
