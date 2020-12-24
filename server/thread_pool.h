/*************************************************************************
	> File Name: thread_pool.h
	> Author:
	> Mail:
	> Created Time: Thu 19 Nov 2020 05:01:02 AM EST
 ************************************************************************/

#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H
#include<semaphore.h>
#include<pthread.h>
#include <sys/epoll.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<errno.h>
#include<cstring>
#include <sys/sendfile.h>
#include<string>
#include<mysql/mysql.h>
#include<fstream>
#include<cstddef>
#include<netinet/tcp.h>
#include<set>
#include"fdfs_upload_file.h"
#include"fdfs_download_file.h"
#include"MysqlPool.h"

class job {
public:
    job*next;
    job*pre;
    job(int,int,struct epoll_event *all,int,MysqlPool*);
    struct epoll_event *all;
    string userid;
    void execute_Task();
    
    
private:
    int _connect_fd;
    int _epoll_fd;
    int index;
    bool login();
    bool regist();
    bool upload();
    bool download();
    bool sendDirs();
    bool sendDirs2();
    MysqlPool *mysql_pool;
    
    
};
class worker {
public:
    pthread_t id;
    worker*next;
    worker* pre;
    worker();

};
class thread_pool {
public:
    worker*workers;
    job*jobs;
    int job_num;
    pthread_mutex_t mutex,job_add_mutex;
    thread_pool(int);
    void addjob(job*);
    sem_t sem;
};

#endif
