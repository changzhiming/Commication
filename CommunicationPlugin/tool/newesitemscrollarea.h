#ifndef NEWESITEMSCROLLAREA_H
#define NEWESITEMSCROLLAREA_H

#include <QScrollArea>
#include "listFriend/my_listmodel.h"
#include "mainfunctionbutton.h"
namespace Ui {
class Form;
}

class NewesItemScrollArea : public QWidget
{
    Q_OBJECT

public:
    explicit NewesItemScrollArea(MainFunctionButton::MessageType messagetype, QWidget *parent = 0);
    ~NewesItemScrollArea();

    /* 增加消息提示列表项到窗体
     * return no
     * @param1 好友信息结构体
     * @param2 最新好友消息
     */
    void insertListItem(ListItemData *data, QString toolTip);
    /* 设置窗口的标题
     * return no
     * @param1 窗口标题字符串
     */
    void setTitle(QString name);
    /*
     * 存储提列表项和用户数据之间的关联
     */
    QMap<QWidget *, ListItemData *> widgetMaplistItem;
    /*
     * 显示和隐藏关闭按钮
     */
    void removeListItemWidget(QWidget *widget);
    void setCloseShow();
    void setCloseHide();
signals:
    void removeItem(NewesItemScrollArea *item);
    void showFriend(ListItemData * data);
public:
    MainFunctionButton::MessageType messagetype;

protected:
    bool eventFilter(QObject *obj, QEvent *ev);

private slots:
    /*
     * 关闭这个窗体释放 资源
     */
    void on_toolButton_clicked();

private:
    Ui::Form *ui;
};

#endif // NEWESITEMSCROLLAREA_H
