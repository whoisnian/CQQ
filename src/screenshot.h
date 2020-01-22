#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QApplication>
#include <QDateTime>
#include <QKeyEvent>
#include <QPainter>
#include <QRubberBand>
#include <QScreen>
#include <QStandardPaths>
#include <QWindow>

class ResizeableRubberBand : public QWidget
{
    Q_OBJECT

public:
    ResizeableRubberBand(QWidget *parent = nullptr);

private:
    QRubberBand *rubberBand;

protected:
    void resizeEvent(QResizeEvent *event);
};

class Screenshot : public QWidget
{
    Q_OBJECT

public:
    Screenshot(QWidget *parent = nullptr);
    void shot();

signals:
    void saved(QString filePath);

private:
    void saveFile();

    QPixmap oriPixmap;
    QPoint startPos, endPos, moveOriPos;
    ResizeableRubberBand *resizeableRubberBand;

protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
};

#endif // SCREENSHOT_H
