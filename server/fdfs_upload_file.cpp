/**
* Copyright (C) 2008 Happy Fish / YuQing
*
* FastDFS may be copied only under the terms of the GNU General
* Public License V3, which may be found in the FastDFS source kit.
* Please visit the FastDFS Home Page http://www.fastken.com/ for more detail.
**/
#include"fdfs_upload_file.h"
string fdfs_upload_file(const char * local_filename)
{
    char *conf_filename="/etc/fdfs/client.conf";
	char group_name[FDFS_GROUP_NAME_MAX_LEN + 1];
	ConnectionInfo *pTrackerServer;
	int result;
	int store_path_index;
	ConnectionInfo storageServer;
	char file_id[128];

	log_init();
	g_log_context.log_level = LOG_ERR;
	ignore_signal_pipe();

	if ((result=fdfs_client_init(conf_filename)) != 0)
	{
		return NULL;
	}

	pTrackerServer = tracker_get_connection();
	if (pTrackerServer == NULL)
	{
		fdfs_client_destroy();
		return NULL;
	}

	*group_name = '\0';

	if ((result=tracker_query_storage_store(pTrackerServer, \
	                &storageServer, group_name, &store_path_index)) != 0)
	{
		fdfs_client_destroy();
		fprintf(stderr, "tracker_query_storage fail, " \
			"error no: %d, error info: %s\n", \
            result, STRERROR(result));
		return NULL;
	}

	result = storage_upload_by_filename1(pTrackerServer, \
			&storageServer, store_path_index, \
			local_filename, NULL, \
			NULL, 0, group_name, file_id);
	if (result != 0)
	{
		fprintf(stderr, "upload file fail, " \
			"error no: %d, error info: %s\n", \
			result, STRERROR(result));
	}

	tracker_close_connection_ex(pTrackerServer, true);
	fdfs_client_destroy();
    string res;
    int i=0;
    while(file_id[i]!=0)res.push_back(file_id[i++]);
	return res;
}

