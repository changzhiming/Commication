#ifndef ADDRESSWIDGET_H
#define ADDRESSWIDGET_H

#include <QWidget>

namespace Ui {
class AddressWidget;
}

class AddressWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AddressWidget(QWidget *parent = 0);
    ~AddressWidget();

private:
    Ui::AddressWidget *ui;
};

#endif // ADDRESSWIDGET_H
