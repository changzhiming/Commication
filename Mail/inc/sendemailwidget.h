#ifndef SENDEMAILWIDGET_H
#define SENDEMAILWIDGET_H

#include <QWidget>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolBar>
#include <QAction>
#include <QFont>
#include <QPixmap>
#include <QPalette>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QPixmap>
#include <QColor>
#include <QComboBox>
#include <QFontComboBox>
#include <QFile>
#include <QDebug>
#include <QListWidget>
#include <QListWidgetItem>
#include <QFileDialog>
#include <QTextCharFormat>
#include <QColorDialog>
#include <QTextListFormat>
#include <QTextBlockFormat>
#include <QTextList>
#include <QList>
#include <QTextCodec>
#include <QMenu>
#include <QPoint>
#include <QCursor>
#include <QSize>
#include <QObject>
#include "emailmanager.h"
#include "addrecipienter.h"
#include "reademailwidget.h"

namespace Ui {
class SendemailWidget;
}

class SendemailWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SendemailWidget(QWidget *parent = 0);
    ~SendemailWidget();
    EmailManager manager;
    QListWidget *listwidget = new QListWidget;
    typedef struct transmit
    {
        QTextDocument *doc;
        QList<QListWidgetItem *> lw;
    }_trans;

    QAction *actionwriteemail;
    QAction *actionemailbox;
    QAction *actionTextBold;
    QAction *actionTextItalic;
    QAction *actionTextUnderline;
    QComboBox *comboStyle;
    QComboBox *comboSize;
    QComboBox *comboFont;
    QAction *actionAlignLeft;
    QAction *actionAlignCenter;
    QAction *actionAlignRight;
    QAction *actionAlignJustify;
    QAction *actionTextColor;
    QAction *actionTitleSquare;
    QAction *actionTitleDecimal;
    QAction *actionTitleStandard;

    void tabwidegtsheet();
    void lineeditsheet();
    void tabwidgetarea();
    void attemptlistwidgetset();
    void hidesecretrecipient();
    void setupfileaction();
    //发送文本设计相关
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
    void textStyleSelection(const QTextListFormat::Style &style);
    void colorChanged(const QColor &c);

    addRecipienter *addrecipient;
    //转发响应
    void transmitemail(ReademailWidget::_trans transmit);
    //快速回复响应
    void replyemail(QString recipient, QTextDocument *content);

    //user password
    void setuser(QString user = "test@gmail.com");
    void setpassword(QString password = "123456");
    void sethostip(QString host = "192.168.20.110");

private:
    Ui::SendemailWidget *ui;

    QString user;
    QString password;
    QString hostip;

    int clickflag;

private slots:
    //设置发送文本
    void textBold();
    void textItalic();
    void textUnderline();
    void textColor();
    void textAlign(QAction *a);
    void textFamily(const QString &f);
    void textSize(const QString &p);
    void actionTitleStandardStyle();
    void actionTitleDecimalStyle();
    void actionTitleSquareStyle();
    //动作相关
    void showsecretrecipient();
    void slotsendemail();
    void addattemptfile();
    void slotattemptdoubleclick(QListWidgetItem *item);
    void slotsavescribble();
    //添加联系人
    void slotaddcontacts();
    void slotsetaddcontacts(QList<QListWidgetItem*> item);
signals:
    void signalfinish(QString);
};

#endif // SENDEMAILWIDGET_H
