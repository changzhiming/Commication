#ifndef FACEWIDGET_H
#define FACEWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QMenu>
class FaceWidget : public QMenu
{
    Q_OBJECT
public:
    explicit FaceWidget(QWidget *parent = 0);
    ~FaceWidget();
    void setFacePath(QString dirPath);
    void addFacePathDit(QString dirPath);
signals:
    void sendFacePath(QString facePath);

public slots:
    void recivePushbutton();

private:
    QGridLayout * gridLayout;
    QStringList facePath;
    bool enter;
    int iconNum;

};

#endif // FACEWIDGET_H
