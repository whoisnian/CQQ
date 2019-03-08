#include "downloadmanager.h"

DownloadManager::DownloadManager(QObject *parent)
{
    downloadManager = new QNetworkAccessManager(parent);
}

void DownloadManager::addTask(QUrl url, QString path, TaskType type)
{
    Task temp;
    temp.url = url;
    temp.path = path;
    temp.type = type;
    if(downloadQueue.isEmpty())
    {
        downloadQueue.enqueue(temp);
        newStart = true;
        startNextDownload();
    }
    else
    {
        downloadQueue.enqueue(temp);
    }
}

void DownloadManager::startNextDownload()
{
    if(!newStart)
    {
        downloadQueue.dequeue();
    }
    newStart = false;

    if(downloadQueue.isEmpty())
    {
        return;
    }

    Task temp = downloadQueue.head();

    if(QFileInfo::exists(temp.path)&&QFileInfo(temp.path).isFile())
    {
        startNextDownload();
        return;
    }

    output.setFileName(temp.path);
    if(!output.open(QIODevice::WriteOnly))
    {
        qDebug() << "Open file failed " << output.errorString();
        startNextDownload();
        return;
    }

    qDebug() << "Downloading " << temp.url;
    QNetworkRequest request(temp.url);
    currentDownload = downloadManager->get(request);
    connect(currentDownload, SIGNAL(finished()),
            this, SLOT(downloadFinished()));
    connect(currentDownload, SIGNAL(readyRead()),
            this, SLOT(downloadReadyRead()));
}

void DownloadManager::downloadFinished()
{
    output.close();

    if(currentDownload->error())
    {
        output.remove();
        qDebug() << "Download failed"
                 << currentDownload->url()
                 << currentDownload->errorString();
    }

    currentDownload->deleteLater();
    startNextDownload();
}

void DownloadManager::downloadReadyRead()
{
    output.write(currentDownload->readAll());
}
