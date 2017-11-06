#ifndef LABELTHUMBNAIL_H
#define LABELTHUMBNAIL_H

#include <QObject>
#include <QLabel>

class LabelThumbnail : public QLabel
{
public:
    LabelThumbnail(QWidget *parent=0);
    ~LabelThumbnail();

protected:
    void paintEvent(QPaintEvent *ev);
};

#endif // LABELTHUMBNAIL_H
