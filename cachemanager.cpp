#include "cachemanager.h"

CacheManager::CacheManager(QObject *parent)
{
    this->setParent(parent);
    downloadManager = new DownloadManager(this);
    cachePath = QStandardPaths::writableLocation(
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
    QFile file(cachePath + "/chat/data");
    if(!file.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QDataStream ds(&file);
    ds >> *chatManager
            >> nicknameMap
            >> remarkMap
            >> groupnameMap
            >> cardMap;
    file.close();
    qDebug() << "load chatManager successfully";
    return true;
}

bool CacheManager::saveChatManager(ChatManager *chatManager)
{
    QFile file(cachePath + "/chat/data");
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "save chatManager unsuccessfully";
        return false;
    }
    QDataStream ds(&file);
    ds << *chatManager
       << nicknameMap
       << remarkMap
       << groupnameMap
       << cardMap;
    file.close();
    qDebug() << "save chatManager successfully";
    return true;
}

QString CacheManager::getAvatar(QString ID, AvatarType type, int size)
{
    // size 40 100 140 640
    QUrl url;
    QString realPath = "";
    if(type == AvatarType::Friend)
    {
        url.setUrl("http://q1.qlogo.cn/g?b=qq&s="
                   + QString::number(size) + "&nk=" + ID);
        realPath = cachePath + "/avatar/friend/"
                + ID + "_" + QString::number(size);
    }
    else if(type == AvatarType::Group)
    {
        url.setUrl("http://p.qlogo.cn/gh/"
                   + ID + "/" + ID + "/" + QString::number(size));
        realPath = cachePath + "/avatar/group/"
                + ID + "_" + QString::number(size);
    }

    if(QFileInfo::exists(realPath)&&QFileInfo(realPath).isFile())
        return realPath;

    downloadManager->addTask(url, realPath, TaskType::avatar);
    return realPath;
}

QString CacheManager::getImage(QString file, QString urlString)
{
    QUrl url(urlString);
    QString realPath = "";
    realPath = cachePath + "/image/" + file;

    if(QFileInfo::exists(realPath)&&QFileInfo(realPath).isFile())
        return realPath;

    downloadManager->addTask(url, realPath, TaskType::image);
    return realPath;
}

QString CacheManager::getCard(QString groupID, QString userID)
{
    if(!cardMap[groupID + "_" + userID].isEmpty())
    {
        return cardMap[groupID + "_" + userID];
    }

    emit getCardSignal(userID, groupID);
    return userID + "(loading)";
}
