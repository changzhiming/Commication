#ifndef MAINFUNCTIONBUTTON_H
#define MAINFUNCTIONBUTTON_H

#include <QObject>
#include <QToolButton>

class MainFunctionButton : public QToolButton
{
    Q_OBJECT
    /*
     * 窗体类型枚举
     */
    Q_ENUMS(MessageType)
public:
    enum MessageType
    {
        TEXT=0,
        EMAIl,
        NOTICE,
    };

    explicit MainFunctionButton(MainFunctionButton::MessageType messagetype, QWidget *parent = 0);
    ~MainFunctionButton();

    void setNewsNum(int newsnum);
    int getNewsNum();

protected:
    void paintEvent(QPaintEvent *ev);
    void mousePressEvent(QMouseEvent *ev);

private:
    int newsNum=0;       //未读消息的数量
    MainFunctionButton::MessageType messagtType;
};

#endif // MAINFUNCTIONBUTTON_H
