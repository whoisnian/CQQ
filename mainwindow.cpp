#include "mainwindow.h"

CacheManager *CQCode::cacheManager = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ConfigAddress = QString("110.110.110.110:6700");
    ConfigToken = QString("qwertyuiop");

    ConfigCachePath = QString("/home/test/CQQ/cache/");
    cacheManager = new CacheManager(ConfigCachePath);

    this->setWindowIcon(QIcon::fromTheme("im-qq"));
    this->resize(800, 600);
    this->statusBar()->clearMessage();

    mainTabWidget = new QTabWidget;
    mainTabWidget->setIconSize(QSize(64, 16));
    this->setCentralWidget(mainTabWidget);

    recentMessagesPage = new QWidget;
    mainTabWidget->addTab(recentMessagesPage, QIcon::fromTheme("message-new"), "");

    recentChatsListWidget = new QListWidget;
    QWidget *messagesWidget = new QWidget;
    recentChatsListWidget->setMinimumWidth(200);
    messagesWidget->setMinimumWidth(300);
    QSizePolicy sizePolicyMessagesWidget = messagesWidget->sizePolicy();
    sizePolicyMessagesWidget.setHorizontalStretch(1);
    messagesWidget->setSizePolicy(sizePolicyMessagesWidget);

    messageTextBrowser = new QTextBrowser;
    messageTextBrowser->setReadOnly(true);
    messageTextBrowser->setOpenExternalLinks(true);
    QSizePolicy sizePolicyMessageListWidget = messageTextBrowser->sizePolicy();
    sizePolicyMessageListWidget.setVerticalStretch(1);
    messageTextBrowser->setSizePolicy(sizePolicyMessageListWidget);
    editWidget = new QWidget;
    editWidget->setMinimumHeight(150);

    QVBoxLayout *layoutEdit = new QVBoxLayout;
    layoutEdit->setMargin(0);
    layoutEdit->setSpacing(0);

    QWidget *toolButtonsWidget = new QWidget;
    toolButtonsWidget->setMaximumHeight(40);
    layoutEdit->addWidget(toolButtonsWidget);

    QHBoxLayout *layoutToolButtons = new QHBoxLayout;
    layoutToolButtons->setMargin(0);
    layoutToolButtons->setSpacing(0);

    facePushButton = new QPushButton;
    facePushButton->setFlat(true);
    facePushButton->setIcon(QIcon::fromTheme("smiley-shape"));
    facePushButton->setIconSize(QSize(24, 24));
    facePushButton->setToolTip("插入emoji");
    connect(facePushButton, &QPushButton::clicked, this, &MainWindow::sendFace);
    layoutToolButtons->addWidget(facePushButton);

    picturePushButton = new QPushButton;
    picturePushButton->setFlat(true);
    picturePushButton->setIcon(QIcon::fromTheme("emblem-photos-symbolic"));
    picturePushButton->setIconSize(QSize(24, 24));
    picturePushButton->setToolTip("发送图片");
    connect(picturePushButton, &QPushButton::clicked, this, &MainWindow::sendPicture);
    layoutToolButtons->addWidget(picturePushButton);

    screenshotPushButton =  new QPushButton;
    screenshotPushButton->setFlat(true);
    screenshotPushButton->setIcon(QIcon::fromTheme("edit-cut-symbolic"));
    screenshotPushButton->setIconSize(QSize(24, 24));
    screenshotPushButton->setToolTip("发送剪切版中的截图");
    connect(screenshotPushButton, &QPushButton::clicked, this, &MainWindow::sendScreenshot);
    layoutToolButtons->addWidget(screenshotPushButton);

    QSpacerItem *spacerItemTool = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    layoutToolButtons->addSpacerItem(spacerItemTool);

    sendPushButton = new QPushButton;
    sendPushButton->setIcon(QIcon::fromTheme("document-send"));
    sendPushButton->setText("发送");
    sendPushButton->setToolTip("发送");
    connect(sendPushButton, &QPushButton::clicked, this, &MainWindow::sendTextMessage);
    layoutToolButtons->addWidget(sendPushButton);

    toolButtonsWidget->setLayout(layoutToolButtons);

    messageTextEdit = new QTextEdit;
    QSizePolicy sizePolicyMessageTextEdit = messageTextEdit->sizePolicy();
    sizePolicyMessageTextEdit.setVerticalPolicy(QSizePolicy::Ignored);
    messageTextEdit->setSizePolicy(sizePolicyMessageTextEdit);
    layoutEdit->addWidget(messageTextEdit);
    editWidget->setLayout(layoutEdit);

    QSplitter *messageSplitter = new QSplitter;
    messageSplitter->addWidget(messageTextBrowser);
    messageSplitter->addWidget(editWidget);
    messageSplitter->setOrientation(Qt::Vertical);
    messageSplitter->setChildrenCollapsible(false);
    QHBoxLayout *layoutMessages = new QHBoxLayout;
    layoutMessages->setMargin(0);
    layoutMessages->setSpacing(0);
    layoutMessages->addWidget(messageSplitter);
    messagesWidget->setLayout(layoutMessages);

    QSplitter *recentSplitter = new QSplitter;
    recentSplitter->addWidget(recentChatsListWidget);
    recentSplitter->addWidget(messagesWidget);
    recentSplitter->setOrientation(Qt::Horizontal);
    recentSplitter->setChildrenCollapsible(false);
    QHBoxLayout *layoutRecentMessagesPage = new QHBoxLayout;
    layoutRecentMessagesPage->addWidget(recentSplitter);
    recentMessagesPage->setLayout(layoutRecentMessagesPage);

    messageTextBrowser->hide();
    editWidget->hide();

    contactsPage = new QWidget;
    mainTabWidget->addTab(contactsPage, QIcon::fromTheme("group"), "");

    contactsTabWidget = new QTabWidget;
    QWidget *infoWidgets = new QWidget;
    QWidget *infoWidgetL = new QWidget;
    QWidget *infoWidget = new QWidget;
    QWidget *infoWidgetR = new QWidget;

    contactsTabWidget->setMinimumWidth(200);
    infoWidgets->setMinimumWidth(300);

    QHBoxLayout *layoutL = new QHBoxLayout;
    QSpacerItem *spacerItemL = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    layoutL->addSpacerItem(spacerItemL);
    infoWidgetL->setLayout(layoutL);

    QHBoxLayout *layoutR = new QHBoxLayout;
    QSpacerItem *spacerItemR = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    layoutR->addSpacerItem(spacerItemR);
    infoWidgetR->setLayout(layoutR);

    infoAvatarLabel = new QLabel;
    infoAvatarLabel->setAlignment(Qt::AlignCenter);
    infoNumberLabel = new QLabel;
    infoNumberLabel->setAlignment(Qt::AlignCenter);
    infoNicknameLabel = new QLabel;
    infoNicknameLabel->setAlignment(Qt::AlignCenter);
    infoRemarkLabel = new QLabel;
    infoRemarkLabel->setAlignment(Qt::AlignCenter);
    QVBoxLayout *layoutInfo = new QVBoxLayout;
    QSpacerItem *spacerItemU = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QSpacerItem *spacerItemD = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    layoutInfo->addSpacerItem(spacerItemU);
    layoutInfo->addWidget(infoAvatarLabel);
    layoutInfo->addWidget(infoNumberLabel);
    layoutInfo->addWidget(infoNicknameLabel);
    layoutInfo->addWidget(infoRemarkLabel);
    layoutInfo->addSpacerItem(spacerItemD);
    infoWidget->setLayout(layoutInfo);

    QHBoxLayout *layouts = new QHBoxLayout;
    layouts->addWidget(infoWidgetL);
    layouts->addWidget(infoWidget);
    layouts->addWidget(infoWidgetR);
    infoWidgets->setLayout(layouts);
    QSizePolicy sizePolicyInfoWidgets = infoWidget->sizePolicy();
    sizePolicyInfoWidgets.setHorizontalStretch(1);
    infoWidgets->setSizePolicy(sizePolicyInfoWidgets);

    QSplitter *contactsSplitter = new QSplitter;
    contactsSplitter->addWidget(contactsTabWidget);
    contactsSplitter->addWidget(infoWidgets);
    contactsSplitter->setOrientation(Qt::Horizontal);
    contactsSplitter->setChildrenCollapsible(false);
    QHBoxLayout *layoutContactsPage = new QHBoxLayout;
    layoutContactsPage->addWidget(contactsSplitter);
    contactsPage->setLayout(layoutContactsPage);

    friendsTreeWidget = new QTreeWidget;
    friendsTreeWidget->header()->hide();
    contactsTabWidget->addTab(friendsTreeWidget, "好友");

    groupsTreeWidget = new QTreeWidget;
    groupsTreeWidget->header()->hide();
    contactsTabWidget->addTab(groupsTreeWidget, "群组");
}

MainWindow::~MainWindow()
{
    delete WSConn;
    delete mainTabWidget;
}

void MainWindow::init()
{
    this->statusBar()->showMessage("连接中。。。");
    WSConn = new WSConnection(ConfigAddress, ConfigToken, cacheManager, this);
    if(WSConn->isConnected())
    {
        selfId = new QString;
        selfNickname = new QString;
        WSConn->getMeInfo(selfId, selfNickname);
        connect(WSConn, &WSConnection::getMeInfoFinished, this, &MainWindow::updateMeInfo);

        WSConn->updateContacts(friendsTreeWidget, groupsTreeWidget, infoAvatarLabel, infoNumberLabel, infoNicknameLabel, infoRemarkLabel);
        this->statusBar()->showMessage("连接成功。", 3000);
    }
    else
    {
        this->statusBar()->showMessage("连接失败。");
    }
    chatsManager = new Chats;
    cacheManager->loadChats(chatsManager);
    WSConn->setChats(chatsManager, mainTabWidget, recentChatsListWidget, messageTextBrowser, editWidget, messageTextEdit);
    CQCode::cacheManager = cacheManager;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    /*
    QMessageBox::StandardButton resBtn = QMessageBox::question(this, "退出 CQQ", "确定现在就要退出了吗？\n", QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);
    if(resBtn != QMessageBox::Yes)
    {
        event->ignore();
    }
    else
    {
        event->accept();
    }
    */
    WSConn->endChat();
    cacheManager->saveChats(chatsManager);
    event->accept();
}

void MainWindow::updateMeInfo()
{
    infoAvatarLabel->setText("<img src=\""+cacheManager->getAvatar(*selfId, CacheManager::FriendAvatar, 100)+"\" />");
    infoNumberLabel->setText(*selfId);
    infoNicknameLabel->setText("昵称：" + *selfNickname);
}

void MainWindow::sendTextMessage()
{
    WSConn->sendTextMessage(messageTextEdit->toPlainText(), chatsManager->chats[recentChatsListWidget->currentRow()].type, chatsManager->chats[recentChatsListWidget->currentRow()].senderId);
    messageTextEdit->clear();
}

void MainWindow::sendFace()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("发送表情");

    QStringList list;
    for(auto it = stringFace.begin();it != stringFace.end();it++)
        list.push_back(it.key());

    QCompleter *completer = new QCompleter(list);
    completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    completer->setCaseSensitivity(Qt::CaseInsensitive);

    QLineEdit *lineEdit = new QLineEdit;
    lineEdit->setCompleter(completer);
    connect(lineEdit, &QLineEdit::returnPressed, dialog, &QDialog::close);

    QPushButton *ok = new QPushButton("确定");
    connect(ok, &QPushButton::clicked, dialog, &QDialog::close);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(lineEdit);
    layout->addWidget(ok);

    dialog->setLayout(layout);
    dialog->exec();

    QString name = lineEdit->text();
    if(stringFace[name].isEmpty())
    {
        qDebug() << "no face";
        return;
    }
    qDebug() << name << stringFace[name];
    WSConn->sendFace(stringFace[name], chatsManager->chats[recentChatsListWidget->currentRow()].type, chatsManager->chats[recentChatsListWidget->currentRow()].senderId);
}

void MainWindow::sendPicture()
{
    QString fileName = QFileDialog::getOpenFileName(this, "选择要发送的图片", QDir::home().path(), "Images (*.png *.jpeg *.jpg)");
    if(!fileName.isEmpty())
    {
        if(QFileInfo::exists(fileName)&&QFileInfo(fileName).isFile())
        {
            WSConn->sendImage(fileName, chatsManager->chats[recentChatsListWidget->currentRow()].type, chatsManager->chats[recentChatsListWidget->currentRow()].senderId);
        }
    }
}

void MainWindow::sendScreenshot()
{
    QClipboard *systemClipboard = QApplication::clipboard();
    QString fileName = systemClipboard->text();
    if(fileName.left(7) == "file://")
        fileName = fileName.mid(7);
    if(!fileName.isEmpty())
    {
        if(QFileInfo::exists(fileName)&&QFileInfo(fileName).isFile())
        {
            WSConn->sendImage(fileName, chatsManager->chats[recentChatsListWidget->currentRow()].type, chatsManager->chats[recentChatsListWidget->currentRow()].senderId);
        }
    }
}
