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
    QPushButton *ok = new QPushButton(UnifiedIcon::getIcon("check"),
                                      "确定",
                                      buttonWidget);
    connect(ok, SIGNAL(clicked()), dialog, SLOT(accept()));
    QPushButton *cancel = new QPushButton(UnifiedIcon::getIcon("close"),
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
    dialog->setWindowIcon(UnifiedIcon::getIcon("build"));
    dialog->setMinimumWidth(300);
    connect(dialog, SIGNAL(finished(int)),
            this, SLOT(changeConfigDialogFinished(int)));

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
    QString configPath = QStandardPaths::writableLocation(
                QStandardPaths::AppConfigLocation);
    QDir dir;
    if(!dir.exists(configPath))
    {
        dir.mkpath(configPath);
        return false;
    }
    QFile configFile(configPath + "/config");
    if(!configFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QDataStream ds(&configFile);
    ds >> this->configAddress
       >> this->configToken
       >> this->configPos
       >> this->configMainWindowWidth
       >> this->configMainWindowHeight
       >> this->configEnableNotify
       >> this->configChatWidgetSplitterSizes
       >> this->configMessageTabSplitterSizes
       >> this->configContactTabSplitterSizes;
    configFile.close();
    return true;
}

// 保存配置文件
void ConfigManager::saveConfig()
{
    QString configPath = QStandardPaths::writableLocation(
                QStandardPaths::AppConfigLocation);
    QDir dir;
    if(!dir.exists(configPath))
    {
        dir.mkpath(configPath);
    }
    QFile configFile(configPath + "/config");
    if(!configFile.open(QIODevice::WriteOnly))
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::NoIcon);
        msgBox.setWindowIcon(UnifiedIcon::getIcon("warn"));
        msgBox.setStyleSheet("QLabel{min-width:250px;}");
        msgBox.setText("配置文件保存失败");
        msgBox.setInformativeText(configFile.errorString());
        msgBox.exec();
        return;
    }
    QDataStream ds(&configFile);
    ds << this->configAddress
       << this->configToken
       << this->configPos
       << this->configMainWindowWidth
       << this->configMainWindowHeight
       << this->configEnableNotify
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
    configMessageTabSplitterSizes.push_back(220);
    configMessageTabSplitterSizes.push_back(559);

    configContactTabSplitterSizes.clear();
    configContactTabSplitterSizes.push_back(220);
    configContactTabSplitterSizes.push_back(559);
}

// 修改配置文件完成
void ConfigManager::changeConfigDialogFinished(int result)
{
    if(result == QDialog::Accepted)
    {
        QString newConfigAddress = addressLineEdit->text();
        QString newConfigToken = tokenLineEdit->text();
        if(this->configAddress != newConfigAddress
                || this->configToken != newConfigToken)
        {
            this->configAddress = newConfigAddress;
            this->configToken = newConfigToken;
            emit configChanged();
        }
    }

    delete dialog;
    dialog = nullptr;
    addressLineEdit = nullptr;
    tokenLineEdit = nullptr;
}
