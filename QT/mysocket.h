#ifndef MYSOCKET_H
#define MYSOCKET_H

#include<QTcpSocket>
#include<QEventLoop>
#include<signal.h>
#include<QEvent>
class MySocket:public QTcpSocket
{
public:
    MySocket();
    void wait();
    QEventLoop loop;
private slots:
    void loopStopSlot();
};

#endif // MYSOCKET_H
