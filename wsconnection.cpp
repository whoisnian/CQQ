#include "wsconnection.h"

WSConnection::WSConnection(QString address,
                           QString token,
                           CacheManager *cacheManager,
                           QObject *parent)
{
    this->address = address;
    this->token = token;
    this->cacheManager = cacheManager;
    this->setParent(parent);
    this->loop = new QEventLoop(this);
    this->heartbeatTimer = new QTimer(this);

    connect(&wsAPI, SIGNAL(connected()),
            this, SLOT(wsAPIConnected()));
    connect(&wsAPI, SIGNAL(disconnected()),
            this, SLOT(wsAPIDisconnected()));
    connect(&wsEVENT, SIGNAL(connected()),
            this, SLOT(wsEVENTConnected()));
    connect(&wsEVENT, SIGNAL(disconnected()),
            this, SLOT(wsEVENTDisconnected()));

    wsAPI.open(QUrl("ws://"+address+"/api/?access_token="+token));
    loop->exec();
    wsEVENT.open(QUrl("ws://"+address+"/event/?access_token="+token));
    loop->exec();
}

WSConnection::~WSConnection()
{
    qDebug() << "all disconnected";
    wsAPI.close();
    wsEVENT.close();
}

bool WSConnection::isConnected()
{
    if(wsAPI.state() == QAbstractSocket::ConnectedState
            &&wsEVENT.state() == QAbstractSocket::ConnectedState)
        return true;
    return false;
}

void WSConnection::addCommand(CommandType type, QString content)
{
    Command temp;
    temp.content = content;
    temp.type = type;
    if(commandQueue.isEmpty())
    {
        commandQueue.enqueue(temp);
        newStart = true;
        startNextCommand();
    }
    else
    {
        commandQueue.enqueue(temp);
    }
}

void WSConnection::startNextCommand()
{
    if(!newStart)
    {
        commandQueue.dequeue();
    }
    newStart = false;

    if(commandQueue.isEmpty())
    {
        return;
    }

    Command temp = commandQueue.head();
    wsAPI.sendTextMessage(temp.content);
}

void WSConnection::getLoginInfo()
{
    QString content = "{\"action\":\"get_login_info\",\"params\":{}}";
    addCommand(CommandType::get_login_info, content);
}

void WSConnection::wsAPIConnected()
{
    qDebug() << "wsAPI connected";
    connect(&wsAPI, SIGNAL(textMessageReceived(const QString &)),
            this, SLOT(wsAPIReceived(const QString &)));
    loop->exit();
    heartbeatTimer->start(15000);
    connect(heartbeatTimer, SIGNAL(timeout()),
            this, SLOT(sendHeartbeat()));
}

void WSConnection::wsAPIDisconnected()
{
    qDebug() << "wsAPI disconnected";
    loop->exit();
}

void WSConnection::wsAPIReceived(const QString message)
{
    qDebug() << "wsAPI received:" << message;
    QJsonDocument jsonDoc;
    jsonDoc = QJsonDocument::fromJson(message.toLocal8Bit().data());
    if(jsonDoc.isNull())
    {
        qDebug() << "broken wsAPI message";
        return;
    }

    if(commandQueue.head().type == CommandType::get_status)
    {
        qDebug() << "wsAPI status: "
                 << (jsonDoc.object().value("data")
                     .toObject().value("good")
                     .toBool()?"OK":"ERROR");
    }
    else if(commandQueue.head().type == CommandType::get_login_info)
    {
        qDebug() << "get_login_info";
        QString ID, Nickname;
        ID = QString::number(jsonDoc.object().value("data")
                             .toObject().value("user_id")
                             .toVariant().toLongLong());
        Nickname = jsonDoc.object().value("data")
                   .toObject().value("nickname").toString();
        emit getLoginInfoFinished(ID, Nickname);
    }
    else if(commandQueue.head().type == CommandType::_get_friend_list)
    {

    }
    else if(commandQueue.head().type == CommandType::get_group_list)
    {

    }
    else if(commandQueue.head().type == CommandType::get_group_member_info)
    {

    }

    startNextCommand();
}

void WSConnection::wsEVENTConnected()
{
    qDebug() << "wsEVENT connected";
    connect(&wsEVENT, SIGNAL(textMessageReceived(const QString &)),
            this, SLOT(wsEVENTReceived(const QString &)));
    loop->exit();
}

void WSConnection::wsEVENTDisconnected()
{
    qDebug() << "wsEVENT disconnected";
    loop->exit();
}

void WSConnection::wsEVENTReceived(const QString message)
{
    qDebug() << "wsEVENT received:" << message;
    QJsonDocument jsonDoc;
    jsonDoc = QJsonDocument::fromJson(message.toLocal8Bit().data());
    if(jsonDoc.isNull())
    {
        qDebug() << "broken wsEVENT message";
        return;
    }

    if(jsonDoc.object().value("post_type") == "meta_event")
    {
        if(jsonDoc.object().value("meta_event_type") == "heartbeat")
        {
            qDebug() << "wsEVENT status: "
                     << (jsonDoc.object().value("status")
                         .toObject().value("good")
                         .toBool()?"OK":"ERROR");
        }
    }
}

void WSConnection::sendHeartbeat()
{
    if(commandQueue.isEmpty())
    {
        QString content = "{\"action\":\"get_status\",\"params\":{}}";
        addCommand(CommandType::get_status, content);
    }
    heartbeatTimer->start(15000);
}
