/*************************************************************************
	> File Name: epoll.cpp
	> Author:
	> Mail:
	> Created Time: Tue 06 Oct 2020 09:50:13 AM EDT
 ************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#include<errno.h>
#include<netinet/tcp.h>
#include"thread_pool.h"
#define port 8888

using namespace std;


int main()
{
    
    string base="/home/liuhaolei/Desktop";
    char nn[1024]={0};
    for(int i=0;i<base.size();i++)nn[i]=base[i];
    chmod(nn,0777);
    
    int lfd=socket ( AF_INET,SOCK_STREAM,0 );
    struct sockaddr_in server;
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=htonl ( INADDR_ANY );
    server.sin_port=htons ( port );

    int flag=1;
    setsockopt ( lfd,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof ( flag ) );
    bind ( lfd, ( sockaddr* ) &server,sizeof ( server ) );
    listen ( lfd,128 );

    int root=epoll_create ( 2000 );
    if ( root==-1 ) perror ( "epoll error:" );
    //初始化epoll树
    struct epoll_event ll;
    ll.events=EPOLLIN|EPOLLET;

    ll.data.fd=lfd;
    epoll_ctl ( root,EPOLL_CTL_ADD,lfd,&ll );
    //初始化数组
    struct epoll_event all[2000];

    sockaddr_in client;
    socklen_t len=sizeof ( client );

    thread_pool *pool = new thread_pool ( 100 );
    MysqlPool *mysql_pool = new MysqlPool(10,20);

    cout<<"初始化完成"<<endl;
    while ( 1 ) {
        int num= epoll_wait ( root,all,sizeof ( all ) /sizeof ( all[0] ),-1 );
        if ( num==-1 ) {
            perror ( "epoll_wait error:" );
            break;
        }
        for ( int i=0; i<num; i++ ) {
            if ( all[i].data.fd==lfd ) {
                int c_fd= accept ( lfd, ( sockaddr* ) &client,&len );
                if ( c_fd==-1 ) perror ( "accept error" );
                //no delay
                //setsockopt(c_fd,SOL_SOCKET,SO_SNDBUF,(char*)&flag,sizeof(flag));
                //setsockopt(c_fd,SOL_SOCKET,SO_KEEPALIVE,(char*)&flag,sizeof(flag));
                //int res=setsockopt(c_fd,IPPROTO_TCP,TCP_NODELAY,&flag,sizeof(flag));
                //if(res==-1)cout<<"set nodelay failed"<<endl;
                //else cout<<"set nodelay successed"<<endl;
                
                struct epoll_event c;
                c.events=EPOLLIN|EPOLLET;
                c.data.fd=c_fd;
                epoll_ctl ( root,EPOLL_CTL_ADD,c_fd,&c );
                char buf[64]= {0};
                inet_ntop ( AF_INET,&client.sin_addr.s_addr,buf,sizeof ( buf ));
                cout<<"ip:"<<buf<<"  port:"<<ntohs ( client.sin_port ) <<"已连接"<<endl;
            } else {
                job *j = new job (all[i].data.fd,root,all,i,mysql_pool);
                pool->addjob(j);
            }
        }
    }
    close ( lfd );
    delete pool;
    return 0;
}
