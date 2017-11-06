#ifndef NOTITLE_H
#define NOTITLE_H

#include <QWidget>

class NoTitle: public QWidget
{
    Q_OBJECT

public:
    NoTitle(QWidget *parent=0);
    ~NoTitle();


protected:
    virtual void mousePressEvent(QMouseEvent *ev);
    virtual void mouseMoveEvent(QMouseEvent *ev);
    virtual void mouseReleaseEvent(QMouseEvent *ev);
#ifdef Q_OS_WIN32
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
#endif
private:
    QPoint startPosition;
    bool press=false;
};

#endif // NOTITLE_H
