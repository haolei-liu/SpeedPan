/*************************************************************************
	> File Name: fdfs_upload_file.h
	> Author: 
	> Mail: 
	> Created Time: Thu 15 Oct 2020 12:12:49 AM EDT
 ************************************************************************/

#ifndef _FDFS_UPLOAD_FILE_H
#define _FDFS_UPLOAD_FILE_H
#include<string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "logger.h"
#include "fdfs_client.h"

using namespace std;
string fdfs_upload_file(const char *local_filename);
#endif
