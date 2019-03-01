#include "configmanager.h"

ConfigManager::ConfigManager(QWidget *parent)
{
    this->setParent(parent);
}

// 修改配置对话框
void ConfigManager::changeConfig(bool blocking)
{
    dialog = new QDialog;
    QVBoxLayout *dialogLayout = new QVBoxLayout(dialog);
    QLabel *addressLabel = new QLabel("服务器 WebSocket 监听地址：", dialog);
    addressLineEdit = new QLineEdit(dialog);
    addressLineEdit->setPlaceholderText("1.2.3.4:6700");
    if(!configAddress.isEmpty())
    {
        addressLineEdit->setText(configAddress);
    }
    QLabel *tokenLabel = new QLabel("API 访问 token：", dialog);
    tokenLineEdit = new QLineEdit(dialog);
    tokenLineEdit->setPlaceholderText("Mgep4rV49rM8Jf");
    if(!configToken.isEmpty())
    {
        tokenLineEdit->setText(configToken);
    }

    QWidget *buttonWidget = new QWidget(dialog);
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttonWidget);
    QPushButton *ok = new QPushButton(QIcon::fromTheme("dialog-ok"),
                                      "确定",
                                      buttonWidget);
    connect(ok, SIGNAL(clicked()), dialog, SLOT(accept()));
    QPushButton *cancel = new QPushButton(QIcon::fromTheme("dialog-cancel"),
                                          "取消",
                                          buttonWidget);
    connect(cancel, SIGNAL(clicked()), dialog, SLOT(reject()));
    buttonLayout->addStretch(150);
    buttonLayout->addWidget(ok);
    buttonLayout->addWidget(cancel);
    buttonLayout->setMargin(0);
    buttonWidget->setLayout(buttonLayout);

    dialogLayout->addWidget(addressLabel);
    dialogLayout->addWidget(addressLineEdit);
    dialogLayout->addWidget(tokenLabel);
    dialogLayout->addWidget(tokenLineEdit);
    dialogLayout->addWidget(buttonWidget);
    dialog->setLayout(dialogLayout);
    dialog->setWindowTitle("CQQ 设置");
    dialog->setWindowIcon(QIcon::fromTheme("settings-configure"));
    dialog->setMinimumWidth(300);
    connect(dialog, SIGNAL(finished(int)),
            this, SLOT(changeConfigFinished(int)));

    if(blocking)
    {
        // 阻塞
        dialog->exec();
    }
    else
    {
        // 非阻塞
        dialog->open();
    }
}

// 加载配置文件
bool ConfigManager::loadConfig()
{
    QString configPath = QDir::homePath() + "/.config/CQQ/";
    QDir dir;
    if(!dir.exists(configPath))
    {
        dir.mkpath(configPath);
        return false;
    }
    QFile configFile(configPath + "config");
    if(!configFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QDataStream ds(&configFile);
    ds >> this->configCachePath
       >> this->configAddress
       >> this->configToken
       >> this->configMainWindowWidth
       >> this->configMainWindowHeight
       >> this->configChatWidgetSplitterSizes
       >> this->configMessageTabSplitterSizes
       >> this->configContactTabSplitterSizes;
    configFile.close();
    return true;
}

// 保存配置文件
void ConfigManager::saveConfig()
{
    QString configPath = QDir::homePath() + "/.config/CQQ/";
    QDir dir;
    if(!dir.exists(configPath))
    {
        dir.mkpath(configPath);
    }
    QFile configFile(configPath + "config");
    if(!configFile.open(QIODevice::WriteOnly))
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::NoIcon);
        msgBox.setWindowIcon(QIcon::fromTheme("dialog-warning-symbolic"));
        msgBox.setStyleSheet("QLabel{min-width:250px;}");
        msgBox.setText("配置文件保存失败");
        msgBox.setInformativeText(configFile.errorString());
        msgBox.exec();
        return;
    }
    QDataStream ds(&configFile);
    ds << this->configCachePath
       << this->configAddress
       << this->configToken
       << this->configMainWindowWidth
       << this->configMainWindowHeight
       << this->configChatWidgetSplitterSizes
       << this->configMessageTabSplitterSizes
       << this->configContactTabSplitterSizes;
    configFile.close();
}

// 重置窗口大小配置
void ConfigManager::resetWindowSize()
{
    configMainWindowWidth = 800;
    configMainWindowHeight = 600;

    configChatWidgetSplitterSizes.clear();
    configChatWidgetSplitterSizes.push_back(400);
    configChatWidgetSplitterSizes.push_back(128);

    configMessageTabSplitterSizes.clear();
    configMessageTabSplitterSizes.push_back(200);
    configMessageTabSplitterSizes.push_back(579);

    configContactTabSplitterSizes.clear();
    configContactTabSplitterSizes.push_back(200);
    configContactTabSplitterSizes.push_back(579);
}

// 修改配置文件完成
void ConfigManager::changeConfigFinished(int result)
{
    if(result == QDialog::Accepted)
    {
        this->configAddress = addressLineEdit->text();
        this->configToken = tokenLineEdit->text();
        this->configCachePath = QDir::homePath() + "/.cache/CQQ/";
    }

    delete dialog;
    dialog = nullptr;
    addressLineEdit = nullptr;
    tokenLineEdit = nullptr;
}
