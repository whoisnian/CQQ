#include "cachemanager.h"

CacheManager::CacheManager(QObject *parent)
{
    this->setParent(parent);
    downloadManager = new DownloadManager(this);
    connect(downloadManager, SIGNAL(downloadImageFinished(QString, QString)),
            this, SLOT(downloadImageFinished(QString, QString)));
    connect(downloadManager, SIGNAL(downloadAvatarFinished(QString, QString)),
            this, SLOT(downloadAvatarFinished(QString, QString)));
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
    if(!dir.exists(cachePath + "/screenshot/"))
        dir.mkpath(cachePath + "/screenshot/");
}

// 从缓存目录加载聊天记录缓存
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

// 保存聊天记录到缓存目录
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

// 获取头像
QString CacheManager::getAvatar(QString ID, AvatarType type, int size)
{
    // size 可选值 40 100 140 640
    QUrl url;
    QString realPath = "";

    if(type == AvatarType::Friend)
    {
        url.setUrl("https://q1.qlogo.cn/g?b=qq&s="
                   + QString::number(size) + "&nk=" + ID);
        realPath = cachePath + "/avatar/friend/"
                + ID + "_" + QString::number(size);
    }
    else if(type == AvatarType::Group)
    {
        url.setUrl("https://p.qlogo.cn/gh/"
                   + ID + "/" + ID + "/" + QString::number(size));
        realPath = cachePath + "/avatar/group/"
                + ID + "_" + QString::number(size);
    }

    // 图片文件存在且不为空
    if(QFileInfo::exists(realPath)
            &&QFileInfo(realPath).isFile()
            &&QFileInfo(realPath).size() > 10)
    {
        avatarNum--;
        return realPath;
    }

    // 添加下载任务
    downloadManager->addTask(url, realPath, TaskType::avatar);
    return realPath;
}

// 获取图片
QString CacheManager::getImage(QString file, QString urlString)
{
    QUrl url(urlString);
    QString realPath = "";
    realPath = cachePath + "/image/" + file;

    // 图片文件存在且不为空
    if(QFileInfo::exists(realPath)
            &&QFileInfo(realPath).isFile()
            &&QFileInfo(realPath).size() > 10)
        return realPath;

    if(url.isEmpty())
    {
        return "";
    }
    downloadManager->addTask(url, realPath, TaskType::image);
    return realPath;
}

// 获取群名片
QString CacheManager::getCard(QString groupID, QString userID)
{
    if(!cardMap[groupID + "_" + userID].isEmpty())
    {
        return cardMap[groupID + "_" + userID];
    }

    emit getCardSignal(groupID, userID);
    return userID + "(loading)";
}

// 将本地图片保存到缓存目录
QString CacheManager::cacheImage(QString fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return "";
    // 以图片的MD5作为文件名
    QCryptographicHash imageMD5(QCryptographicHash::Md5);
    imageMD5.addData(file.readAll());
    QString result(imageMD5.result().toHex().toUpper().toStdString().c_str());
    result = result + "." + QFileInfo(fileName).suffix();
    file.copy(cachePath + "/image/" + result);
    return result;
}

void CacheManager::openCacheDir()
{
    QDesktopServices::openUrl("file://" + cachePath);
}

void CacheManager::clearCacheDir()
{
    QDir dir;
    if(dir.exists(cachePath))
        QDir(cachePath).removeRecursively();
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

void CacheManager::downloadImageFinished(QString filePath, QString url)
{
    emit getImageFinished(filePath, url);
}

void CacheManager::downloadAvatarFinished(QString, QString)
{
    avatarNum--;
    if(avatarNum == 0)
    {
        emit getAllAvatarFinished();
    }
}
