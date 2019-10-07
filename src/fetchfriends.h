#ifndef FETCHFRIENDS_H
#define FETCHFRIENDS_H

#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QWebEngineCookieStore>
#include <QWebEngineUrlRequestInterceptor>
#include <QWebEngineView>
#include <QTimer>
#include <QTextDocument>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMap>
#include "cachemanager.h"
#include "contactlist.h"

class QQMailLoginPage : public QWebEngineView
{
    Q_OBJECT

public:
    QQMailLoginPage(QWidget *parent = nullptr);
    void run(CacheManager *cacheManager, ContactList *friendList);

private slots:
    void urlChangedSlot(const QUrl &url);
    void loadFinishedSlot(bool ok);
    void getContentSlot();

private:
    CacheManager *cacheManager;
    ContactList *friendList;
};

class MyInterceptor : public QWebEngineUrlRequestInterceptor
{
    Q_OBJECT

public:
    MyInterceptor(QObject *parent = nullptr);
    void interceptRequest(QWebEngineUrlRequestInfo &info);
};

#endif // FETCHFRIENDS_H
