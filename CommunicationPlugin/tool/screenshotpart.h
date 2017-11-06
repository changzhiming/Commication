#ifndef SCREENSHOTPART_H
#define SCREENSHOTPART_H
#include <QDialog>
#include <QTimer>
#include <QPoint>
#include <QPushButton>
#include <QRect>
class ScreenShotPart : public QDialog
{
    Q_OBJECT

public:
    explicit ScreenShotPart(QWidget *parent);
    ~ScreenShotPart();
protected:
    void showEvent(QShowEvent *ev);
    void paintEvent(QPaintEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);

    void mouseReleaseEvent(QMouseEvent *ev);
    bool eventFilter(QObject *obj, QEvent *ev);
public slots:
    void copytoclipboard();
private:
    QPixmap fullScreenPix,fullTempPix;
    bool isDrawing=false;
    QPoint startPoint;
    QPoint endPoint;
    QRect shotRect;
    QScreen *screen;
    QPushButton *complete;
};

#endif // SCREENSHOTPART_H
