#ifndef HOME_H
#define HOME_H

#include <QMainWindow>
#include<QFile>
#include<QFileDialog>
#include<QProcess>
#include<QTcpSocket>
#include<QHostAddress>
#include<QDataStream>
#include<string.h>
#include<qmap.h>
#include<QPushButton>
#include<qvector.h>
using namespace std;

namespace Ui {
class home;
}

class home : public QMainWindow
{
    Q_OBJECT

public:
    explicit home(QWidget *parent = nullptr);
    ~home();
    QTcpSocket *socket;
    QMap<QString,QString>map;
    QVector<QPushButton*>buts;
    void showFiles();
    void download(QString file_name);
    void acceptDirs();
private slots:

    void on_upload_clicked();

    void on_fresh_clicked();

private:
Ui::home *ui;

};

#endif // HOME_H
