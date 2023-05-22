#ifndef __CHAOSI_HTTP_SERVER_H
#define __CHAOSI_HTTP_SERVER_H

#include "esp_vfs.h"

/* Max length a file path can have on storage */
#define FILE_PATH_MAX                       30

/* Max size of an individual file. Make sure this
 * value is same as that set in upload_script.html */
#define MAX_FILE_SIZE                       (200*1024) // 200 KB
#define MAX_FILE_SIZE_STR                   "200KB"

/* Scratch buffer size */
#define SCRATCH_BUFSIZE                     8192

struct file_server_data {
    /* Base path of file storage */
    char base_path[ESP_VFS_PATH_MAX + 1];

    /* Scratch buffer for temporary storage during file transfer */
    char scratch[SCRATCH_BUFSIZE];
};


void http_server_start();


#endif