#ifndef ETopTitleWidget_H
#define ETopTitleWidget_H

#include <QWidget>
#include <QToolButton>
#include <QLabel>
namespace Mail {
class CTagWidget;
}
class ETopTitleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ETopTitleWidget(QWidget *parent = 0);
    ~ETopTitleWidget();

    const QToolButton *addItem(QString noticeName);
    void setButtonCheked(QString noticeName);
    void setTitle(int titlewidth, QString name);

signals:
    void sendclickButton(QString notice);
    void closeemail();
    void maximized();
    void minimized();
public slots:
    void reciverClickSignal(QString clickname);
private:
    Mail::CTagWidget *widgetTagBar;
    QLabel *labelTitle;
};

#endif // ETopTitleWidget_H
