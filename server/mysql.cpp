/*************************************************************************
	> File Name: mysql.cpp
	> Author: 
	> Mail: 
	> Created Time: Sat 14 Nov 2020 04:03:33 AM EST
 ************************************************************************/

#include<iostream>
#include<mysql.h>
using namespace std;
#define Host  "210.45.251.106"
#define User "liuhaolei"
#define Passwd "liuhaolei"
#define DB "test"
#define Port 3306
int main(){
    MYSQL conn;
    mysql_init(&conn);
    if(mysql_real_connect(&conn,Host,User,Passwd,DB,Port,NULL,CLIENT_FOUND_ROWS)){
        cout<<"connect mysql successed"<<endl;
        if(mysql_query(&conn,"create table user(name varchar(64),id int); ")==0){
            cout<<"create table successed"<<endl;
        }else{
            cout<<"create table failed"<<endl;
        }
    }else{
        cout<<"connect mysql failed"<<endl;
    }
    return 0;
}

MYSQL conn;
    mysql_init(&conn);
    if(mysql_real_connect(&conn,Host,User,Passwd,DB,Port,NULL,CLIENT_FOUND_ROWS)){
        cout<<"connect mysql successed"<<endl;
        char sql[1024]={0};
        sprintf(sql,"select password from user where id=%s;",pwd);
        if(mysql_query(&conn, ")==0){
            cout<<"create table successed"<<endl;
        }else{
            cout<<"create table failed"<<endl;
        }
    }else{
        cout<<"connect mysql failed"<<endl;
    }

