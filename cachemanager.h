#ifndef CACHEMANAGER_H
#define CACHEMANAGER_H

#include <QDebug>
#include <QDir>
#include <QMap>
#include <QObject>
#include <QStandardPaths>
#include "chatmanager.h"
#include "downloadmanager.h"

// 头像，图片等文件缓存，昵称，备注，群名片缓存
class CacheManager : public QObject
{
    Q_OBJECT

public:
    enum AvatarType
    {
        Friend,
        Group
    };

    CacheManager(QObject *parent = nullptr);
    bool loadChatManager(ChatManager *chatManager);
    bool saveChatManager(ChatManager *chatManager);
    QString getAvatar(QString ID, AvatarType type, int size);
    QString getImage(QString file, QString url);
    QString getCard(QString userID, QString groupID);

    QMap<QString, QString> nicknameMap;
    QMap<QString, QString> remarkMap;
    QMap<QString, QString> groupnameMap;
    QMap<QString, QString> cardMap;

signals:
    void getCardSignal(QString groupID, QString userID);

private:
    QString cachePath;
    DownloadManager *downloadManager;
};

#endif // CACHEMANAGER_H
