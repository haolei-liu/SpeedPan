#include"MysqlPool.h"

MysqlPool::MysqlPool(int minNum,int maxNum){
    this->minNum=minNum;
    this->maxNum=maxNum;
    this->lock = new pthread_mutex_t;
    this->cond = new pthread_cond_t;
    for(int i=0;i<minNum;i++){
        MYSQL *tem = new MYSQL;
        mysql_init(tem);
        if(mysql_real_connect(tem,Host,User,Passwd,DB,Port,NULL,CLIENT_FOUND_ROWS))
            conns.push_back(tem);
        else cout<<"init connpool error"<<endl;
    }
    this->num=minNum;
    pthread_mutex_init(this->lock,NULL);
    pthread_cond_init(this->cond,NULL);
}
MYSQL* MysqlPool::getConn(){
    pthread_mutex_lock(lock);
    if(conns.size()>0){
        MYSQL *tem=conns[conns.size()-1];
        conns.pop_back();
        pthread_mutex_unlock(lock);
        return tem;
    }else if(conns.size()==0&&num<maxNum){
        MYSQL *tem = new MYSQL;
        mysql_init(tem);
        if(mysql_real_connect(tem,Host,User,Passwd,DB,Port,NULL,CLIENT_FOUND_ROWS)){
            num++;
            pthread_mutex_unlock(lock);
            return tem;
        }
    }else{
        pthread_cond_wait(cond,lock);
        MYSQL *tem=conns[conns.size()-1];
        conns.pop_back();
        pthread_mutex_unlock(lock);
        return tem;
    }
}
void MysqlPool::back(MYSQL*conn){
    pthread_mutex_lock(lock);
    conns.push_back(conn);
    pthread_mutex_unlock(lock);
    pthread_cond_signal(cond);
}

