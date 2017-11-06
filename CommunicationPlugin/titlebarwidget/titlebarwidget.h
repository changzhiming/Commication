#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsScene>
struct ListItemData;
struct RoomListData;
class MyView;
class MyItem;
namespace Ui {
class TitleWidget;
}

class TitleBarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TitleBarWidget(QWidget *parent = 0);
    ~TitleBarWidget();
protected:
    void paintEvent(QPaintEvent *ev);


public slots:
    /*
     * 使项目在可见区域整体右一个标签的宽度
     * return no
     */
    void on_toolButton_clicked();
    /*
     * 使项目在可见区域整体右一个标签的宽度
     * return no
     */
    void on_toolButton_2_clicked();
    /*
     * 光标移动到item
     * return no
     * @param1 要移动到的item地址
     */
    void focusToItem(ListItemData *item);
    void focusToItem(RoomListData *item);

public:
    /*
     * 增加项目到视图
     * return no
     * @param1 标签项类
     */
    void addItem(MyItem *item);
    MyView *view;

private:
    Ui::TitleWidget *ui;
    int i=0;          //当前项目索引
    int widthItem=0;

};

#endif // WIDGET_H
