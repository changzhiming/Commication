#ifndef MYAPPLICATION_H
#define MYAPPLICATION_H

#include <QLocalServer>
#include <QLocalSocket>
#include <QApplication>
class MyApplication : public QApplication
{
    Q_OBJECT
public:
    MyApplication(int argc, char *argv[]);
    ~MyApplication();

    bool isRunning();
signals:
    void mainWindowsShow();
private slots:
    void newLocalConnection();
private:
    QLocalServer *server;
    bool isrunning;
};

#endif // MYAPPLICATION_H
