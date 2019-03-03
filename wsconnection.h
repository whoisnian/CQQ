#ifndef WSCONNECTION_H
#define WSCONNECTION_H

#include <QEventLoop>
#include <QObject>
#include <QString>
#include <QWebSocket>
#include "cachemanager.h"

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

private slots:
    void wsAPIConnected();
    void wsAPIDisconnected();
    void wsAPIReceived(const QString message);
    void wsEVENTConnected();
    void wsEVENTDisconnected();
    void wsEVENTReceived(const QString message);

private:
    QString address;
    QString token;
    CacheManager *cacheManager;

    QWebSocket wsAPI;
    QWebSocket wsEVENT;
    QEventLoop *loop;
};

#endif // WSCONNECTION_H
