#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QFile>
#include <QFileInfo>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QQueue>
#include <QString>

enum TaskType
{
    avatar,
    image
};

struct Task
{
    QUrl url;
    QString path;
    TaskType type;
};

// 从网络下载指定内容
class DownloadManager : public QObject
{
    Q_OBJECT

public:
    DownloadManager(QObject *parent = nullptr);

    void addTask(QUrl url, QString path, TaskType type);

signals:
    void downloadImageFinished(QString filePath, QString url);

private slots:
    void startNextDownload();
    void downloadFinished();
    void downloadReadyRead();

private:
    QNetworkAccessManager *downloadManager;
    QQueue<Task> downloadQueue;
    QNetworkReply *currentDownload;
    QFile output;
    bool newStart;
};

#endif // DOWNLOADMANAGER_H
