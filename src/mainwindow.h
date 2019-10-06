#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCloseEvent>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QMenuBar>
#include <QScreen>
#include <QSplitter>
#include <QStatusBar>
#include <QSystemTrayIcon>
#include <QTabWidget>
#include <QVBoxLayout>
#include "chatlist.h"
#include "chatmanager.h"
#include "configmanager.h"
#include "contactlist.h"
#include "cqcode.h"
#include "fetchfriends.h"
#include "infowidget.h"
#include "messagebrowser.h"
#include "messageedit.h"
#include "messageedittool.h"
#include "wsconnection.h"

// 主界面
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void init();

private slots:
    void insertFace(QString face);
    void sendImage(QString fileName);
    void sendScreenshot(QString fileName);
    void sendMessage();
    void changeChat(QListWidgetItem *item);
    void deleteChat(QListWidgetItem *item);
    void clearChat(QListWidgetItem *item);
    void showFriendInfo(QTreeWidgetItem *item, int column);
    void startPrivateChat(QTreeWidgetItem *item, int column);
    void showGroupInfo(QTreeWidgetItem *item, int column);
    void startGroupChat(QTreeWidgetItem *item, int column);
    void updateLoginInfo(QString id, QString nickname);
    void updateChatListFromChatManager();
    void reloadAvatar();
    // 记录窗口分割器 QSplitter 调整
    void resizeChatWidgetSplitter(int pos, int index);
    void resizeMessageTabSplitter(int pos, int index);
    void resizeContactTabSplitter(int pos, int index);
    // 菜单栏功能
    void openCacheDir();
    void clearCache();
    void resetWindowSize();
    void fetchFriends();
    void changeSetting();
    void changeNotify(bool checked);
    void clearAndRestart();
    void quitApp();
    void about();

    void trayIconClicked();
    void notifyMessage(QString title, QString message, QIcon icon);

protected:
    void closeEvent(QCloseEvent *event);

private:
    ConfigManager *CONFIG;
    WSConnection *WSConn;
    ChatManager *chatManager = nullptr;
    CacheManager *cacheManager;
    QString *selfID;
    QString *selfNickname;
    QPoint oldPos;

    QTabWidget *mainTabWidget;
    ChatList *chatList;
    ContactList *friendList;
    ContactList *groupList;
    InfoWidget *infoWidget;
    QWidget *editWidget;
    MessageBrowser *messageBrowser;
    MessageEdit *messageEdit;
    MessageEditTool *messageEditTool;

    QSplitter *chatWidgetSplitter;
    QSplitter *messageTabSplitter;
    QSplitter *contactTabSplitter;

    QSystemTrayIcon *trayIcon;
};

#endif // MAINWINDOW_H
