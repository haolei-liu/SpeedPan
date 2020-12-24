#ifndef _MYSQL_POOL_H
#define _MYSQL_POOL_H
#include<mysql/mysql.h>
#include<stdio.h>
#include<iostream>
#include<vector>
#include<pthread.h>
using namespace std;

#define Host  "210.45.251.160"
#define User "liuhaolei"
#define Passwd "liuhaolei"
#define DB "test"
#define Port 3306

class MysqlPool{
public:
    MysqlPool(int,int);
    int num;
    int minNum;
    int maxNum;
    pthread_mutex_t *lock;
    pthread_cond_t *cond;
    vector<MYSQL *>conns;
    MYSQL *getConn();
    void back(MYSQL*);
};
#endif
