/*************************************************************************
	> File Name: thread_pool.cpp
	> Author:
	> Mail:
	> Created Time: Thu 19 Nov 2020 03:19:04 AM EST
 ************************************************************************/

#include<iostream>
#include"thread_pool.h"

using namespace std;

job::job(int cfd,int epoll,struct epoll_event *all,int index,MysqlPool *mysql_pool) {
    this->mysql_pool = mysql_pool;
    this->_connect_fd=cfd;
    this->_epoll_fd=epoll;
    this->pre=NULL;
    this->next=NULL;
    this->all=all;
    this->index=index;
}

void job::execute_Task()
{
    while(1){
            /* 读数据 */
        /* 初始化缓冲区，用来接收客户端发送的数据 */
        /* 处理客户端发来的请求 */
        int readData_len;
        int total=0;
        short num=0;

        char in_buf[1024] = {0};
        total=0;
        char *p = (char*)&num;
        while(total<2) {
            readData_len=recv(_connect_fd,p+total,1,0);
            total+=readData_len;
            if(readData_len<=0){
                epoll_ctl(_epoll_fd,EPOLL_CTL_DEL,_connect_fd,&all[index]);
                close(_connect_fd);
                return;
            }
        }

        //read num  bytes data
        total=0;
        read(_connect_fd,in_buf,num);
        //logical  process
        string str;
        int i=0;
        while(in_buf[i]!=0)str.push_back(in_buf[i++]);
      
        if(str=="login"){
            if(login()){
                cout<<"succ"<<endl;
                num=7;
                send(_connect_fd,(char*)&num,2,0);
                char suc[1024]={'s','u','c','c','e','s','s'};
                write(_connect_fd,suc,7);
                sendDirs();
            }
            else {
                num=6;
                send(_connect_fd,(char*)&num,2,0);
                char fal[1024]={'f','a','i','l','e','d'};
                write(_connect_fd,fal,6);
            }
        }else if(str=="regist"){
            if(regist()){
                num=7;
                send(_connect_fd,(char*)&num,2,0);
                char suc[1024]={'s','u','c','c','e','s','s'};
                write(_connect_fd,suc,7);
            }
            else{
                num=6;
                send(_connect_fd,(char*)&num,2,0);
                char fal[1024]={'f','a','i','l','e','d'};
                write(_connect_fd,fal,6);
            }
        }else if(str=="upload"){
            if(upload()){
                num=7;
                send(_connect_fd,(char*)&num,2,0);
                char suc[1024]={'s','u','c','c','e','s','s'};
                write(_connect_fd,suc,7);
            }else{
                num=6;
                send(_connect_fd,(char*)&num,2,0);
                char fal[1024]={'f','a','i','l','e','d'};
                write(_connect_fd,fal,6);
            }
        }else if(str=="download"){
             if(download()){
                 num=7;
                send(_connect_fd,(char*)&num,2,0);
                char suc[1024]={'s','u','c','c','e','s','s'};
                write(_connect_fd,suc,7);
            }else{
                num=6;
                send(_connect_fd,(char*)&num,2,0);
                char fal[1024]={'f','a','i','l','e','d'};
                write(_connect_fd,fal,6);
            }
        }else if(str=="fresh"){
            cout<<"fresh"<<endl;
            if(sendDirs()){
                num=7;
                send(_connect_fd,(char*)&num,2,0);
                char suc[1024]={'s','u','c','c','e','s','s'};
                write(_connect_fd,suc,7);
                cout<<"over"<<endl;
            }else{
                num=6;
                send(_connect_fd,(char*)&num,2,0);
                char fal[1024]={'f','a','i','l','e','d'};
                write(_connect_fd,fal,6);
            }
        }
    }
}

bool job::login(){
    
    //receive userid
    char in_buf[1024] = {0};
    int total=0;
    int readData_len;
    short num=0;
    char *p = (char*)&num;
    while(total<2) {
        readData_len=recv(_connect_fd,p+total,1,0);
        total+=readData_len;
    }
    //read num  bytes data
    total=0;
    read(_connect_fd,in_buf,num);
    //logical  process
    string id;
    int i=0;
    while(in_buf[i]!=0)id.push_back(in_buf[i++]);
    //receive  password
    while(total<2) {
        readData_len=recv(_connect_fd,p+total,1,0);
        total+=readData_len;
    }

    //read num  bytes data
    memset(in_buf,0,sizeof(in_buf));
    total=0;
    read(_connect_fd,in_buf,num);
    //logical  process
    string pwd;
    i=0;
    while(in_buf[i]!=0)pwd.push_back(in_buf[i++]);
    MYSQL *conn=mysql_pool->getConn();
   
        string sql="select password from user where id='";
        sql+=id;
        char query[1024]={0};
        sql+="';";
        
        for(int i=0;i<sql.size();i++)query[i]=sql[i];
        
        if(mysql_query(conn, query)==0){
            MYSQL_RES* res=mysql_store_result(conn);
            MYSQL_ROW row;
            while(row=mysql_fetch_row(res)){
                if(row[0]==pwd){
                    this->userid=id;
                    mysql_close(conn);
                    return true;
                }
            }
            mysql_pool->back(conn);
            return false;
        }else{
            cout<<"query table failed"<<endl;
        }
    return false;
}

bool job::sendDirs(){
    cout<<"sending"<<endl;
        string sql="select file_name,file_id from user_file_list where user='";
        sql+=userid;
        sql+="';";
        char query[1024]={0};
        char tt[1024]={0};
        for(int i=0;i<sql.size();i++)query[i]=sql[i];
        MYSQL *conn=mysql_pool->getConn();
        if(mysql_query(conn, query)==0){
            MYSQL_RES* res=mysql_store_result(conn);
            mysql_pool->back(conn);
            MYSQL_ROW row;
            
            string file_name,file_id;
            char buf[1024]={0};
            short num;
            while(row=mysql_fetch_row(res)){
                file_name=row[0];file_id=row[1];
                num=file_name.size();
                
                memset(buf,0,sizeof(buf));
                for(int i=0;i<num;i++)buf[i]=file_name[i];
                send(_connect_fd,(char*)&num,2,0);
                write(_connect_fd,buf,num);
                
                num=file_id.size();
                memset(buf,0,sizeof(buf));
                for(int i=0;i<num;i++)buf[i]=file_id[i];
                send(_connect_fd,(char*)&num,2,0);
                write(_connect_fd,buf,num);
            }
            num=0;
            send(_connect_fd,(char*)&num,2,0);
            return true;
        }else{
            mysql_pool->back(conn);
            return false;
        }
}



bool job::regist(){
        
    //receive userid
    
    char in_buf[1024] = {0};
    int total=0;
    int readData_len;
    short num=0;
    char *p = (char*)&num;
    while(total<2) {
        readData_len=recv(_connect_fd,p+total,1,0);
        total+=readData_len;
    }

    //read num  bytes data
    total=0;
    read(_connect_fd,in_buf,num);
    //logical  process
    string id;
    int i=0;
    while(in_buf[i]!=0)id.push_back(in_buf[i++]);
    
    
    //receive  password
    while(total<2) {
        readData_len=recv(_connect_fd,p+total,1,0);
        total+=readData_len;
    }

    //read num  bytes data
    total=0;
    memset(in_buf,0,sizeof(in_buf));
    read(_connect_fd,in_buf,num);
    //logical  process
    string pwd;
    i=0;
    while(in_buf[i]!=0)pwd.push_back(in_buf[i++]);
    MYSQL* conn = mysql_pool->getConn();
        string sql="insert into user (id,password) value('";
        sql+=id;
        sql+="','";
        sql+=pwd;
        sql+="');";
        char query[1024]={0};
        
        for(int i=0;i<sql.size();i++)query[i]=sql[i];
        
        if(mysql_query(conn, query)!=0){
            cout<<"insert user table failed"<<endl;
            mysql_close(conn);
            return false;
        }
        mysql_pool->back(conn);
    return true;
}

bool job::upload(){
    char buf[1024]={0};
    short num;
    int total=0;
    int readData_len;
    char *p = (char*)&num;
    while(total<2){
        readData_len=recv(_connect_fd,p+total,1,0);
        total+=readData_len;
    }
    
    total=0;
    read(_connect_fd,buf,num);
    
    
    string filename;
    int i=0;
    while(buf[i]!=0)filename.push_back(buf[i++]);
    
    
    cout<<"upload  "<<filename<<endl;
    if(filename.size()==0)return false;

    string name="/home/liuhaolei/Desktop/";
    
    
    char nn[1024]={0};
    name+=filename;
    memset(nn,0,sizeof(nn));
    for(int i=0;i<name.size();i++)nn[i]=name[i];
    
    
    int ff=open(nn,O_CREAT|O_TRUNC|O_RDWR);
    close(ff);
    chmod(nn,0777);
    ff=open(nn,O_APPEND|O_RDWR);
    
    
    //ready to read
    write(_connect_fd,buf,1);
    while(1){
        memset(buf,0,sizeof(buf));
        readData_len=read(_connect_fd,buf,sizeof(buf));
        fcntl(_connect_fd,F_SETFL,O_NONBLOCK);
        if(readData_len<=0)break;
        write(ff,buf,readData_len);
    }
    fcntl(_connect_fd,F_SETFL,0);
    close(ff);
    string file_id=fdfs_upload_file(nn);
    if(file_id.size()==0)return false;
    MYSQL *conn = mysql_pool->getConn();
        string sql="insert into user_file_list (user,file_name ,file_id) value('";
        sql+=this->userid;
        sql+="','";
        sql+=filename;
        sql+="','";
        sql+=file_id;
        sql+="');";
        char query[1024]={0};
        for(int i=0;i<sql.size();i++)query[i]=sql[i];
        if(mysql_query(conn, query)!=0){
            cout<<"insert user_file_list table failed"<<endl;
            mysql_close(conn);
            return false;
        }
        mysql_pool->back(conn);
    return true;
}

bool job::download(){

     //receive file_id
    
    char in_buf[1024] = {0};
    int total=0;
    int readData_len;
    short num=0;
    char *p = (char*)&num;
    while(total<2) {
        readData_len=recv(_connect_fd,p+total,1,0);
        total+=readData_len;
    }
    //read num  bytes data
    total=0;
    read(_connect_fd,in_buf,num);
    string fileid;
    int i=0;
    while(in_buf[i]!=0)fileid.push_back(in_buf[i++]);
    string local="/home/liuhaolei/Desktop/fdfs_files/aa";
    char local_name[1024]={0};
    for(int i=0;i<local.size();i++)local_name[i]=local[i];
    
    int ff=open(local_name,O_CREAT|O_TRUNC|O_RDWR);
    chmod(local_name,0777);
    close(ff);
    fdfs_download_file(fileid,local_name);
    ff=open(local_name,O_RDONLY);
    while(1){
        memset(in_buf,0,sizeof(in_buf));
        readData_len=read(ff,in_buf,sizeof(in_buf));
        if(readData_len<=0)break;
        write(_connect_fd,in_buf,readData_len);
    }
    close(ff);
    return true;
}


worker::worker() {
    this->next=NULL;
    this->pre=NULL;
}
void* work(void*arg) {
    thread_pool *pool = (thread_pool*)arg;
    while(1) {
        sem_wait(&pool->sem);
        if(pool->jobs==NULL)continue;
        job*cur;
        pthread_mutex_lock(&pool->mutex);
        pool->job_num--;
        cur=pool->jobs;
        pool->jobs = pool->jobs->next;
        pthread_mutex_unlock(&pool->mutex);
        if(cur==NULL)return NULL;
        cur->execute_Task();
        if(cur!=NULL){
            delete cur;
            cur=NULL;
        }
    }
}
thread_pool::thread_pool(int thread_nums) {

    job_num=0;
    workers = new worker();
    jobs=NULL;
    pthread_create(&workers->id,NULL,work,this);
    pthread_mutex_init(&mutex,NULL);
    pthread_mutex_init(&job_add_mutex,NULL);
    if(sem_init(&sem,0,0)!=0)perror("sem_init:");
    for(int i=1; i<thread_nums; i++) {
        worker* tem = new worker();
        pthread_create(&tem->id,NULL,work,this);
        workers->pre=tem;
        tem->next=workers;
        workers=tem;
    }
    cout<<"thread_pool  init "<<endl;
}
void thread_pool::addjob(job*tem) {
    pthread_mutex_lock(&this->job_add_mutex);
    if(jobs==NULL) {
        jobs=tem;
    }
    else {
        tem->next = jobs;
        jobs->pre=tem;
        jobs = tem;
        job_num++;
    }
    sem_post(&this->sem);
    pthread_mutex_unlock(&this->job_add_mutex);
}
