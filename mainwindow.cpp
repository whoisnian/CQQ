#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 读取配置文件
    CONFIG = new ConfigManager(this);
    if(!CONFIG->loadConfig())
    {
        CONFIG->resetWindowSize();
        CONFIG->changeConfig(true);
    }
    CONFIG->resetWindowSize();

    // 主窗口设置
    this->setWindowIcon(QIcon::fromTheme("im-qq"));
    this->resize(CONFIG->configMainWindowWidth,
                 CONFIG->configMainWindowHeight);
    this->statusBar()->clearMessage();

    // 主 TabWidget
    mainTabWidget = new QTabWidget(this);
    mainTabWidget->setFocusPolicy(Qt::NoFocus);
    mainTabWidget->setIconSize(QSize(64, 16));
    this->setCentralWidget(mainTabWidget);

    // 主 TabWidget 的消息 Tab
    QWidget *messageTab = new QWidget(mainTabWidget);
    mainTabWidget->addTab(messageTab, QIcon::fromTheme("message-new"), "");

    // 消息 Tab 中的聊天列表
    chatList = new ChatList(messageTab);

    // 消息 Tab 中的聊天区域
    QWidget *chatWidget = new QWidget(messageTab);

    // 聊天区域中的消息显示区域
    messageBrowser = new MessageBrowser(chatWidget);

    // 聊天区域中的消息编辑区域
    QWidget *editWidget = new QWidget(chatWidget);

    // 消息编辑区域中的编辑工具栏
    messageEditTool = new MessageEditTool(editWidget);

    // 消息编辑区域中的文本编辑框
    messageEdit = new MessageEdit(editWidget);

    // 聊天区域中的消息编辑区域布局
    QVBoxLayout *editLayout = new QVBoxLayout(editWidget);
    editLayout->setMargin(0);
    editLayout->setSpacing(0);
    editLayout->addWidget(messageEditTool);
    editLayout->addWidget(messageEdit);
    editWidget->setLayout(editLayout);

    // 消息 Tab 中的聊天区域布局
    chatWidgetSplitter = new QSplitter(chatWidget);
    chatWidgetSplitter->addWidget(messageBrowser);
    chatWidgetSplitter->addWidget(editWidget);
    chatWidgetSplitter->setOrientation(Qt::Vertical);
    chatWidgetSplitter->setChildrenCollapsible(false);
    chatWidgetSplitter->setSizes(CONFIG->configChatWidgetSplitterSizes);
    QHBoxLayout *chatWidgetLayout = new QHBoxLayout(chatWidget);
    chatWidgetLayout->setMargin(0);
    chatWidgetLayout->setSpacing(0);
    chatWidgetLayout->addWidget(chatWidgetSplitter);
    chatWidget->setLayout(chatWidgetLayout);

    // 主 TabWidget 的消息 Tab 布局
    messageTabSplitter = new QSplitter(messageTab);
    messageTabSplitter->addWidget(chatList);
    messageTabSplitter->addWidget(chatWidget);
    messageTabSplitter->setOrientation(Qt::Horizontal);
    messageTabSplitter->setChildrenCollapsible(false);
    messageTabSplitter->setSizes(CONFIG->configMessageTabSplitterSizes);
    QHBoxLayout *messageTabLayout = new QHBoxLayout(messageTab);
    messageTabLayout->addWidget(messageTabSplitter);
    messageTab->setLayout(messageTabLayout);

    // 主 TabWidget 的联系人 Tab
    QWidget *contactTab = new QWidget(mainTabWidget);
    mainTabWidget->addTab(contactTab, QIcon::fromTheme("group"), "");

    // 联系人 Tab 中的好友群组 TabWidget
    QTabWidget *contactTabWidget = new QTabWidget(contactTab);
    contactTabWidget->setFocusPolicy(Qt::NoFocus);

    // 好友群组 TabWidget 中的好友列表
    friendList = new ContactList(contactTabWidget);
    contactTabWidget->addTab(friendList, "好友");

    // 好友群组 TabWidget 中的群组列表
    groupList = new ContactList(contactTabWidget);
    contactTabWidget->addTab(groupList, "群组");

    // 联系人 Tab 中的信息区域
    infoWidget = new InfoWidget(contactTab);

    // 主 TabWidget 的联系人 Tab 布局
    contactTabSplitter = new QSplitter(contactTab);
    contactTabSplitter->addWidget(contactTabWidget);
    contactTabSplitter->addWidget(infoWidget);
    contactTabSplitter->setOrientation(Qt::Horizontal);
    contactTabSplitter->setChildrenCollapsible(false);
    contactTabSplitter->setSizes(CONFIG->configContactTabSplitterSizes);
    QHBoxLayout *contactTabLayout = new QHBoxLayout(contactTab);
    contactTabLayout->addWidget(contactTabSplitter);
    contactTab->setLayout(contactTabLayout);

    connect(messageEditTool, SIGNAL(insertFace(QString)),
            this, SLOT(insertFace(QString)));
    connect(messageEditTool, SIGNAL(sendImage(QString)),
            this, SLOT(sendImage(QString)));
    connect(messageEditTool, SIGNAL(sendScreenshot(QString)),
            this, SLOT(sendScreenshot(QString)));
    connect(messageEditTool, SIGNAL(sendMessage()),
            this, SLOT(sendMessage()));
    connect(messageEdit, SIGNAL(ctrlEnterPressed()),
            this, SLOT(sendMessage()));
    connect(chatList, SIGNAL(itemClicked(QListWidgetItem *)),
            this, SLOT(changeChat(QListWidgetItem *)));
    connect(chatList, SIGNAL(deleteItem(QListWidgetItem *)),
            this, SLOT(deleteChat(QListWidgetItem *)));
    connect(friendList, SIGNAL(itemClicked(QTreeWidgetItem *, int)),
            this, SLOT(showFriendInfo(QTreeWidgetItem *, int)));
    connect(friendList, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)),
            this, SLOT(startPrivateChat(QTreeWidgetItem *, int)));
    connect(groupList, SIGNAL(itemClicked(QTreeWidgetItem *, int)),
            this, SLOT(showGroupInfo(QTreeWidgetItem *, int)));
    connect(groupList, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)),
            this, SLOT(startGroupChat(QTreeWidgetItem *, int)));

    //messageBrowser->hide();
    //editWidget->hide();
}

MainWindow::~MainWindow()
{
    qDebug() << "~MainWindow";
}

CacheManager *CQCode::cacheManager = nullptr;

void MainWindow::init()
{
    chatManager = new ChatManager;
    cacheManager = new CacheManager(this);
    CQCode::cacheManager = cacheManager;

    // 从缓存目录中加载之前的聊天记录
    cacheManager->loadChatManager(chatManager);
    updateChatListFromChatManager();

    // WebSocket连接
    this->statusBar()->showMessage("连接中。。。");
    WSConn = new WSConnection(CONFIG->configAddress,
                              CONFIG->configToken,
                              cacheManager,
                              chatManager,
                              chatList,
                              this);
    if(WSConn->isConnected())
    {
        connect(WSConn, SIGNAL(getLoginInfoFinished(QString, QString)),
                this, SLOT(updateLoginInfo(QString, QString)));
        WSConn->getLoginInfo();
        WSConn->getFriendList(friendList);
        WSConn->getGroupList(groupList);
        this->statusBar()->showMessage("连接成功。", 3000);
    }
    else
    {
        this->statusBar()->showMessage("连接失败。");
    }

    qDebug() << chatWidgetSplitter->sizes();
    qDebug() << messageTabSplitter->sizes();
    qDebug() << contactTabSplitter->sizes();
}

void MainWindow::insertFace(QString face)
{
    qDebug() << face;
    messageEdit->insertPlainText(face);
}

void MainWindow::sendImage(QString fileName)
{
    qDebug() << fileName;
}

void MainWindow::sendScreenshot(QString fileName)
{
    qDebug() << fileName;
}

void MainWindow::sendMessage()
{
    qDebug() << "send" << messageEdit->toPlainText();
}

void MainWindow::changeChat(QListWidgetItem *item)
{
    qDebug() << "change to" << item->text();
}

void MainWindow::deleteChat(QListWidgetItem *item)
{
    qDebug() << "delete" << item->text();
}

void MainWindow::showFriendInfo(QTreeWidgetItem *item, int)
{
    if(item->toolTip(1).isEmpty())
    {
        return;
    }
    QString avatar;
    avatar = cacheManager->getAvatar(item->toolTip(1),
                                     CacheManager::Friend,
                                     100);
    infoWidget->setInfo(avatar,
                        item->toolTip(1),
                        item->toolTip(0),
                        item->text(0));
    qDebug() << "showFriendInfo";
}

void MainWindow::startPrivateChat(QTreeWidgetItem *item, int)
{
    if(item->toolTip(1).isEmpty())
    {
        return;
    }
    int index = chatManager->indexOf(item->toolTip(1), Chat::Private);
    if(index == -1)
    {
        chatManager->addNewChat(item->toolTip(1),
                                item->text(0),
                                Chat::Private,
                                Chat::SubFriend);
        QString avatar;
        avatar = cacheManager->getAvatar(item->toolTip(1),
                                         CacheManager::Friend,
                                         100);
        chatList->addNewChatItem(avatar,
                                 item->toolTip(1),
                                 item->text(0));
        chatList->setCurrentRow(0);
        this->changeChat(chatList->item(0));
    }
    else
    {
        chatList->setCurrentRow(index);
        this->changeChat(chatList->item(index));
    }
    mainTabWidget->setCurrentIndex(0);
    qDebug() << "startPrivateChat";
}

void MainWindow::showGroupInfo(QTreeWidgetItem *item, int)
{
    if(item->toolTip(1).isEmpty())
    {
        return;
    }
    QString avatar;
    avatar = cacheManager->getAvatar(item->toolTip(1),
                                     CacheManager::Group,
                                     100);
    infoWidget->setInfo(avatar,
                        item->toolTip(1),
                        item->text(0),
                        QString());
    qDebug() << "showGroupInfo";
}

void MainWindow::startGroupChat(QTreeWidgetItem *item, int)
{
    if(item->toolTip(1).isEmpty())
    {
        return;
    }
    int index = chatManager->indexOf(item->toolTip(1), Chat::Group);
    if(index == -1)
    {
        chatManager->addNewChat(item->toolTip(1),
                                item->text(0),
                                Chat::Group,
                                Chat::SubNormal);
        QString avatar;
        avatar = cacheManager->getAvatar(item->toolTip(1),
                                         CacheManager::Group,
                                         100);
        chatList->addNewChatItem(avatar,
                                 item->toolTip(1),
                                 item->text(0));
        chatList->setCurrentRow(0);
        this->changeChat(chatList->item(0));
    }
    else
    {
        chatList->setCurrentRow(index);
        this->changeChat(chatList->item(index));
    }
    mainTabWidget->setCurrentIndex(0);
    qDebug() << "startGroupChat";
}

void MainWindow::updateLoginInfo(QString id, QString nickname)
{
    this->selfID = new QString(id);
    this->selfNickname = new QString(nickname);
    QString avatar;
    avatar = cacheManager->getAvatar(id,
                                     CacheManager::Friend,
                                     100);
    infoWidget->setInfo(avatar,
                        id,
                        nickname,
                        "");
}

void MainWindow::updateChatListFromChatManager()
{
    chatList->clear();
    for(auto it = chatManager->chats.end() - 1;
        it >= chatManager->chats.begin();it--)
    {
        QString chatID, chatName, avatar;
        chatID = it->chatID;
        chatName = it->chatName + "(" + QString::number(it->unreadNum) + ")";
        if(it->type == Chat::Private)
        {
            avatar = cacheManager->getAvatar(chatID,
                                             CacheManager::Friend,
                                             100);
        }
        else
        {
            avatar = cacheManager->getAvatar(chatID,
                                             CacheManager::Group,
                                             100);
        }
        chatList->addNewChatItem(avatar,
                                 chatID,
                                 chatName);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "closeEvent";
    CONFIG->configMainWindowWidth = this->width();
    CONFIG->configMainWindowHeight = this->height();
    CONFIG->configChatWidgetSplitterSizes = chatWidgetSplitter->sizes();
    CONFIG->configMessageTabSplitterSizes = messageTabSplitter->sizes();
    CONFIG->configContactTabSplitterSizes = contactTabSplitter->sizes();
    CONFIG->saveConfig();
    cacheManager->saveChatManager(chatManager);
    delete chatManager;
    chatManager = nullptr;
    event->accept();
}
