#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QDebug>
#include<string>
#include<stdio.h>
#include<iostream>
#include<QTcpSocket>
#include<QHostAddress>
#include<QByteArray>
#include<stdlib.h>
#include<home.h>
#include<qmap.h>
#include<mysocket.h>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    bool conn();
    ~MainWindow();
    void acceptDirs();

private slots:
    void on_login_clicked();
    void on_regist_clicked();

private:
    Ui::MainWindow *ui;
    MySocket *socket;
    QString ip;
    quint16 port;
    home *h;
};
#endif // MAINWINDOW_H
