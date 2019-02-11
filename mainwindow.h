#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStatusBar>
#include <QTabWidget>
#include <QSplitter>
#include <QLabel>
#include <QDir>
#include <QSpacerItem>
#include <QClipboard>
#include <QApplication>
#include <QCompleter>
#include <QLineEdit>
#include <QDialog>
#include <QTableWidget>
#include <QTreeWidget>
#include <QListWidget>
#include <QPushButton>
#include <QTextBrowser>
#include <QFileDialog>
#include <QPlainTextEdit>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QSizePolicy>
#include <QMessageBox>
#include <QCloseEvent>
#include "wsconnection.h"
#include "cachemanager.h"
#include "chats.h"
#include "cqcode.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void init();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void updateMeInfo();
    void sendTextMessage();
    void sendFace();
    void sendPicture();
    void sendScreenshot();

private:
    QString ConfigAddress;
    QString ConfigToken;
    QString ConfigCachePath;

    WSConnection *WSConn;
    CacheManager *cacheManager;
    Chats *chatsManager;
    QString *selfId;
    QString *selfNickname;

    QTabWidget *mainTabWidget;
    QWidget *recentMessagesPage;
    QWidget *contactsPage;

    QListWidget *recentChatsListWidget;
    QTextBrowser *messageTextBrowser;
    QWidget *editWidget;
    QPushButton *facePushButton;
    QPushButton *picturePushButton;
    QPushButton *screenshotPushButton;
    QPushButton *sendPushButton;
    QTextEdit *messageTextEdit;

    QTabWidget *contactsTabWidget;
    QTreeWidget *friendsTreeWidget;
    QTreeWidget *groupsTreeWidget;
    QLabel *infoAvatarLabel;
    QLabel *infoNumberLabel;
    QLabel *infoNicknameLabel;
    QLabel *infoRemarkLabel;
};

#endif // MAINWINDOW_H
