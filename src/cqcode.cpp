#include "cqcode.h"

// 解析接收到的信息为可显示的格式
QString CQCode::ParseMessageFromString(QString messageString, QString groupID)
{
    QString result = "", temp = "";
    // 匹配 CQ 码
    QRegularExpression regexp("\\[CQ:[\\s\\S]*?\\]");
    int index = 0;
    QRegularExpressionMatch match = regexp.match(messageString, index);
    while(match.hasMatch())
    {
        // CQ 码之前混杂的文本消息
        if(match.capturedStart() > index)
        {
            temp = messageString.mid(index, match.capturedStart()-index);
            temp.replace("<", "&lt;");
            temp.replace(">", "&gt;");
            result += temp;
        }
        // 解析匹配到的 CQ 码
        result += ParseCQCodeFromString(match.captured(), groupID);

        index = match.capturedEnd();
        match = regexp.match(messageString, index);
    }
    // 匹配完 CQ 码之后剩余的部分
    if(messageString.length() > index)
    {
        temp = messageString.mid(index);
        temp.replace("<", "&lt;");
        temp.replace(">", "&gt;");
        result += temp;
    }
    result.replace("\n", "<br />");
    return result;
}

// 解析 CQ 码为可显示的格式
QString CQCode::ParseCQCodeFromString(QString cqcodeString, QString groupID)
{
    QString result = "";
    // CQ 码格式错误
    if(cqcodeString.length() < 5
            ||cqcodeString.left(4) != "[CQ:"
            ||cqcodeString.at(cqcodeString.length()-1) != ']')
    {
        return result;
    }
    QString str = cqcodeString.mid(4, cqcodeString.length() - 5);
    QString type = str.mid(0, str.indexOf(','));
    if(type == "face")
    {
        // QQ 表情
        QString id;
        do
        {
            str = str.mid(str.indexOf(',') + 1);
            type = str.mid(0, str.indexOf('='));

            QString temp;
            if(str.contains(','))
            {
                temp = str.mid(str.indexOf('=') + 1,
                               str.indexOf(',') - str.indexOf('=') - 1);
            }
            else
            {
                temp = str.mid(str.indexOf('=') + 1);
            }

            if(type == "id")
            {
                id = temp;
            }
        }
        while(str.contains(','));
        result += "<span style=\" color: #ffff66;\">/"
                + faceIDToString[id]
                + "</span>";
    }
    else if(type == "emoji")
    {
        // emoji 表情
        QString id;
        do
        {
            str = str.mid(str.indexOf(',') + 1);
            type = str.mid(0, str.indexOf('='));

            QString temp;
            if(str.contains(','))
            {
                temp = str.mid(str.indexOf('=') + 1,
                               str.indexOf(',') - str.indexOf('=') - 1);
            }
            else
            {
                temp = str.mid(str.indexOf('=') + 1);
            }

            if(type == "id")
            {
                id = temp;
            }
        }
        while(str.contains(','));
        result += "&#x" + QString::number(id.toInt(), 16) + ";";
    }
    else if(type == "bface")
    {
        // 原创表情
        result += "<span style=\" color: #ffff66;\">[原创表情]</span>";
    }
    else if(type == "sface")
    {
        // 小表情
        result += "<span style=\" color: #ffff66;\">[小表情]</span>";
    }
    else if(type == "image")
    {
        // 图片
        QString file;
        QString url;
        do
        {
            str = str.mid(str.indexOf(',') + 1);
            type = str.mid(0, str.indexOf('='));

            QString temp;
            if(str.contains(','))
            {
                temp = str.mid(str.indexOf('=') + 1,
                               str.indexOf(',') - str.indexOf('=') - 1);
            }
            else
            {
                temp = str.mid(str.indexOf('=') + 1);
            }

            if(type == "file")
            {
                file = temp;
            }
            else if(type == "url")
            {
                url = temp;
            }
        }
        while(str.contains(','));
        result += "<a href=\"file://"
                + cacheManager->getImage(file, url)
                + "\"><img height=\"50\" src=\"file://"
                + cacheManager->getImage(file, url)
                + "\"/ ></a>";
    }
    else if(type == "record")
    {
        // 语音
        result += "<span style=\" color: #ffff66;\">[语音消息]</span>";
    }
    else if(type == "at")
    {
        // @ 消息
        QString qq;
        do
        {
            str = str.mid(str.indexOf(',') + 1);
            type = str.mid(0, str.indexOf('='));

            QString temp;
            if(str.contains(','))
            {
                temp = str.mid(str.indexOf('=') + 1,
                               str.indexOf(',') - str.indexOf('=') - 1);
            }
            else
            {
                temp = str.mid(str.indexOf('=') + 1);
            }

            if(type == "qq")
            {
                qq = temp;
            }
        }
        while(str.contains(','));
        if(qq == "all")
        {
            result += "<span style=\" color: #ffff66;\">@全体成员 </span>";
        }
        else
        {
            result += "<span style=\" color: #ffff66;\">@"
                    + cacheManager->getCard(groupID, qq)
                    + " </span>";
        }
    }
    else if(type == "rps")
    {
        // 猜拳魔法表情
        result += "<span style=\" color: #ffff66;\">[猜拳魔法表情]</span>";
    }
    else if(type == "dice")
    {
        // 掷骰子魔法表情
        result += "<span style=\" color: #ffff66;\">[掷骰子魔法表情]</span>";
    }
    else if(type == "shake")
    {
        // 戳一戳
        result += "<span style=\" color: #ffff66;\">[戳一戳]</span>";
    }
    else if(type == "anonymous")
    {
        // 匿名消息
        result += "<span style=\" color: #ffff66;\">[匿名消息]</span>";
    }
    else if(type == "music")
    {
        // 音乐分享
        QString from;
        QString baseurl;
        QString id;
        do
        {
            str = str.mid(str.indexOf(',') + 1);
            type = str.mid(0, str.indexOf('='));

            QString temp;
            if(str.contains(','))
            {
                temp = str.mid(str.indexOf('=') + 1,
                               str.indexOf(',') - str.indexOf('=') - 1);
            }
            else
            {
                temp = str.mid(str.indexOf('=') + 1);
            }

            if(type == "type")
            {
                if(temp == "qq")
                {
                    from = "QQ音乐";
                    baseurl = "https://i.y.qq.com/v8/playsong.html?songid=";
                }
                else if(temp == "163")
                {
                    from = "网易云音乐";
                    baseurl = "https://music.163.com/#/song?id=";
                }
                else if(temp == "xiami")
                {
                    from = "虾米音乐";
                    baseurl = "https://www.xiami.com/song/";
                }
                else
                {
                    from = temp;
                    baseurl = "";
                }
            }
            else if(type == "id")
            {
                id = temp;
            }
        }
        while(str.contains(','));
        if(baseurl.isEmpty())
        {
            result += "<span style=\" color: #ffff66;\">[音乐分享]</span><br />"
                    + from + "：" + id;
        }
        else
        {
            result += "<span style=\" color: #ffff66;\">[音乐分享]</span><br />"
                    + from + "：<a href=\"" + baseurl + id + "\">"
                    + id + "</a>";
        }
    }
    else if(type == "share")
    {
        // 链接分享
        QString content;
        QString image;
        QString title;
        QString url;
        do
        {
            str = str.mid(str.indexOf(',') + 1);
            type = str.mid(0, str.indexOf('='));

            QString temp;
            if(str.contains(','))
            {
                temp = str.mid(str.indexOf('=') + 1,
                               str.indexOf(',') - str.indexOf('=') - 1);
            }
            else
            {
                temp = str.mid(str.indexOf('=') + 1);
            }

            if(type == "content")
            {
                content = temp;
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
        while(str.contains(','));
        if(title.isEmpty())
        {
            result += "<span style=\" color: #ffff66;\">[分享]</span><br />"
                    + QString("<a href=\"") + url + "\">"
                    + content + "</a>";
        }
        else
        {
            result += "<span style=\" color: #ffff66;\">[分享]</span><br />"
                    + QString("<a href=\"") + url + "\">"
                    + title  + "</a><br />"
                    + content;
        }
    }
    else if(type == "contact")
    {
        // 联系人分享
        QString from;
        QString id;
        do
        {
            str = str.mid(str.indexOf(',') + 1);
            type = str.mid(0, str.indexOf('='));
            QString temp;
            if(str.contains(','))
            {
                temp = str.mid(str.indexOf('=') + 1,
                               str.indexOf(',') - str.indexOf('=') - 1);
            }
            else
            {
                temp = str.mid(str.indexOf('=') + 1);
            }

            if(type == "type")
            {
                if(temp == "qq")
                {
                    from = "好友";
                }
                else if(temp == "group")
                {
                    from = "群组";
                }
                else
                {
                    from = temp;
                }
            }
            else if(type == "id")
            {
                id = temp;
            }
        }
        while(str.contains(','));
        result += "<span style=\" color: #ffff66;\">[联系人分享]</span><br />"
                + from + "：" + id;
    }
    else if(type == "location")
    {
        // 位置分享
        QString content;
        QString title;
        QString lat, lon;
        do
        {
            str = str.mid(str.indexOf(',') + 1);
            type = str.mid(0, str.indexOf('='));

            QString temp;
            if(str.contains(','))
            {
                temp = str.mid(str.indexOf('=') + 1,
                               str.indexOf(',') - str.indexOf('=') - 1);
            }
            else
            {
                temp = str.mid(str.indexOf('=') + 1);
            }

            if(type == "content")
            {
                content = temp;
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
        while(str.contains(','));
        if(title == "位置分享")
        {
            result += "<span style=\" color: #ffff66;\">[位置分享]</span><br />"
                    + QString("<a href=\"https://www.google.com/maps/place/")
                    + lat + "," + lon + "/@" + lat + "," + lon + ",17z/\">"
                    + content + "</a>";
        }
        else
        {
            result += "<span style=\" color: #ffff66;\">[位置分享]</span><br />"
                    + QString("<a href=\"https://www.google.com/maps/place/")
                    + lat + "," + lon + "/@" + lat + "," + lon + ",17z/\">"
                    + content + title + "</a>";
        }
    }
    else if(type == "show")
    {
        // 厘米秀
        result += "<span style=\" color: #ffff66;\">[厘米秀]</span>";
    }
    else if(type == "sign")
    {
        // 签到
        result += "<span style=\" color: #ffff66;\">[签到]</span>";
    }
    else if(type == "rich")
    {
        // 富媒体
        QString text;
        QString url;
        do
        {
            str = str.mid(str.indexOf(',') + 1);
            type = str.mid(0, str.indexOf('='));

            QString temp;
            if(str.contains(','))
            {
                temp = str.mid(str.indexOf('=') + 1,
                               str.indexOf(',') - str.indexOf('=') - 1);
            }
            else
            {
                temp = str.mid(str.indexOf('=') + 1);
            }

            if(type == "text")
            {
                text = temp;
            }
            else if(type == "url")
            {
                url = temp;
            }
        }
        while(str.contains(','));
        if(url.isEmpty())
        {
            result += "<span style=\" color: #ffff66;\">[富媒体]</span><br />"
                    + text;
        }
        else
        {
            result += "<span style=\" color: #ffff66;\">[富媒体]</span><br />"
                    + QString("<a href=\"") + url + "\">" + text  + "</a>";
        }
    }
    return result;
}

// 将原始消息中需要转义的字符进行转义
QString CQCode::EncodeMessageToCQCodeText(QString messageString)
{
    messageString.replace("&", "&amp;");
    messageString.replace("[", "&#91;");
    messageString.replace("]", "&#93;");
    messageString.replace(",", "&#44;");
    return messageString;
}

// 将图片转换为CQ码
QString CQCode::EncodeImageToCQCodeText(QString fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return "";
    QString imageBase64(file.readAll().toBase64().toStdString().c_str());
        return "[CQ:image,file=base64://" + imageBase64 + "]";
}

// 表情 ID 与表情描述之间的对应关系
// 来自于 https://github.com/catsworld/qq-bot-api/blob/master/cqcode/cqcode.go

const QMap<QString, QString> CQCode::faceIDToString = {
    {"14", "微笑"},
    {"1", "撇嘴"},
    {"2", "色"},
    {"3", "发呆"},
    {"4", "得意"},
    {"5", "流泪"},
    {"6", "害羞"},
    {"7", "闭嘴"},
    {"8", "睡"},
    {"9", "大哭"},
    {"10", "尴尬"},
    {"11", "发怒"},
    {"12", "调皮"},
    {"13", "呲牙"},
    {"0", "惊讶"},
    {"15", "难过"},
    {"16", "酷"},
    {"96", "冷汗"},
    {"18", "抓狂"},
    {"19", "吐"},
    {"20", "偷笑"},
    {"21", "可爱"},
    {"22", "白眼"},
    {"23", "傲慢"},
    {"24", "饥饿"},
    {"25", "困"},
    {"26", "惊恐"},
    {"27", "流汗"},
    {"28", "憨笑"},
    {"29", "大兵"},
    {"30", "奋斗"},
    {"31", "咒骂"},
    {"32", "疑问"},
    {"33", "嘘"},
    {"34", "晕"},
    {"35", "折磨"},
    {"36", "衰"},
    {"37", "骷髅"},
    {"38", "敲打"},
    {"39", "再见"},
    {"97", "擦汗"},
    {"98", "抠鼻"},
    {"99", "鼓掌"},
    {"100", "糗大了"},
    {"101", "坏笑"},
    {"102", "左哼哼"},
    {"103", "右哼哼"},
    {"104", "哈欠"},
    {"105", "鄙视"},
    {"106", "委屈"},
    {"107", "快哭了"},
    {"108", "阴险"},
    {"109", "亲亲"},
    {"110", "吓"},
    {"111", "可怜"},
    {"172", "眨眼睛"},
    {"182", "笑哭"},
    {"179", "doge"},
    {"173", "泪奔"},
    {"174", "无奈"},
    {"212", "托腮"},
    {"175", "卖萌"},
    {"178", "斜眼笑"},
    {"177", "喷血"},
    {"180", "惊喜"},
    {"181", "骚扰"},
    {"176", "小纠结"},
    {"183", "我最美"},
    {"112", "菜刀"},
    {"89", "西瓜"},
    {"113", "啤酒"},
    {"114", "篮球"},
    {"115", "乒乓"},
    {"171", "茶"},
    {"60", "咖啡"},
    {"61", "饭"},
    {"46", "猪头"},
    {"63", "玫瑰"},
    {"64", "凋谢"},
    {"116", "示爱"},
    {"66", "爱心"},
    {"67", "心碎"},
    {"53", "蛋糕"},
    {"54", "闪电"},
    {"55", "炸弹"},
    {"56", "刀"},
    {"57", "足球"},
    {"117", "瓢虫"},
    {"59", "便便"},
    {"75", "月亮"},
    {"74", "太阳"},
    {"69", "礼物"},
    {"49", "拥抱"},
    {"76", "强"},
    {"77", "弱"},
    {"78", "握手"},
    {"79", "胜利"},
    {"118", "抱拳"},
    {"119", "勾引"},
    {"120", "拳头"},
    {"121", "差劲"},
    {"122", "爱你"},
    {"123", "NO"},
    {"124", "OK"},
    {"42", "爱情"},
    {"85", "飞吻"},
    {"43", "跳跳"},
    {"41", "发抖"},
    {"86", "怄火"},
    {"125", "转圈"},
    {"126", "磕头"},
    {"127", "回头"},
    {"128", "跳绳"},
    {"129", "挥手"},
    {"130", "激动"},
    {"131", "街舞"},
    {"132", "献吻"},
    {"133", "左太极"},
    {"134", "右太极"},
    {"136", "双喜"},
    {"137", "鞭炮"},
    {"138", "灯笼"},
    {"140", "K歌"},
    {"144", "喝彩"},
    {"145", "祈祷"},
    {"146", "爆筋"},
    {"147", "棒棒糖"},
    {"148", "喝奶"},
    {"151", "飞机"},
    {"158", "钞票"},
    {"168", "药"},
    {"169", "手枪"},
    {"188", "蛋"},
    {"192", "红包"},
    {"184", "河蟹"},
    {"185", "羊驼"},
    {"190", "菊花"},
    {"187", "幽灵"},
    {"193", "大笑"},
    {"194", "不开心"},
    {"197", "冷漠"},
    {"198", "呃"},
    {"199", "好棒"},
    {"200", "拜托"},
    {"201", "点赞"},
    {"202", "无聊"},
    {"203", "托脸"},
    {"204", "吃"},
    {"205", "送花"},
    {"206", "害怕"},
    {"207", "花痴"},
    {"208", "小样儿"},
    {"210", "飙泪"},
    {"211", "我不看"}
};

const QMap<QString, QString> CQCode::stringToFaceID = {
    {"微笑", "14"},
    {"撇嘴", "1"},
    {"色", "2"},
    {"发呆", "3"},
    {"得意", "4"},
    {"流泪", "5"},
    {"害羞", "6"},
    {"闭嘴", "7"},
    {"睡", "8"},
    {"大哭", "9"},
    {"尴尬", "10"},
    {"发怒", "11"},
    {"调皮", "12"},
    {"呲牙", "13"},
    {"惊讶", "0"},
    {"难过", "15"},
    {"酷", "16"},
    {"冷汗", "96"},
    {"抓狂", "18"},
    {"吐", "19"},
    {"偷笑", "20"},
    {"可爱", "21"},
    {"白眼", "22"},
    {"傲慢", "23"},
    {"饥饿", "24"},
    {"困", "25"},
    {"惊恐", "26"},
    {"流汗", "27"},
    {"憨笑", "28"},
    {"大兵", "29"},
    {"奋斗", "30"},
    {"咒骂", "31"},
    {"疑问", "32"},
    {"嘘", "33"},
    {"晕", "34"},
    {"折磨", "35"},
    {"衰", "36"},
    {"骷髅", "37"},
    {"敲打", "38"},
    {"再见", "39"},
    {"擦汗", "97"},
    {"抠鼻", "98"},
    {"鼓掌", "99"},
    {"糗大了", "100"},
    {"坏笑", "101"},
    {"左哼哼", "102"},
    {"右哼哼", "103"},
    {"哈欠", "104"},
    {"鄙视", "105"},
    {"委屈", "106"},
    {"快哭了", "107"},
    {"阴险", "108"},
    {"亲亲", "109"},
    {"吓", "110"},
    {"可怜", "111"},
    {"眨眼睛", "172"},
    {"笑哭", "182"},
    {"doge", "179"},
    {"泪奔", "173"},
    {"无奈", "174"},
    {"托腮", "212"},
    {"卖萌", "175"},
    {"斜眼笑", "178"},
    {"喷血", "177"},
    {"惊喜", "180"},
    {"骚扰", "181"},
    {"小纠结", "176"},
    {"我最美", "183"},
    {"菜刀", "112"},
    {"西瓜", "89"},
    {"啤酒", "113"},
    {"篮球", "114"},
    {"乒乓", "115"},
    {"茶", "171"},
    {"咖啡", "60"},
    {"饭", "61"},
    {"猪头", "46"},
    {"玫瑰", "63"},
    {"凋谢", "64"},
    {"示爱", "116"},
    {"爱心", "66"},
    {"心碎", "67"},
    {"蛋糕", "53"},
    {"闪电", "54"},
    {"炸弹", "55"},
    {"刀", "56"},
    {"足球", "57"},
    {"瓢虫", "117"},
    {"便便", "59"},
    {"月亮", "75"},
    {"太阳", "74"},
    {"礼物", "69"},
    {"拥抱", "49"},
    {"强", "76"},
    {"弱", "77"},
    {"握手", "78"},
    {"胜利", "79"},
    {"抱拳", "118"},
    {"勾引", "119"},
    {"拳头", "120"},
    {"差劲", "121"},
    {"爱你", "122"},
    {"NO", "123"},
    {"OK", "124"},
    {"爱情", "42"},
    {"飞吻", "85"},
    {"跳跳", "43"},
    {"发抖", "41"},
    {"怄火", "86"},
    {"转圈", "125"},
    {"磕头", "126"},
    {"回头", "127"},
    {"跳绳", "128"},
    {"挥手", "129"},
    {"激动", "130"},
    {"街舞", "131"},
    {"献吻", "132"},
    {"左太极", "133"},
    {"右太极", "134"},
    {"双喜", "136"},
    {"鞭炮", "137"},
    {"灯笼", "138"},
    {"K歌", "140"},
    {"喝彩", "144"},
    {"祈祷", "145"},
    {"爆筋", "146"},
    {"棒棒糖", "147"},
    {"喝奶", "148"},
    {"飞机", "151"},
    {"钞票", "158"},
    {"药", "168"},
    {"手枪", "169"},
    {"蛋", "188"},
    {"红包", "192"},
    {"河蟹", "184"},
    {"羊驼", "185"},
    {"菊花", "190"},
    {"幽灵", "187"},
    {"大笑", "193"},
    {"不开心", "194"},
    {"冷漠", "197"},
    {"呃", "198"},
    {"好棒", "199"},
    {"拜托", "200"},
    {"点赞", "201"},
    {"无聊", "202"},
    {"托脸", "203"},
    {"吃", "204"},
    {"送花", "205"},
    {"害怕", "206"},
    {"花痴", "207"},
    {"小样儿", "208"},
    {"飙泪", "210"},
    {"我不看", "211"}
};
