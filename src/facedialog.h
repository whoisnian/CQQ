#ifndef FACEDIALOG_H
#define FACEDIALOG_H

#include <QDialog>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMouseEvent>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QWidget>
#include "unifiedicon.h"

// 表情选择对话框（emoji）
class FaceDialog : public QDialog
{
    Q_OBJECT

public:
    FaceDialog(QWidget *parent = nullptr);

    static const QStringList emojiList;

signals:
    void dialogFinished(QString face);

protected:
    void mousePressEvent(QMouseEvent *event);
};

#endif // FACEDIALOG_H
