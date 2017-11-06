#ifndef MAINMENUSTACKEDWIDGET_H
#define MAINMENUSTACKEDWIDGET_H

#include <QStackedWidget>
#include <QToolButton>
#include <QGridLayout>
#include "tool/mainfunctionbutton.h"

namespace Ui {
class MainMenuStackedWidget;
}

class MainMenuStackedWidget : public QStackedWidget
{
    Q_OBJECT

public:
    explicit MainMenuStackedWidget(QWidget *parent = 0);
    ~MainMenuStackedWidget();

protected:
    bool eventFilter(QObject *obj, QEvent *ev);
public:
    void addToolButton(MainFunctionButton *button);


public slots:
    void changedPage();

private:
    Ui::MainMenuStackedWidget *ui;
    QGridLayout *currentgridLayout;
};

#endif // MAINMENUSTACKEDWIDGET_H
