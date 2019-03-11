#ifndef CQCODE_H
#define CQCODE_H

#include <QMap>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QString>
#include "cachemanager.h"

// CQ 码转换
class CQCode
{
public:
    CQCode(){}

    static CacheManager *cacheManager;

    // 解析接收到的信息为可显示的格式
    static QString ParseMessageFromString(QString messageString,
                                          QString groupID = "");
    // 解析 CQ 码为可显示的格式
    static QString ParseCQCodeFromString(QString cqcodeString,
                                         QString groupID = "");
    // 将原始消息中需要转义的字符进行转义
    static QString EncodeMessageToCQCodeText(QString messageString);

    // 表情 ID 与表情描述之间的对应关系
    static const QMap<QString, QString> faceIDToString;
    static const QMap<QString, QString> stringToFaceID;
};

#endif // CQCODE_H
