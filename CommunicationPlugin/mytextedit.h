#ifndef MYTEXTEDIT_H
#define MYTEXTEDIT_H
#include <QTextEdit>
#include <chatwindow.h>
#include <QKeyEvent>
#include <QImage>
class MyTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    MyTextEdit(QWidget *parent = 0);
    ~MyTextEdit();
    bool canInsertFromMimeData( const QMimeData *source ) const;
    void insertFromMimeData( const QMimeData *source );

protected:
    void keyPressEvent(QKeyEvent *event);
Q_SIGNALS:
    void return_sendMessage();

};

#endif // MYTEXTEDIT_H
