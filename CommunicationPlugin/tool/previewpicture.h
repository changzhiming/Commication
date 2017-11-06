#ifndef PREVIEWPICTURE_H
#define PREVIEWPICTURE_H

#include <QLabel>

class PreviewPicture : public QLabel
{
public:
    PreviewPicture(QString picturepath=QString());
    ~PreviewPicture();
    void setPicture(QString picpath)
    {
        picturepath=picpath;
    }

protected:
    void paintEvent(QPaintEvent *painevent);


private:
    QString picturepath;

    int margin=50;
};

#endif // PREVIEWPICTURE_H
