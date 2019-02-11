#ifndef WSCONNECTION_H
#define WSCONNECTION_H

#include <QWebSocket>
#include <QIcon>
#include <QLabel>
#include <QPicture>
#include <QString>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTreeWidget>
#include <QEventLoop>
#include <QListWidget>
#include <QTextEdit>
#include <QTextBrowser>
#include <QTableWidget>
#include <QScrollBar>
#include <QListWidgetItem>
#include <QStyleOption>
#include "cachemanager.h"
#include "chats.h"
#include "cqcode.h"

class WSConnection : public QObject
{
    Q_OBJECT

public:
    WSConnection(QString address, QString token, CacheManager *cacheManager, QObject *parent = nullptr);
    ~WSConnection();

    bool isConnected();
    void updateContacts(QTreeWidget *friendsTreeWidget, QTreeWidget *groupsTreeWidget, QLabel *infoAvatarLabel, QLabel *infoNumberLabel, QLabel *infoNicknameLabel, QLabel *infoRemarkLabel);
    void getMeInfo(QString *id, QString *nickname);
    void setChats(Chats *ch, QTabWidget *main, QListWidget *recentChats, QTextBrowser *textBrower, QWidget *editWidget, QTextEdit *messageTextEdit);
    void endChat();
    void preloadCheck(QString message, QString group_id = "");
    void sendTextMessage(QString textMessage, int messageType, QString id);
    void sendImage(QString fileName, int messageType, QString id);
    void sendFace(QString faceId, int messageType, QString id);

signals:
    void getMeInfoFinished();

private Q_SLOTS:
    void wsAPIConnected();
    void wsAPIDisconnected();
    void wsAPIReceived(QString message);
    void wsEVENTConnected();
    void wsEVENTDisconnected();
    void wsEVENTReceived(QString message);

    void wsAPIHeartbeat();

    void showFriendInfo(QTreeWidgetItem *item, int column);
    void showGroupInfo(QTreeWidgetItem *item, int column);
    void startPrivateChat(QTreeWidgetItem *item, int column);
    void startGroupChat(QTreeWidgetItem *item, int column);
    void updateAvatar();
    void updateChat();
    void updateCard(QString user_id, QString group_id);
    void addCommand(QString command);
    void startNextCommand();

private:
    QString address;
    QString token;
    CacheManager *cacheManager;

    QWebSocket wsAPI;
    QWebSocket wsEVENT;
    QQueue<QString> commandQueue;
    QJsonDocument jsonDocFromwsAPI;
    bool newStart;

    QEventLoop *loop;
    QTimer *heartbeatTimer;

    Chats *chats;
    QString *selfId;
    QString *selfNickname;

    QTabWidget *mainTabWidget;
    QListWidget *recentChatsListWidget;
    QTextBrowser *messageTextBrowser;
    QWidget *editWidget;
    QTextEdit *messageTextEdit;
    QString curChatId;
    int curChatType;

    QTreeWidget *friendsTreeWidget;
    QTreeWidget *groupsTreeWidget;
    QLabel *infoAvatarLabel;
    QLabel *infoNumberLabel;
    QLabel *infoNicknameLabel;
    QLabel *infoRemarkLabel;
};

#endif // WSCONNECTION_H
