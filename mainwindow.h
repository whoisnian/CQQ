#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCloseEvent>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QSplitter>
#include <QStatusBar>
#include <QTabWidget>
#include <QVBoxLayout>
#include "chatlist.h"
#include "configmanager.h"
#include "contactlist.h"
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

protected:
    void closeEvent(QCloseEvent *event);

private:
    ConfigManager *CONFIG;
    WSConnection *WSConn;

    ChatList *chatList;
    ContactList *friendList;
    ContactList *groupList;
    InfoWidget *infoWidget;
    MessageBrowser *messageBrowser;
    MessageEdit *messageEdit;
    MessageEditTool *messageEditTool;

    QSplitter *chatWidgetSplitter;
    QSplitter *messageTabSplitter;
    QSplitter *contactTabSplitter;
};

#endif // MAINWINDOW_H
