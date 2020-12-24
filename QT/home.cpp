#include "home.h"
#include "ui_home.h"

home::home(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::home)
{
    ui->setupUi(this);
    this->buts =QVector<QPushButton*>();
}

home::~home()
{
    delete ui;
}

void home::showFiles(){
    //添加button
    QMap<QString,QString>::iterator it = this->map.begin();
    int start=30;
    int i=0;
    while(it!=this->map.end()){
        QPushButton *btn = new QPushButton();
        QString file_name=it.key();
        btn->setText(file_name);
        btn->setFont(QFont("楷体",15,0));
        btn->resize(760,30);
        btn->setParent(this);
        btn->move(0,start+(i++)*30);
        btn->show();
        this->buts.push_back(btn);
        it++;
        connect(btn,&QPushButton::clicked,this,[file_name,this]()->void{
           download(file_name);
         });
    }
}
void home::download(QString file_name){
    QString filename=QFileDialog::getExistingDirectory(this,"选择文件","C:/");
    if(filename.size()==0)return ;
    filename+="/";
    filename+=file_name;

    short bytes=8;
    socket->write((char*)&bytes,2);
    this->socket->write("download");

    bytes=this->map[file_name].size();
    socket->write((char*)&bytes,2);
    this->socket->write(map[file_name].toUtf8());


    socket->waitForReadyRead(-1);
    QFile file(filename);
    file.open(QIODevice::ReadWrite);
    char buf[1024];
    int len=0;
    while(1){
        len=socket->read(buf,sizeof(buf));
        if(len<=0)break;
        file.write(buf,len);
    }
    file.close();

}
void home::on_upload_clicked()
{
    QString filename=QFileDialog::getOpenFileName(this,"选择文件","C:/");
    if(filename.size()==0)return;
    QFile file(filename);
    file.open(QIODevice::ReadOnly);

    short bytes;
    bytes=6;
    socket->write((char*)&bytes,2);
    socket->write("upload");

    string name=filename.toStdString();
    for(int i=name.size()-1;i>=0;i--){
        if(name[i]=='/'){
            name=name.substr(i+1,name.size()-i);
            break;
        }
    }
    filename=QString::fromStdString(name);

    bytes=name.size();
    socket->write((char*)&bytes,2);
    socket->write(filename.toUtf8());
    socket->flush();

    char buf[1024]={0};
    socket->waitForReadyRead(-1);
    socket->read(buf,1);


    int len;
    while(1){
        memset(buf,0,sizeof (buf));
        len=file.read(buf,sizeof (buf));
        if(len<=0)break;
        socket->write(buf,len);
    }
    file.close();
}

void home::on_fresh_clicked()
{
    map.clear();
    for(QPushButton*btn:buts){
        btn->setVisible(false);
        delete btn;
    }
    buts.clear();

    short bytes=5;
    QString str="fresh";
    socket->write((char*)&bytes,2);
    socket->write(str.toUtf8());
    socket->flush();
    qDebug()<<"flush";

    acceptDirs();
    showFiles();

}
void home::acceptDirs(){
    short bytes=0;
    QString file_name,file_id;
    string name,id;

    socket->waitForReadyRead(-1);
    socket->read((char*)&bytes,2);
    char buf[1024]={0};

    while(bytes!=0){
        memset(buf,0,sizeof(buf));
        while(!socket->bytesAvailable());
        socket->read(buf,bytes);
        int i=0;
        name.clear();
        while(buf[i]!=0)name.push_back(buf[i++]);
        file_name=QString::fromStdString(name);

        while(!socket->bytesAvailable());
        socket->read((char*)&bytes,2);
        memset(buf,0,sizeof(buf));
        while(!socket->bytesAvailable());
        socket->read(buf,bytes);
        i=0;
        id.clear();
        while(buf[i]!=0)id.push_back(buf[i++]);
        file_id=QString::fromStdString(id);

        map[file_name]=file_id;
        while(!socket->bytesAvailable());
        socket->read((char*)&bytes,2);
    }
}
