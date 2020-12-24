#include "mysocket.h"
MySocket::MySocket()
{
   connect(this, &MySocket::bytesWritten, this, &MySocket::loopStopSlot);
   connect(this, &MySocket::readyRead, this, &MySocket::loopStopSlot);
}

void MySocket::loopStopSlot(){
    qDebug()<<"aaa";
    if(loop.isRunning())
        loop.quit();
}
void MySocket::wait(){
    if(!loop.isRunning())
        loop.exec();
}
