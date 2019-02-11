#include "cqcode.h"

QString CQCode::ParseMessageFromString(QString messageString, QString group_id)
{
    QString res = "";
    QRegularExpression regexp("\\[CQ:[\\s\\S]*?\\]");
    int index = 0;
    QRegularExpressionMatch match = regexp.match(messageString, index);
    while(match.hasMatch())
    {
        if(match.capturedStart() > index)
            res += messageString.mid(index, match.capturedStart()-index);
        res = res + ParseCQCode(match.captured(), group_id);
        index = match.capturedEnd();
        match = regexp.match(messageString, index);
    }
    if(messageString.length() > index)
        res += messageString.mid(index);
    return res;
}

QString CQCode::ParseCQCode(QString cqcode, QString group_id)
{
    QString res = "";
    if(cqcode.length() < 5||cqcode.left(4) != "[CQ:"||cqcode.at(cqcode.length()-1) != ']')
        return res;
    QString content = cqcode.mid(4, cqcode.length()-5);
    QString type = content.mid(0, content.indexOf(','));
    if(type == "face")
    {
        QString id;
        do
        {
            content = content.mid(content.indexOf(',')+1);
            type = content.mid(0, content.indexOf('='));
            QString temp;
            if(content.contains(','))
                temp = content.mid(content.indexOf('=')+1, content.indexOf(',')-content.indexOf('=')-1);
            else
                temp = content.mid(content.indexOf('=')+1);
            if(type == "id")
            {
                id = temp;
            }
        }
        while(content.contains(','));
        qDebug() << id;
        res += "<span style=\" color: #ffff66;\">/" + faceString[id] + "</span>";
    }
    else if(type == "emoji")
    {
        QString id;
        do
        {
            content = content.mid(content.indexOf(',')+1);
            type = content.mid(0, content.indexOf('='));

            QString temp;
            if(content.contains(','))
                temp = content.mid(content.indexOf('=')+1, content.indexOf(',')-content.indexOf('=')-1);
            else
                temp = content.mid(content.indexOf('=')+1);
            if(type == "id")
            {
                id = temp;
            }
        }
        while(content.contains(','));
        res += "&#x" + QString::number(id.toInt(), 16) + ";";
    }
    else if(type == "bface")
    {
        res += "<span style=\" color: #ffff66;\">[原创表情]</span>";
    }
    else if(type == "sface")
    {
        res += "<span style=\" color: #ffff66;\">[小表情]</span>";
    }
    else if(type == "image")
    {
        QString file;
        QString url;
        do
        {
            content = content.mid(content.indexOf(',')+1);
            type = content.mid(0, content.indexOf('='));

            QString temp;
            if(content.contains(','))
                temp = content.mid(content.indexOf('=')+1, content.indexOf(',')-content.indexOf('=')-1);
            else
                temp = content.mid(content.indexOf('=')+1);
            if(type == "file")
            {
                file = temp;
            }
            else if(type == "url")
            {
                url = temp;
            }
        }
        while(content.contains(','));
        res += "<a href=\"" + url + "\"><img height=\"50\" src=\"" + CQCode::cacheManager->getImage(file, url) + "\"/ ></a><br />";
    }
    else if(type == "record")
    {
        res += "<span style=\" color: #ffff66;\">[语音消息]</span>";
    }
    else if(type == "at")
    {
        QString qq;
        do
        {
            content = content.mid(content.indexOf(',')+1);
            type = content.mid(0, content.indexOf('='));

            QString temp;
            if(content.contains(','))
                temp = content.mid(content.indexOf('=')+1, content.indexOf(',')-content.indexOf('=')-1);
            else
                temp = content.mid(content.indexOf('=')+1);
            if(type == "qq")
            {
                qq = temp;
            }
        }
        while(content.contains(','));
        if(qq == "all")
            res += "<span style=\" color: #ffff66;\">@全体成员 </span>";
        else
            res += "<span style=\" color: #ffff66;\">@" + cacheManager->getCard(qq, group_id) + " </span>";
    }
    else if(type == "rps")
    {
        res += "<span style=\" color: #ffff66;\">[猜拳魔法表情]</span>";
    }
    else if(type == "dice")
    {
        res += "<span style=\" color: #ffff66;\">[掷骰子魔法表情]</span>";
    }
    else if(type == "shake")
    {
        res += "<span style=\" color: #ffff66;\">[戳一戳]</span>";
    }
    else if(type == "anonymous")
    {
        res += "<span style=\" color: #ffff66;\">[匿名消息]</span>";
    }
    else if(type == "music")
    {
        QString from;
        QString baseurl;
        bool haveurl = false;
        QString id;
        do
        {
            content = content.mid(content.indexOf(',')+1);
            type = content.mid(0, content.indexOf('='));

            QString temp;
            if(content.contains(','))
                temp = content.mid(content.indexOf('=')+1, content.indexOf(',')-content.indexOf('=')-1);
            else
                temp = content.mid(content.indexOf('=')+1);
            if(type == "type")
            {
                if(temp == "qq")
                {
                    from = "QQ音乐";
                    baseurl = "https://i.y.qq.com/v8/playsong.html?songid=";
                    haveurl = true;
                }
                else if(temp == "163")
                {
                    from = "网易云音乐";
                    baseurl = "https://music.163.com/#/song?id=";
                    haveurl = true;
                }
                else if(temp == "xiami")
                {
                    from = "虾米音乐";
                    baseurl = "https://www.xiami.com/song/";
                    haveurl = true;
                }
                else
                {
                    from = temp;
                    baseurl = "";
                    haveurl = false;
                }
            }
            else if(type == "id")
            {
                id = temp;
            }
        }
        while(content.contains(','));
        if(haveurl)
            res += "<span style=\" color: #ffff66;\">[音乐分享]</span><br />" + from + "：<a href=\"" + baseurl + id + "\">" + id + "</a>";
        else
            res += "<span style=\" color: #ffff66;\">[音乐分享]</span><br />" + from + "：" + id;
    }
    else if(type == "share")
    {
        QString description;
        QString image;
        QString title;
        QString url;
        do
        {
            content = content.mid(content.indexOf(',')+1);
            type = content.mid(0, content.indexOf('='));

            QString temp;
            if(content.contains(','))
                temp = content.mid(content.indexOf('=')+1, content.indexOf(',')-content.indexOf('=')-1);
            else
                temp = content.mid(content.indexOf('=')+1);
            if(type == "content")
            {
                description = temp;
            }
            else if(type == "image")
            {
                image = temp;
            }
            else if(type == "title")
            {
                title = temp;
            }
            else if(type == "url")
            {
                url = temp;
            }
        }
        while(content.contains(','));
        if(!title.isEmpty())
            res += "<span style=\" color: #ffff66;\">[分享]</span><br /><a href=\"" + url + "\">" + title  + "</a>" + "<br />" + description;
        else
            res += "<span style=\" color: #ffff66;\">[分享]</span><br /><a href=\"" + url + "\">" + description  + "</a>";
    }
    else if(type == "contact")
    {
        QString from;
        QString id;
        do
        {
            content = content.mid(content.indexOf(',')+1);
            type = content.mid(0, content.indexOf('='));

            QString temp;
            if(content.contains(','))
                temp = content.mid(content.indexOf('=')+1, content.indexOf(',')-content.indexOf('=')-1);
            else
                temp = content.mid(content.indexOf('=')+1);
            if(type == "type")
            {
                if(temp == "qq")
                    from = "好友";
                else if(temp == "group")
                    from = "群组";
                else
                    from = temp;
            }
            else if(type == "id")
            {
                id = temp;
            }
        }
        while(content.contains(','));
        res += "<span style=\" color: #ffff66;\">[联系人分享]</span><br />" + from + "：" + id;
    }
    else if(type == "location")
    {
        QString description;
        QString title;
        QString lat, lon;
        do
        {
            content = content.mid(content.indexOf(',')+1);
            type = content.mid(0, content.indexOf('='));

            QString temp;
            if(content.contains(','))
                temp = content.mid(content.indexOf('=')+1, content.indexOf(',')-content.indexOf('=')-1);
            else
                temp = content.mid(content.indexOf('=')+1);
            if(type == "content")
            {
                description = temp;
            }
            if(type == "title")
            {
                title = temp;
            }
            else if(type == "lat")
            {
                lat = temp;
            }
            else if(type == "lon")
            {
                lon = temp;
            }
        }
        while(content.contains(','));
        if(title == "位置分享")
            res += "<span style=\" color: #ffff66;\">[位置分享]</span><br /><a href=\"https://www.google.com/maps/place/" + lat + "," + lon + "/@" + lat + "," + lon + ",17z/\">" + description + "</a>";
        else
            res += "<span style=\" color: #ffff66;\">[位置分享]</span><br /><a href=\"https://www.google.com/maps/place/" + lat + "," + lon + "/@" + lat + "," + lon + ",17z/\">" + description + title + "</a>";
    }
    else if(type == "show")
    {
        res += "<span style=\" color: #ffff66;\">[厘米秀]</span>";
    }
    else if(type == "sign")
    {
        res += "<span style=\" color: #ffff66;\">[签到]</span>";
    }
    else if(type == "rich")
    {
        QString text;
        do
        {
            content = content.mid(content.indexOf(',')+1);
            type = content.mid(0, content.indexOf('='));

            QString temp;
            if(content.contains(','))
                temp = content.mid(content.indexOf('=')+1, content.indexOf(',')-content.indexOf('=')-1);
            else
                temp = content.mid(content.indexOf('=')+1);
            if(type == "text")
            {
                text = temp;
            }
        }
        while(content.contains(','));
        res += "<span style=\" color: #ffff66;\">[富媒体信息]</span><br />" + text;
    }
    return res;
}

QString CQCode::EncodeToCQCodeText(QString message)
{
    message.replace("&", "&amp;");
    message.replace("[", "&#91;");
    message.replace("]", "&#93;");
    message.replace(",", "&#44;");
    return message;
}

QString CQCode::EncodeToCQCodeJsonText(QString message)
{
    message.replace("&", "&amp;");
    message.replace("[", "&#91;");
    message.replace("]", "&#93;");
    message.replace(",", "&#44;");
    message.replace("\"", "\\\"");
    return message;
}

QString CQCode::EncodeImageToCQCode(QString fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return "open image failed";
    QString imageBase64(file.readAll().toBase64().toStdString().c_str());
        return "[CQ:image,file=base64://" + imageBase64 + "]";
}
