#include "messageedittool.h"

MessageEditTool::MessageEditTool(QWidget *parent)
{
    this->setParent(parent);
    QHBoxLayout *messageEditToolLayout = new QHBoxLayout(this);
    messageEditToolLayout->setMargin(0);
    messageEditToolLayout->setSpacing(0);

    facePushButton = new QPushButton(this);
    facePushButton->setStyleSheet("padding: 0px");
    facePushButton->setFlat(true);
    facePushButton->setIcon(UnifiedIcon::getIcon("face"));
    facePushButton->setIconSize(QSize(24, 24));
    facePushButton->setToolTip("插入表情");
    connect(facePushButton, SIGNAL(clicked()),
            this, SLOT(facePushButtonClicked()));
    messageEditToolLayout->addWidget(facePushButton);

    imagePushButton = new QPushButton(this);
    imagePushButton->setStyleSheet("padding: 0px");
    imagePushButton->setFlat(true);
    imagePushButton->setIcon(UnifiedIcon::getIcon("photo"));
    imagePushButton->setIconSize(QSize(24, 24));
    imagePushButton->setToolTip("发送图片");
    connect(imagePushButton, SIGNAL(clicked()),
            this, SLOT(imagePushButtonClicked()));
    messageEditToolLayout->addWidget(imagePushButton);

    screenshotPushButton = new QPushButton(this);
    screenshotPushButton->setStyleSheet("padding: 0px");
    screenshotPushButton->setFlat(true);
    screenshotPushButton->setIcon(UnifiedIcon::getIcon("cut"));
    screenshotPushButton->setIconSize(QSize(24, 24));
    screenshotPushButton->setToolTip("截图");
    connect(screenshotPushButton, SIGNAL(clicked()),
            this, SLOT(screenshotPushButtonClicked()));
    messageEditToolLayout->addWidget(screenshotPushButton);

    messageEditToolLayout->addStretch(1);

    sendMessagePushButton = new QPushButton(this);
    sendMessagePushButton->setIcon(UnifiedIcon::getIcon("send"));
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
    connect(dialog, SIGNAL(dialogFinished(QString)),
            this, SLOT(facePushButtonFinished(QString)));
    dialog->open();
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
    Screenshot *screenshot = new Screenshot();
    connect(screenshot, SIGNAL(saved(QString)),
            this, SLOT(screenshotPushButtonFinished(QString)));
    screenshot->shot();
}

void MessageEditTool::screenshotPushButtonFinished(QString filePath)
{
    if(!filePath.isEmpty())
    {
        if(QFileInfo::exists(filePath)&&QFileInfo(filePath).isFile())
        {
            emit sendImage(filePath);
        }
    }
}

void MessageEditTool::sendMessagePushButtonClicked()
{
    emit sendMessage();
}
