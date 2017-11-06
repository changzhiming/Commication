#ifndef CTagWidget_H
#define CTagWidget_H

#include <QWidget>
#include <QFile>
#include <QDebug>
class CTagWidgetPrivate;
class QVBoxLayout;
class QPushButton;
class QToolButton;
namespace Mail {

class CTagWidget : public QWidget
{
    Q_OBJECT
    //Q_PROPERTY(QStringList noticeList)

public:
    enum EmailLabel{
        EmailBox=0,
        SendEmail,
        DraftEmail,
        RecycleBox,
        AddressField
    };
    //Q_DECLARE_FLAGS(NoticeLabels, NoticeLabel)

    explicit CTagWidget(QWidget *parent = 0);
    ~CTagWidget();
    QToolButton *addItem(QString noticeName);

    QString getStylesheet(const QString &filename);

    void setButtonCheked(QString noticeName);

public Q_SLOTS:
    void recvClickButton();

Q_SIGNALS:
    void sendclickButton(QString notice);

private:

    CTagWidgetPrivate *d_ptr;
    Q_DECLARE_PRIVATE(CTagWidget)
};
}

#endif // CTagWidget_H
