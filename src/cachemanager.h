#ifndef CACHEMANAGER_H
#define CACHEMANAGER_H

#include <QCryptographicHash>
#include <QDebug>
#include <QDir>
#include <QMap>
#include <QObject>
#include <QStandardPaths>
#include "chatmanager.h"
#include "downloadmanager.h"

// 头像，图片等文件缓存，聊天记录缓存，昵称，备注，群名片缓存
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
    // 从缓存目录加载聊天记录缓存
    bool loadChatManager(ChatManager *chatManager);
    // 保存聊天记录到缓存目录
    bool saveChatManager(ChatManager *chatManager);
    // 获取头像
    QString getAvatar(QString ID, AvatarType type, int size);
    // 获取图片
    QString getImage(QString file, QString url);
    // 获取群名片
    QString getCard(QString groupID, QString userID);
    // 将本地图片保存到缓存目录
    QString cacheImage(QString fileName);

    // 昵称，备注，群名片缓存
    QMap<QString, QString> nicknameMap;
    QMap<QString, QString> remarkMap;
    QMap<QString, QString> groupnameMap;
    QMap<QString, QString> cardMap;

signals:
    void getCardSignal(QString groupID, QString userID);
    void getImageFinished(QString filePath, QString url);

private slots:
    void downloadImageFinished(QString filePath, QString url);

private:
    QString cachePath;
    DownloadManager *downloadManager;
};

#endif // CACHEMANAGER_H
