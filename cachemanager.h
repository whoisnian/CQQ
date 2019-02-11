#ifndef CACHEMANAGER_H
#define CACHEMANAGER_H

#include <QUrl>
#include <QString>
#include <QQueue>
#include <QMap>
#include <QDir>
#include <QFileInfo>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include "chats.h"

class Task
{
public:
    Task(QUrl u, QString p)
        : url(u), path(p){}

    QUrl url;
    QString path;
};

class CacheManager : public QObject
{
    Q_OBJECT

public:
    CacheManager(QString cachePath);

    QString getAvatar(QString id, int type, int size = 100);
    QString getImage(QString file, QString url);
    QString getCard(QString user_id, QString group_id);
    bool loadChats(Chats *chats);
    bool saveChats(Chats *chats);

    QMap<QString, QString> nicknameMap;
    QMap<QString, QString> remarkMap;
    QMap<QString, QString> groupnameMap;
    QMap<QString, QString> cardMap;

    const static int FriendAvatar = 1;
    const static int GroupAvatar = 2;

signals:
    void getAvatarFinished();
    void getImageFinished(QString realPath);
    void getCardSignal(QString user_id, QString group_id);

private slots:
    void startNextDownload();
    void downloadFinished();
    void downloadReadyRead();

private:
    QString cachePath;
    QNetworkAccessManager *manager;
    QQueue<Task> downloadQueue;
    QNetworkReply *currentDownload;
    QFile output;
    bool newStart;
    int avatarNum;
};

#endif // CACHEMANAGER_H
