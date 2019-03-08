#ifndef CACHEMANAGER_H
#define CACHEMANAGER_H

#include <QDebug>
#include <QDir>
#include <QObject>
#include <QStandardPaths>
#include "chatmanager.h"

// 头像，图片等文件缓存
class CacheManager : public QObject
{
    Q_OBJECT

public:
    CacheManager(QObject *parent = nullptr);
    bool loadChatManager(ChatManager *chatManager);
    bool saveChatManager(ChatManager *chatManager);
};

#endif // CACHEMANAGER_H
