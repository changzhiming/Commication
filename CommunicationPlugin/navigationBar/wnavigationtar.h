#ifndef WNAVIGATIONTAR_H
#define WNAVIGATIONTAR_H
#include "wtglobal.h"
#include <QVBoxLayout>
#include <QToolButton>

class WNavigationTarPrivate;
class  WNavigationTar : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QColor currentItemColor READ currentItemColor WRITE setCurrentItemColor)
    Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize)
    Q_PROPERTY(int itemSpacing READ itemSpacing WRITE setItemSpacing)

public:
    WNavigationTar(QWidget *parent = 0);
    ~WNavigationTar();
    QColor& currentItemColor() const;
    void setCurrentItemColor(QColor &color);

    void setIconSize(QSize size);
    QSize &iconSize();

    void setItemSpacing(int spacing);
    int &itemSpacing();

    const QToolButton *addItem(QToolButton *toolButton, QWidget *widget, QString objectname=QString());
private slots:
    void recvClickButton();
protected:

    virtual void mouseMoveEvent(QMouseEvent *ev);
    virtual void mouseReleaseEvent(QMouseEvent *ev);
    virtual void dropEvent(QDropEvent *ev);
    virtual void dragEnterEvent(QDragEnterEvent *ev);
    virtual bool eventFilter(QObject *obj, QEvent *ev);

Q_SIGNALS:
    void clickShowWidget(QWidget *widget);

private:
    WNavigationTarPrivate *d_ptr;
    Q_DECLARE_PRIVATE(WNavigationTar)
    Q_DISABLE_COPY(WNavigationTar)
};
#endif // WNAVIGATIONTAR_H
