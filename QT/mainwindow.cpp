#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new MySocket();
    ip="210.45.251.160";
    port=8888;
}

MainWindow::~MainWindow()
{
    delete ui;
    if(socket!=NULL){
        socket->close();
        delete socket;
        socket=NULL;
    }
}

bool MainWindow::conn(){
    socket->connectToHost(QHostAddress(ip),port);

    if(!socket->waitForConnected(20000)){
        ui->alert->setText("网络连接错误");
        QFont font("楷体",15,0);
        ui->alert->setFont(font);
        ui->alert->show();
        return false;
    }
    return true;

}
void MainWindow::on_login_clicked()
{
    if(ui->user->text().isEmpty())return ;
    if(ui->pwd->text().isEmpty())return ;
    QString user = ui->user->text();
    QString pwd = ui->pwd->text();

    //连接服务器
    if(!conn())return ;


    short bytes;
    bytes=5;
    socket->write((char*)&bytes,2);
    socket->write("login");

    bytes=user.size();
    socket->write((char*)&bytes,2);
    socket->write(user.toUtf8());

    bytes=pwd.size();
    socket->write((char*)&bytes,2);
    socket->write(pwd.toUtf8());
    socket->flush();

    char buf[1024]={0};
    socket->wait();
    socket->read((char*)&bytes,2);
    qDebug()<<bytes;
    socket->wait();
    socket->read(buf,bytes);
    string res;
    int i=0;
    while(buf[i]!=0)res.push_back(buf[i++]);
    if(res=="success"){
        h = new home();
        h->socket=this->socket;
        acceptDirs();
        this->setVisible(false);
        h->show();
        h->showFiles();
    }else{
        ui->alert->setText("用户名或密码错误");
        ui->alert->setFont(QFont("楷体",15,0));
        this->socket->close();
    }
}
void MainWindow::acceptDirs(){
    qDebug()<<"----------------------";
    short bytes=0;
    QString file_name,file_id;
    string name,id;
    socket->wait();
    socket->read((char*)&bytes,2);
    qDebug()<<bytes;
    char buf[1024]={0};
    while(bytes!=0){
        memset(buf,0,sizeof(buf));
        socket->wait();
        socket->read(buf,bytes);
        int i=0;
        name.clear();
        while(buf[i]!=0)name.push_back(buf[i++]);
        file_name=QString::fromStdString(name);

        socket->wait();
        socket->read((char*)&bytes,2);
        memset(buf,0,sizeof(buf));
        socket->wait();
        socket->read(buf,bytes);
        i=0;
        id.clear();
        while(buf[i]!=0)id.push_back(buf[i++]);
        file_id=QString::fromStdString(id);

        h->map[file_name]=file_id;
        socket->wait();
        socket->read((char*)&bytes,2);
        qDebug()<<file_id<<"   "<<file_name;
    }
}


void MainWindow::on_regist_clicked()
{
    if(ui->user->text().isEmpty())return ;
    if(ui->pwd->text().isEmpty())return ;
    QString user = ui->user->text();
    QString pwd = ui->pwd->text();

    //连接服务器
    if(!conn())return ;

    short bytes;

    bytes=6;
    socket->write((char*)&bytes,2);
    socket->write("regist");

    bytes=user.size();
    socket->write((char*)&bytes,2);
    socket->write(user.toUtf8());

    bytes=pwd.size();
    socket->write((char*)&bytes,2);
    socket->write(pwd.toUtf8());
    socket->flush();

    char buf[1024]={0};
    socket->wait();
    socket->read(buf,sizeof buf);
    socket->close();
    string res;
    int i=0;
    while(buf[i]!=0)res.push_back(buf[i++]);

    if(res=="success"){
        ui->alert->setText("注册成功");
        ui->alert->setFont(QFont("楷体",15,0));
    }else{
        ui->alert->setText("注册失败");
        ui->alert->setFont(QFont("楷体",15,0));
    }
}
