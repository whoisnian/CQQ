#include "screenshot.h"
#include <QDebug>

ResizeableRubberBand::ResizeableRubberBand(QWidget *parent)
    : QWidget(parent)
{
    this->hide();
    this->rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
}

void ResizeableRubberBand::resizeEvent(QResizeEvent *event)
{
    this->rubberBand->resize(event->size());
}

Screenshot::Screenshot(QWidget *parent)
    : QWidget(parent)
{
    this->hide();
    this->setGeometry(QApplication::primaryScreen()->geometry());
    this->setAutoFillBackground(true);
    this->startPos = QPoint(-1, -1);
    this->endPos = QPoint(-1, -1);
    this->moveOriPos = QPoint(-1, -1);
    this->resizeableRubberBand = new ResizeableRubberBand(this);
}

void Screenshot::shot()
{
    QScreen *screen = QApplication::primaryScreen();
    if(QWindow *window = this->windowHandle())
        screen = window->screen();
    if(!screen)
        return;

    oriPixmap = screen->grabWindow(0);
    oriPixmap.setDevicePixelRatio(1.0);

    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(oriPixmap));
    this->setPalette(palette);
    this->showFullScreen();
}

void Screenshot::saveFile()
{
    QString dateStr = QDateTime::currentDateTime().toString("yyyyMMdd");
    QString mSecStr = QString::number(QDateTime::currentMSecsSinceEpoch()%(60*60*24*10));
    QString cachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QString filePath = cachePath + "/screenshot/"
            + "Screenshot_" + dateStr + "_" + mSecStr + ".jpeg";
    oriPixmap.copy(resizeableRubberBand->x(),
                   resizeableRubberBand->y(),
                   resizeableRubberBand->width(),
                   resizeableRubberBand->height()).save(filePath, "JPEG", 90);
    emit saved(filePath);
}

void Screenshot::paintEvent(QPaintEvent *)
{
    QPixmap pm(oriPixmap.size());
    pm.fill(QColor(0, 0, 0, 128));
    QPainter painter(this);
    painter.setBackgroundMode(Qt::OpaqueMode);
    painter.drawPixmap(0, 0, pm);
    if(resizeableRubberBand->isVisible())
        painter.eraseRect(resizeableRubberBand->geometry());
}

void Screenshot::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
    {
        this->close();
    }
    else if(event->key() == Qt::Key_Return)
    {
        qDebug() << "key_return";
        // saveFile
        saveFile();
        this->close();
    }
    event->accept();
}

void Screenshot::mousePressEvent(QMouseEvent *event)
{
    qDebug() << event->pos() << event->type();
    if(event->button() == Qt::LeftButton)
    {
        if(resizeableRubberBand->isVisible())
        {
            if(resizeableRubberBand->geometry().contains(event->pos()))
                moveOriPos = event->pos();
        }
        else
        {
            startPos = event->pos();
            resizeableRubberBand->setGeometry(QRect(startPos, QSize()));
            resizeableRubberBand->show();
        }
    }
    else if(event->button() == Qt::RightButton)
    {
        this->close();
    }
    event->accept();
}

void Screenshot::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << event->pos() << event->type();
    if(endPos.x() < 0)
    {
        resizeableRubberBand->setGeometry(QRect(startPos, event->pos()).normalized());
    }
    else
    {
        if(moveOriPos.x() > 0)
        {
            int x, y;
            x = startPos.x()+event->pos().x()-moveOriPos.x();
            x = qMax(x, 0);
            x = qMin(x, this->width()-resizeableRubberBand->width());
            y = startPos.y()+event->pos().y()-moveOriPos.y();
            y = qMax(y, 0);
            y = qMin(y, this->height()-resizeableRubberBand->height());
            resizeableRubberBand->move(QPoint(x, y));
        }
    }
    event->accept();
}

void Screenshot::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() << event->pos() << event->type();
    if(endPos.x() >= 0)
    {
        this->moveOriPos = QPoint(-1, -1);
    }
    startPos = resizeableRubberBand->geometry().topLeft();
    endPos = resizeableRubberBand->geometry().bottomRight();
    event->accept();
}

void Screenshot::mouseDoubleClickEvent(QMouseEvent *event)
{
    qDebug() << event->pos() << event->type();
    if(resizeableRubberBand->isVisible())
    {
        // saveFile
        saveFile();
        this->close();
    }
    event->accept();
}
