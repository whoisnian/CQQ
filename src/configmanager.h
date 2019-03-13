#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QDataStream>
#include <QDebug>
#include <QDialog>
#include <QDir>
#include <QFile>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QList>
#include <QMessageBox>
#include <QPushButton>
#include <QStandardPaths>
#include <QString>
#include <QVBoxLayout>

// 配置信息存储
class ConfigManager : public QObject
{
    Q_OBJECT

public:
    ConfigManager(QWidget *parent = nullptr);

    // 修改配置对话框
    void changeConfig(bool blocking = false);

    // 加载配置文件
    bool loadConfig();

    // 保存配置文件
    void saveConfig();

    // 重置窗口大小配置
    void resetWindowSize();

private slots:
    void changeConfigFinished(int result);

public:
    QString configAddress;          // COOLQ 服务器地址
    QString configToken;            // COOLQ Token

    int configMainWindowWidth;      // 主窗口宽度
    int configMainWindowHeight;     // 主窗口高度

    QList<int> configChatWidgetSplitterSizes;   // 消息 Tab 中的聊天区域布局
    QList<int> configMessageTabSplitterSizes;   // 主 TabWidget 的消息 Tab 布局
    QList<int> configContactTabSplitterSizes;   // 主 TabWidget 的联系人 Tab 布局

private:
    QDialog *dialog = nullptr;
    QLineEdit *addressLineEdit = nullptr;
    QLineEdit *tokenLineEdit = nullptr;
};

#endif // CONFIGMANAGER_H
