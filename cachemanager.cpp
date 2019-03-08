#include "cachemanager.h"

CacheManager::CacheManager(QObject *parent)
{
    this->setParent(parent);
    QString cachePath = QStandardPaths::writableLocation(
                QStandardPaths::CacheLocation);
    QDir dir;
    if(!dir.exists(cachePath))
        dir.mkpath(cachePath);
    if(!dir.exists(cachePath + "/avatar/friend/"))
        dir.mkpath(cachePath + "/avatar/friend/");
    if(!dir.exists(cachePath + "/avatar/group/"))
        dir.mkpath(cachePath + "/avatar/group/");
    if(!dir.exists(cachePath + "/chat/"))
        dir.mkpath(cachePath + "/chat/");
    if(!dir.exists(cachePath + "/image/"))
        dir.mkpath(cachePath + "/image/");
}

bool CacheManager::loadChatManager(ChatManager *chatManager)
{
    QString cachePath = QStandardPaths::writableLocation(
                QStandardPaths::CacheLocation);
    QFile file(cachePath + "/chat/data");
    if(!file.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QDataStream ds(&file);
    ds >> *chatManager;
    file.close();
    qDebug() << "load chatManager successfully";
    return true;
}

bool CacheManager::saveChatManager(ChatManager *chatManager)
{
    QString cachePath = QStandardPaths::writableLocation(
                QStandardPaths::CacheLocation);
    QFile file(cachePath + "/chat/data");
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "save chatManager unsuccessfully";
        return false;
    }
    QDataStream ds(&file);
    ds << *chatManager;
    file.close();
    qDebug() << "save chatManager successfully";
    return true;
}
