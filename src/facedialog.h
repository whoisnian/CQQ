#ifndef FACEDIALOG_H
#define FACEDIALOG_H

#include <QDialog>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QWidget>

// 表情选择对话框（emoji）
class FaceDialog : public QDialog
{
    Q_OBJECT

public:
    FaceDialog(QWidget *parent = nullptr);

    static const QStringList emojiList;

signals:
    void dialogFinished(QString face);

private slots:
    void dialogFinish(QTableWidgetItem *item);
};

#endif // FACEDIALOG_H
