#ifndef TAGWIDGET_H
#define TAGWIDGET_H

#include <QWidget>
class TagWidgetPrivate;
class QVBoxLayout;
class QPushButton;
class QToolButton;

class TagWidget : public QWidget
{
    Q_OBJECT

public:

    explicit TagWidget(QWidget *parent = 0);
    ~TagWidget();
    QToolButton *addItem(QString noticeName);
    void setButtonCheked(QString noticeName);

public Q_SLOTS:
    void recvClickButton();

signals:
    void sendclickButton(QString notice);

private:

    TagWidgetPrivate *d_ptr;
    Q_DECLARE_PRIVATE(TagWidget)
};

#endif // TAGWIDGET_H
