#ifndef __MYFILESYSTEM_H
#define __MYFILESYSTEM_H

#include "esp_err.h"

esp_err_t MyfileSystem_mount_storage(const char* base_path);
void MyfileSystem_DeInit(void);
void MyfileSystem_Test(void);
esp_err_t MyfileSystem_remove(const char *target);


#endif
