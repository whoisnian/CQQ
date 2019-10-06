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

void QQMailLoginPage::run(ContactList *friendList)
{
    this->friendList = friendList;
    this->load(QUrl("https://mail.qq.com/"));
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
    if(ok&&this->url().toString().contains("https://mail.qq.com/cgi-bin/laddr_lastlist"))
    {
        QTimer::singleShot(100, this, SLOT(getContentSlot()));
    }
}

void QQMailLoginPage::getContentSlot()
{
    this->page()->toPlainText([this](const QString &content) {
        if(content.contains("timeuse")&&content.contains("sortbyupdatetime"))
        {
            qDebug() << content;
            this->destroy();
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
