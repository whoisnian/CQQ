#include "mainwindow.h"

CacheManager *CQCode::cacheManager = nullptr;
QString CQCode::messageHighlightColor;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 读取配置文件
    CONFIG = new ConfigManager(this);
    if(!CONFIG->loadConfig())
    {
        CONFIG->resetWindowSize();
        CONFIG->changeConfig(true);
        CONFIG->configPos.rx() =
                (QGuiApplication::primaryScreen()->availableSize().width()
                 - CONFIG->configMainWindowWidth)/2;
        CONFIG->configPos.ry() =
                (QGuiApplication::primaryScreen()->availableSize().height()
                 - CONFIG->configMainWindowHeight)/2;
        CONFIG->configEnableNotify = true;
    }

    chatManager = new ChatManager;
    cacheManager = new CacheManager(this);

    // 主窗口设置
    this->setWindowIcon(QIcon::fromTheme("im-qq"));
    this->setGeometry(CONFIG->configPos.x(),
                      CONFIG->configPos.y(),
                      CONFIG->configMainWindowWidth,
                      CONFIG->configMainWindowHeight);
    this->statusBar()->clearMessage();

    // 主 TabWidget
    mainTabWidget = new QTabWidget(this);
    mainTabWidget->setFocusPolicy(Qt::NoFocus);
    mainTabWidget->setIconSize(QSize(16, 16));
    this->setCentralWidget(mainTabWidget);

    // 主 TabWidget 的消息 Tab
    QWidget *messageTab = new QWidget(mainTabWidget);
    mainTabWidget->addTab(messageTab, QIcon::fromTheme("message-new"), "聊天");

    // 消息 Tab 中的聊天列表
    chatList = new ChatList(messageTab);

    // 消息 Tab 中的聊天区域
    QWidget *chatWidget = new QWidget(messageTab);

    // 聊天区域中的消息显示区域
    messageBrowser = new MessageBrowser(this, chatWidget);

    // 根据主题是深色还是浅色调整显示
    QColor windowColor = QApplication::palette().color(QPalette::Window);
    qreal h, s, l;
    windowColor.getHslF(&h, &s, &l);
    if(l > 0.5)
    {
        messageBrowser->setColor("#dddddd", "#000000");
        CQCode::messageHighlightColor = "#f67400";
    }
    else
    {
        messageBrowser->setColor("#31363b", "#ffffff");
        CQCode::messageHighlightColor = "#f67400";
    }

    // 聊天区域中的消息编辑区域
    editWidget = new QWidget(chatWidget);

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
    mainTabWidget->addTab(contactTab, QIcon::fromTheme("group"), "联系人");

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

    // 菜单
    QMenu *fileMenu = new QMenu("文件", this);
    fileMenu->setIcon(QIcon::fromTheme("document-page-setup-symbolic"));
    QMenu *viewMenu = new QMenu("视图", this);
    viewMenu->setIcon(QIcon::fromTheme("view-visible"));
    QMenu *settingMenu = new QMenu("设置", this);
    settingMenu->setIcon(QIcon::fromTheme("practice-setup"));
    QMenu *helpMenu = new QMenu("帮助", this);
    helpMenu->setIcon(QIcon::fromTheme("help-contents"));

    QAction *openCacheDirAction = new QAction(
                QIcon::fromTheme("document-open"), "打开缓存目录", this);
    openCacheDirAction->setStatusTip("打开缓存目录");
    connect(openCacheDirAction, SIGNAL(triggered()),
            this, SLOT(openCacheDir()));
    fileMenu->addAction(openCacheDirAction);

    QAction *clearCacheAction = new QAction(
                QIcon::fromTheme("edit-clear-all"), "清空缓存", this);
    clearCacheAction->setStatusTip("清空缓存");
    connect(clearCacheAction, SIGNAL(triggered()),
            this, SLOT(clearCache()));
    fileMenu->addAction(clearCacheAction);

    QAction *resetWindowSizeAction = new QAction(
                QIcon::fromTheme("kt-restore-defaults"), "重置布局", this);
    resetWindowSizeAction->setStatusTip("重置布局");
    connect(resetWindowSizeAction, SIGNAL(triggered()),
            this, SLOT(resetWindowSize()));
    viewMenu->addAction(resetWindowSizeAction);

    QAction *reloadAvatarAction = new QAction(
                QIcon::fromTheme("view-refresh"), "刷新头像", this);
    reloadAvatarAction->setStatusTip("刷新头像");
    connect(reloadAvatarAction, SIGNAL(triggered()),
            this, SLOT(reloadAvatar()));
    viewMenu->addAction(reloadAvatarAction);

    QAction *changeNotifyAction = new QAction(
                QIcon::fromTheme("message-new"), "允许通知", this);
    changeNotifyAction->setStatusTip("允许通知");
    changeNotifyAction->setCheckable(true);
    changeNotifyAction->setChecked(CONFIG->configEnableNotify);
    connect(changeNotifyAction, SIGNAL(toggled(bool)),
            this, SLOT(changeNotify(bool)));
    settingMenu->addAction(changeNotifyAction);

    QAction *fetchFriendsAction = new QAction(
                QIcon::fromTheme("download"), "抓取好友", this);
    fetchFriendsAction->setStatusTip("抓取好友");
    connect(fetchFriendsAction, SIGNAL(triggered()),
            this, SLOT(fetchFriends()));
    settingMenu->addAction(fetchFriendsAction);

    QAction *changeSettingAction = new QAction(
                QIcon::fromTheme("settings-configure"), "修改设置", this);
    changeSettingAction->setStatusTip("修改设置");
    connect(changeSettingAction, SIGNAL(triggered()),
            this, SLOT(changeSetting()));
    settingMenu->addAction(changeSettingAction);

    helpMenu->addAction(QIcon::fromTheme("help-about"),
                        "关于", this, SLOT(about()));

    // 托盘图标菜单
    QMenu *trayIconMenu = new QMenu(this);
    trayIconMenu->addMenu(fileMenu);
    trayIconMenu->addMenu(viewMenu);
    trayIconMenu->addMenu(settingMenu);
    trayIconMenu->addMenu(helpMenu);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(QIcon::fromTheme("application-exit"),
                            "退出", this, SLOT(quitApp()));

    // 托盘图标
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setToolTip("CQQ");
    trayIcon->setIcon(QIcon::fromTheme("im-qq"));
    trayIcon->setContextMenu(trayIconMenu);
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayIconClicked()));
    trayIcon->setVisible(true);
    trayIcon->show();

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
    connect(chatList, SIGNAL(clearItem(QListWidgetItem *)),
            this, SLOT(clearChat(QListWidgetItem *)));
    connect(friendList, SIGNAL(itemClicked(QTreeWidgetItem *, int)),
            this, SLOT(showFriendInfo(QTreeWidgetItem *, int)));
    connect(friendList, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)),
            this, SLOT(startPrivateChat(QTreeWidgetItem *, int)));
    connect(groupList, SIGNAL(itemClicked(QTreeWidgetItem *, int)),
            this, SLOT(showGroupInfo(QTreeWidgetItem *, int)));
    connect(groupList, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)),
            this, SLOT(startGroupChat(QTreeWidgetItem *, int)));
    connect(chatWidgetSplitter, SIGNAL(splitterMoved(int, int)),
            this, SLOT(resizeChatWidgetSplitter(int, int)));
    connect(messageTabSplitter, SIGNAL(splitterMoved(int, int)),
            this, SLOT(resizeMessageTabSplitter(int, int)));
    connect(contactTabSplitter, SIGNAL(splitterMoved(int, int)),
            this, SLOT(resizeContactTabSplitter(int, int)));

    messageBrowser->hide();
    editWidget->hide();
}

MainWindow::~MainWindow()
{
    qDebug() << "~MainWindow";
}

void MainWindow::init()
{
    CQCode::cacheManager = cacheManager;

    // 从缓存目录中加载之前的聊天记录
    cacheManager->loadChatManager(chatManager);
    updateChatListFromChatManager();
    messageBrowser->setCacheManager(cacheManager);
    messageBrowser->setChatManager(chatManager);
    messageBrowser->setChatList(chatList);

    // WebSocket连接
    this->statusBar()->showMessage("连接中。。。");
    WSConn = new WSConnection(CONFIG->configAddress,
                              CONFIG->configToken,
                              cacheManager,
                              chatManager,
                              chatList,
                              messageBrowser,
                              this);
    if(WSConn->isConnected())
    {
        connect(WSConn, SIGNAL(getLoginInfoFinished(QString, QString)),
                this, SLOT(updateLoginInfo(QString, QString)));
        connect(WSConn, SIGNAL(newMessageReceived()),
                messageBrowser, SLOT(updateContent()));
        connect(WSConn, SIGNAL(notifyMessage(QString, QString, QIcon)),
                this, SLOT(notifyMessage(QString, QString, QIcon)));
        connect(cacheManager, SIGNAL(getAllAvatarFinished()),
                this, SLOT(reloadAvatar()));
        WSConn->getLoginInfo();
        cacheManager->avatarNum = 0;
        WSConn->getFriendList(friendList);
        WSConn->getGroupList(groupList);
        this->statusBar()->showMessage("连接成功。", 3000);
    }
    else
    {
        this->statusBar()->showMessage("连接失败。");
    }
}

void MainWindow::insertFace(QString face)
{
    messageEdit->insertPlainText(face);
    messageEdit->setFocus();
}

void MainWindow::sendImage(QString fileName)
{
    WSConn->sendImage(fileName);
}

void MainWindow::sendScreenshot(QString fileName)
{
    WSConn->sendScreenshot(fileName);
}

void MainWindow::sendMessage()
{
    WSConn->sendMessage(messageEdit->toPlainText());
    messageEdit->clear();
}

void MainWindow::changeChat(QListWidgetItem *)
{
    if(messageBrowser->isHidden())
    {
        messageBrowser->show();
    }
    if(editWidget->isHidden())
    {
        editWidget->show();
    }
    // 存储正在编辑的消息
    for(auto &it : chatManager->chats)
    {
        if(it.chatID == messageBrowser->curChatID
                &&it.type == messageBrowser->curChatType)
        {
            it.editingMessage = messageEdit->toPlainText();
            break;
        }
    }
    messageBrowser->updateContent();
    // 恢复之前正在编辑的消息
    for(auto &it : chatManager->chats)
    {
        if(it.chatID == messageBrowser->curChatID
                &&it.type == messageBrowser->curChatType)
        {
            messageEdit->setPlainText(it.editingMessage);
            break;
        }
    }

    if(messageBrowser->curChatType == Chat::Group)
    {
        cacheManager->getCard(messageBrowser->curChatID, *selfID);
    }
}

void MainWindow::deleteChat(QListWidgetItem *item)
{
    if(messageBrowser->curChatID ==
            chatManager->chatAt(chatList->currentRow())->chatID
       &&messageBrowser->curChatType ==
            chatManager->chatAt(chatList->currentRow())->type)
    {
        messageBrowser->hide();
        messageBrowser->clear();
        editWidget->hide();
    }
    chatManager->chats.removeAt(chatList->currentRow());
    delete item;
}

void MainWindow::clearChat(QListWidgetItem *)
{
    if(messageBrowser->curChatID ==
            chatManager->chatAt(chatList->currentRow())->chatID
       &&messageBrowser->curChatType ==
            chatManager->chatAt(chatList->currentRow())->type)
    {
        messageBrowser->clear();
    }
    chatManager->chats[chatList->currentRow()].messages.clear();
    chatManager->chats[chatList->currentRow()].sumNum = 0;
    chatManager->chats[chatList->currentRow()].unreadNum = 0;
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
        cacheManager->getCard(item->toolTip(1), *selfID);
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
    chatManager->selfID = id;
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
        chatName = it->chatName;
        if(it->unreadNum > 0)
        {
            chatName += "(" + QString::number(it->unreadNum) + ")";
        }
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

void MainWindow::reloadAvatar()
{
    qDebug() << "reload avatar";
    QTreeWidgetItemIterator it1(friendList);
    while(*it1)
    {
        if(!(*it1)->toolTip(1).isEmpty())
        {
            (*it1)->setIcon(0, QIcon(
                                cacheManager->getAvatar((*it1)->toolTip(1),
                                CacheManager::Friend, 100)));
        }
        it1++;
    }
    QTreeWidgetItemIterator it2(groupList);
    while(*it2)
    {
        if(!(*it2)->toolTip(1).isEmpty())
        {
            (*it2)->setIcon(0, QIcon(
                                cacheManager->getAvatar((*it2)->toolTip(1),
                                CacheManager::Group, 100)));
        }
        it2++;
    }
    updateChatListFromChatManager();
}

void MainWindow::resizeChatWidgetSplitter(int, int)
{
    CONFIG->configChatWidgetSplitterSizes = chatWidgetSplitter->sizes();
}

void MainWindow::resizeMessageTabSplitter(int, int)
{
    CONFIG->configMessageTabSplitterSizes = messageTabSplitter->sizes();
}

void MainWindow::resizeContactTabSplitter(int, int)
{
    CONFIG->configContactTabSplitterSizes = contactTabSplitter->sizes();
}

void MainWindow::clearCache()
{
    cacheManager->clearCacheDir();
    cacheManager->nicknameMap.clear();
    cacheManager->remarkMap.clear();
    cacheManager->groupnameMap.clear();
    cacheManager->cardMap.clear();

    chatList->clear();
    chatManager->clear();

    messageBrowser->clear();
    messageEdit->clear();

    WSConn->getLoginInfo();
    cacheManager->avatarNum = 0;
    WSConn->getFriendList(friendList);
    WSConn->getGroupList(groupList);
}

void MainWindow::openCacheDir()
{
    cacheManager->openCacheDir();
}

void MainWindow::resetWindowSize()
{
    CONFIG->resetWindowSize();
    this->resize(CONFIG->configMainWindowWidth,
                 CONFIG->configMainWindowHeight);
    chatWidgetSplitter->setSizes(CONFIG->configChatWidgetSplitterSizes);
    messageTabSplitter->setSizes(CONFIG->configMessageTabSplitterSizes);
    contactTabSplitter->setSizes(CONFIG->configContactTabSplitterSizes);
}

void MainWindow::fetchFriends()
{
    QQMailLoginPage *qqMailLoginPage = new QQMailLoginPage();
    qqMailLoginPage->run(cacheManager, friendList);
}

void MainWindow::changeSetting()
{
    CONFIG->changeConfig(false);
    connect(CONFIG, SIGNAL(configChanged()),
            this, SLOT(clearAndRestart()),
            Qt::UniqueConnection);
}

void MainWindow::changeNotify(bool checked)
{
    CONFIG->configEnableNotify = checked;
}

void MainWindow::about()
{
    QMessageBox::about(this, "About",
                 "<b>CQQ</b>"\
                 "<p>Based on Qt Creator 4.8.2</p>"\
                 "<p>Based on Qt 5.12.2 (GCC 8.2.1 20181127, 64 bit)</p>");
}

void MainWindow::trayIconClicked()
{
    if(this->isVisible())
    {
        oldPos = this->pos();
        this->hide();
        qDebug() << this->oldPos;
    }
    else
    {
        // hide() 和 show() 之间move有bug，相同位置move会被忽略
        // https://bugreports.qt.io/browse/QTBUG-56579
        this->move(oldPos.x()+1, oldPos.y());
        this->show();
        this->move(oldPos);
        this->activateWindow();
        qDebug() << this->oldPos << this->pos();
    }
}

void MainWindow::notifyMessage(QString title, QString message, QIcon icon)
{
    if(!CONFIG->configEnableNotify)
        return;
    // 窗口最小化或者隐藏时才显示通知
    if(this->isHidden()||this->isMinimized())
        trayIcon->showMessage(title, message, icon);
}

void MainWindow::clearAndRestart()
{
    qDebug() << "clear and restart";
    cacheManager->clearCacheDir();
    cacheManager->nicknameMap.clear();
    cacheManager->remarkMap.clear();
    cacheManager->groupnameMap.clear();
    cacheManager->cardMap.clear();
    cacheManager->avatarNum = 0;

    chatList->clear();
    chatManager->clear();

    messageBrowser->clear();
    messageEdit->clear();

    CONFIG->configMainWindowWidth = this->width();
    CONFIG->configMainWindowHeight = this->height();
    CONFIG->saveConfig();
    cacheManager->saveChatManager(chatManager);
    delete chatManager;
    chatManager = nullptr;
    qApp->exit(233);
}

void MainWindow::quitApp()
{
    CONFIG->configPos = this->pos();
    CONFIG->configMainWindowWidth = this->width();
    CONFIG->configMainWindowHeight = this->height();
    CONFIG->saveConfig();
    cacheManager->saveChatManager(chatManager);
    delete chatManager;
    chatManager = nullptr;
    qApp->exit(0);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    oldPos = this->pos();
    this->hide();
    event->ignore();
}
