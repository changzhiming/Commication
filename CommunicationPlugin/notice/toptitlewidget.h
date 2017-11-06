#ifndef TOPTITLEWIDGET_H
#define TOPTITLEWIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QLabel>
class TagWidget;
class TopTitleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TopTitleWidget(QWidget *parent = 0);
    ~TopTitleWidget();

    const QToolButton *addItem(QString noticeName);
    void setButtonCheked(QString noticeName);
    void setTitle(int titlewidth, QString name);

signals:
    void sendclickButton(QString notice);
public slots:
    void reciverClickSignal(QString clickname);
private:
    TagWidget *widgetTagBar;
    QLabel *labelTitle;
};

#endif // TOPTITLEWIDGET_H
