#include "cachemanager.h"

CacheManager::CacheManager(QString cachePath)
{
    this->cachePath = cachePath;
    manager = new QNetworkAccessManager;
    avatarNum = 0;
    QDir dir;
    if(!dir.exists(cachePath+"avatar/friends/"))
        dir.mkpath(cachePath+"avatar/friends/");
    if(!dir.exists(cachePath+"avatar/groups/"))
        dir.mkpath(cachePath+"avatar/groups/");
    if(!dir.exists(cachePath+"chats/"))
        dir.mkpath(cachePath+"chats/");
    if(!dir.exists(cachePath+"images/"))
        dir.mkpath(cachePath+"images/");
}

QString CacheManager::getAvatar(QString id, int type, int size)
{
    // size 40 100 140 640
    QUrl url;
    QString realPath = "";
    if(type == CacheManager::FriendAvatar)
    {
        url.setUrl("http://q1.qlogo.cn/g?b=qq&s="+QString::number(size)+"&nk="+id);
        realPath = cachePath + QString("avatar/friends/") + id + "_" + QString::number(size);
    }
    else if(type == CacheManager::GroupAvatar)
    {
        url.setUrl("http://p.qlogo.cn/gh/"+id+"/"+id+"/"+QString::number(size));
        realPath = cachePath + QString("avatar/groups/") + id + "_" + QString::number(size);
    }

    if(QFileInfo::exists(realPath)&&QFileInfo(realPath).isFile())
        return realPath;

    avatarNum++;
    if(downloadQueue.isEmpty())
    {
        downloadQueue.enqueue(Task(url, realPath));
        newStart = true;
        startNextDownload();
    }
    else
    {
        downloadQueue.enqueue(Task(url, realPath));
    }

    return realPath;
}

QString CacheManager::getImage(QString file, QString url)
{
    QString realPath = "";
    realPath = cachePath + QString("images/") + file;

    if(QFileInfo::exists(realPath)&&QFileInfo(realPath).isFile())
        return realPath;

    if(downloadQueue.isEmpty())
    {
        downloadQueue.enqueue(Task(url, realPath));
        newStart = true;
        startNextDownload();
    }
    else
    {
        downloadQueue.enqueue(Task(url, realPath));
    }

    return realPath;
}

QString CacheManager::getCard(QString user_id, QString group_id)
{
    if(!cardMap[group_id+"_"+user_id].isEmpty())
        return cardMap[group_id+"_"+user_id];

    emit getCardSignal(user_id, group_id);
    return user_id + "(loading)";
}

bool CacheManager::loadChats(Chats *chats)
{
    QFile file(cachePath+"chats/data");
    if(!file.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QDataStream ds(&file);
    ds >> *chats;
    file.close();
    qDebug() << "load chats data success";
    return true;
}

bool CacheManager::saveChats(Chats *chats)
{
    QFile file(cachePath+"chats/data");
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "save chats data failed";
        return false;
    }
    QDataStream ds(&file);
    ds << *chats;
    file.close();
    qDebug() << "save chats data success";
    return true;
}

void CacheManager::startNextDownload()
{
    if(!newStart)
    {
        if(downloadQueue.head().path.contains(cachePath + QString("images/")))
            emit getImageFinished(downloadQueue.head().path);
        downloadQueue.dequeue();
    }
    newStart = false;

    if(downloadQueue.isEmpty())
    {
        if(avatarNum == 0)
            emit getAvatarFinished();
        return;
    }

    Task t = downloadQueue.head();

    QString filename = t.path;
    output.setFileName(filename);
    if(!output.open(QIODevice::WriteOnly))
    {
        qDebug() << "open failed" << t.path << output.errorString();
        startNextDownload();
        return;
    }

    avatarNum--;
    qDebug() << "download start" << t.url;
    QNetworkRequest request(t.url);
    currentDownload = manager->get(request);
    connect(currentDownload, SIGNAL(finished()), SLOT(downloadFinished()));
    connect(currentDownload, SIGNAL(readyRead()), SLOT(downloadReadyRead()));
}

void CacheManager::downloadFinished()
{
    output.close();

    if(currentDownload->error())
    {
        output.remove();
        qDebug() << "download failed" << currentDownload->url() << currentDownload->errorString();
    }

    currentDownload->deleteLater();
    startNextDownload();
}

void CacheManager::downloadReadyRead()
{
    output.write(currentDownload->readAll());
}
