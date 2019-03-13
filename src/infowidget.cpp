#include "infowidget.h"

InfoWidget::InfoWidget(QWidget *parent)
{
    this->setParent(parent);
    QHBoxLayout *infoWidgetLayout = new QHBoxLayout(this);
    QWidget *infoWidgetCenter = new QWidget(this);
    QVBoxLayout *infoWidgetCenterLayout = new QVBoxLayout(infoWidgetCenter);
    avatarLabel = new QLabel(infoWidgetCenter);
    avatarLabel->setAlignment(Qt::AlignCenter);
    numberLabel = new QLabel(infoWidgetCenter);
    numberLabel->setAlignment(Qt::AlignCenter);
    nicknameLabel = new QLabel(infoWidgetCenter);
    nicknameLabel->setAlignment(Qt::AlignCenter);
    remarkLabel = new QLabel(infoWidgetCenter);
    remarkLabel->setAlignment(Qt::AlignCenter);

    infoWidgetCenterLayout->addStretch(1);
    infoWidgetCenterLayout->addWidget(avatarLabel);
    infoWidgetCenterLayout->addWidget(numberLabel);
    infoWidgetCenterLayout->addWidget(nicknameLabel);
    infoWidgetCenterLayout->addWidget(remarkLabel);
    infoWidgetCenterLayout->addStretch(1);
    infoWidgetCenter->setLayout(infoWidgetCenterLayout);

    infoWidgetLayout->addStretch(1);
    infoWidgetLayout->addWidget(infoWidgetCenter);
    infoWidgetLayout->addStretch(1);
    this->setLayout(infoWidgetLayout);
}

void InfoWidget::setInfo(QString avatar,
                         QString number,
                         QString nickname,
                         QString remark)
{
    avatar = "<img src=\""
            + avatar
            + "\" width=\"100\"  height=\"100\" />";
    if(avatarLabel->text() == avatar)
    {
        return;
    }
    avatarLabel->setText(avatar);
    numberLabel->setText(number);
    // 不提供remark时认为显示的是群组信息
    // 提供remark时认为显示的是好友信息
    // 提供的remark为空时认为显示的是不需要显示备注的好友信息
    if(remark.isNull())
    {
        nicknameLabel->setText("名称：" + nickname);
        remarkLabel->setText("");
    }
    else
    {
        nicknameLabel->setText("昵称：" + nickname);
        if(remark.isEmpty())
        {
            remarkLabel->setText("");
        }
        else
        {
            remarkLabel->setText("备注：" + remark);
        }
    }
}
