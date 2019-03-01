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
    QTabWidget *mainTabWidget = new QTabWidget(this);
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
    //connect(chatWidgetSplitter, &QSplitter::splitterMoved, this, &MainWindow::init);
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
    connect(friendList, SIGNAL(itemClicked()),
            this, SLOT(showFriendInfo()));
    connect(friendsTreeWidget, &QTreeWidget::itemDoubleClicked, this, &WSConnection::startPrivateChat);
    connect(groupsTreeWidget, &QTreeWidget::itemClicked, this, &WSConnection::showGroupInfo);
    connect(groupsTreeWidget, &QTreeWidget::itemDoubleClicked, this, &WSConnection::startGroupChat);

    //messageBrowser->hide();
    //editWidget->hide();
}

MainWindow::~MainWindow()
{
    qDebug() << "~MainWindow";
}

void MainWindow::init()
{
    infoWidget->setInfo("/home/nian/Pictures/ruby_headphones.jpg",
                        "1334583207",
                        "念",
                        "echo");
    chatList->addItem("nihao");
    chatList->addItem("ceshi");
    chatList->addItem("测试");
    chatList->addItem("12312312312");

    for(int i = 0;i < 5;i++)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem;
        item->setText(0, "num"+QString::number(i));
        friendList->addTopLevelItem(item);
        for(int j = 0;j < i;j++)
        {
            QTreeWidgetItem *subItem = new QTreeWidgetItem;
            subItem->setText(0, "sub"+QString::number(j));
            item->addChild(subItem);
        }
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

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "closeEvent";
    CONFIG->configMainWindowWidth = this->width();
    CONFIG->configMainWindowHeight = this->height();
    CONFIG->configChatWidgetSplitterSizes = chatWidgetSplitter->sizes();
    CONFIG->configMessageTabSplitterSizes = messageTabSplitter->sizes();
    CONFIG->configContactTabSplitterSizes = contactTabSplitter->sizes();
    CONFIG->saveConfig();
    event->accept();
}
