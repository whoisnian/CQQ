#ifndef WSCONNECTION_H
#define WSCONNECTION_H

#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QQueue>
#include <QString>
#include <QTimer>
#include <QWebSocket>
#include "cachemanager.h"

enum CommandType
{
    get_login_info,
    _get_friend_list,
    get_group_list,
    get_group_member_info,
    get_status
};

struct Command
{
    QString content;
    CommandType type;
};

// 处理 WebSocket 连接
class WSConnection : public QObject
{
    Q_OBJECT

public:
    WSConnection(QString address,
                 QString token,
                 CacheManager *cacheManager,
                 QObject *parent = nullptr);
    ~WSConnection();

    bool isConnected();
    void addCommand(CommandType type, QString content);
    void startNextCommand();

    void getLoginInfo();

signals:
    void getLoginInfoFinished(QString id, QString nickname);

private slots:
    void wsAPIConnected();
    void wsAPIDisconnected();
    void wsAPIReceived(const QString message);
    void wsEVENTConnected();
    void wsEVENTDisconnected();
    void wsEVENTReceived(const QString message);

    void sendHeartbeat();

private:
    QString address;
    QString token;
    CacheManager *cacheManager;

    QWebSocket wsAPI;
    QWebSocket wsEVENT;
    QEventLoop *loop;
    QTimer *heartbeatTimer;

    QQueue<Command> commandQueue;
    bool newStart;
};

#endif // WSCONNECTION_H
