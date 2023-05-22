#ifndef __TIME_APP_H
#define __TIME_APP_H

#include "chaosi_system_data.h"
#include "menu_template.h"

typedef struct {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
} TIME_PAGE_T;


void time_page_update();
void time_app_page();


#endif
