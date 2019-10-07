#include "fetchfriends.h"

QQMailLoginPage::QQMailLoginPage(QWidget *parent)
{
    this->setParent(parent);
    this->page()->profile()->cookieStore()->deleteAllCookies();
    MyInterceptor *interceptor = new MyInterceptor(this);
    this->page()->setUrlRequestInterceptor(interceptor);
    connect(this, SIGNAL(urlChanged(const QUrl &)),
            this, SLOT(urlChangedSlot(const QUrl &)));
    connect(this, SIGNAL(loadFinished(bool)),
            this, SLOT(loadFinishedSlot(bool)));
}

void QQMailLoginPage::run(CacheManager *cacheManager, ContactList *friendList)
{
    this->cacheManager = cacheManager;
    this->friendList = friendList;
    this->load(QUrl("https://mail.qq.com/"));
    //this->load(QUrl("http://127.0.0.1:16377/friends.html"));
    this->show();
}

void QQMailLoginPage::urlChangedSlot(const QUrl &url)
{
    QString u = url.toString();
    if(u.contains("https://mail.qq.com/cgi-bin/frame_html"))
    {
        this->hide();
        int from = u.indexOf("sid=")+4;
        QString sid = u.mid(from, u.indexOf("&", from) - from);
        qDebug() << "sid:" << sid;
        this->load(QUrl("https://mail.qq.com/cgi-bin/laddr_lastlist?sid="
                        + sid + "&t=addr_datanew&category=hot"));
        //        QWebEngineHttpRequest req(QUrl("http://127.0.0.1:8000/cgi-bin/laddr_lastlist?sid="
        //                                       + sid + "&t=addr_datanew&category=hot"));
        //        req.setHeader("Referer", "https://mail.qq.com/");
        //        this->load(req);
    }
}

void QQMailLoginPage::loadFinishedSlot(bool ok)
{
    if(ok||this->url().toString().contains("https://mail.qq.com/cgi-bin/laddr_lastlist"))
    {
        QTimer::singleShot(100, this, SLOT(getContentSlot()));
    }
}

void QQMailLoginPage::getContentSlot()
{
    this->page()->toPlainText([this](const QString &content) {
        if(content.contains("timeuse")&&content.contains("sortbyupdatetime"))
        {
            QString jsonContent = content.mid(1, content.length()-2);
            jsonContent.replace("{timeuse", "{\"timeuse\"");
            jsonContent.replace(",sortbyupdatetime", ",\"sortbyupdatetime\"");
            jsonContent.replace(",qqgroups", ",\"qqgroups\"");
            jsonContent.replace(",groups", ",\"groups\"");
            jsonContent.replace(",domaingroups", ",\"domaingroups\"");
            jsonContent.replace(",tool", ",\"tool\"");
            jsonContent.replace(",ggroup", ",\"ggroup\"");
            jsonContent.replace(",]", "]");
            jsonContent.replace("\\x26", "&");
            jsonContent.replace("\\x5c", "&#x5c;");
            qDebug() << jsonContent;
            QJsonDocument jsonDoc;
            jsonDoc = QJsonDocument::fromJson(jsonContent.toLocal8Bit().data());
            if(jsonDoc.isNull())
            {
                qDebug() << "broken json";
                return;
            }
            QJsonArray data = jsonDoc.object().value("sortbyupdatetime").toArray();
            QTextDocument text;
            QMap<long long, QString> qqMap, nicknameMap, remarkMap;
            for(int i = 0;i < data.size();i++)
            {
                long long key = data[i].toArray()[0].toString().toLongLong();
                QString qqMail = data[i].toArray()[2].toString();
                qqMap[key] = qqMail.mid(0, qqMail.length()-7);
                text.setHtml(data[i].toArray()[3].toString());
                nicknameMap[key] = text.toPlainText();
                text.setHtml(data[i].toArray()[8].toString());
                remarkMap[key] = text.toPlainText();
            }
            friendList->clear();
            QJsonArray groups = jsonDoc.object().value("qqgroups").toArray();
            for(int i = 0;i < groups.size();i++)
            {
                QTreeWidgetItem *topItem;
                QString friendGroupName;
                friendGroupName = groups.at(i)
                        .toArray().at(2)
                        .toString();
                text.setHtml(friendGroupName);
                friendGroupName = text.toPlainText();
                topItem = friendList->addTopItem(friendGroupName);
                QJsonArray friends = groups.at(i)
                        .toArray().at(1)
                        .toArray();
                for(int j = 0;j < friends.size();j++)
                {
                    long long key = friends.at(j).toVariant().toLongLong();
                    cacheManager->avatarNum++;
                    QString avatar = cacheManager->getAvatar(qqMap[key],
                                                     CacheManager::Friend,
                                                     100);
                    if(remarkMap[key].isEmpty())
                    {
                        remarkMap[key] = nicknameMap[key];
                    }
                    cacheManager->nicknameMap[qqMap[key]] = nicknameMap[key];
                    cacheManager->remarkMap[qqMap[key]] = remarkMap[key];
                    friendList->addChildItem(remarkMap[key], nicknameMap[key], qqMap[key],
                                             topItem, avatar);
                }
            }
            this->deleteLater();
        }
    });
}

MyInterceptor::MyInterceptor(QObject *parent)
{
    this->setParent(parent);
}

void MyInterceptor::interceptRequest(QWebEngineUrlRequestInfo &info)
{
    qDebug() << "intercept" << info.requestUrl();
    if(info.requestUrl().toString().contains("https://mail.qq.com/cgi-bin/laddr_lastlist"))
    {
        info.setHttpHeader("Referer", "https://mail.qq.com/");
    }
}
