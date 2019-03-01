#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

// 显示好友或群组有关信息
class InfoWidget : public QWidget
{
public:
    InfoWidget(QWidget *parent = nullptr);

    void setInfo(QString avatar,
                 QString number,
                 QString nickname,
                 QString remark = "");

private:
    QLabel *avatarLabel;
    QLabel *numberLabel;
    QLabel *nicknameLabel;
    QLabel *remarkLabel;
};

#endif // INFOWIDGET_H
