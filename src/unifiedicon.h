#ifndef UNIFIEDICON_H
#define UNIFIEDICON_H

#include <QIcon>
#include <QPainter>
#include <QSvgRenderer>

class UnifiedIcon
{
public:
    // style: "dark" or "light"
    static void setStyle(QString style);
    static QString getStyle();
    static QIcon getIcon(QString name);

private:
    static QIcon generateIcon(QString xml);
    static QString iconColor;
};

#endif // UNIFIEDICON_H
