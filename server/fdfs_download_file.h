#ifndef FDFS_DOWNLOAD_FILE_H
#define FDFS_DOWNLOAD_FILE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "fdfs_client.h"
#include<stdio.h>
#include<iostream>
#include "fastcommon/logger.h"

using namespace std;
int fdfs_download_file(string fileid,char*local);


#endif
