#include "flush_page.h"

static lv_obj_t * flush_btn;
static lv_obj_t * label;

static void flush_btn_cb(lv_event_t *e)
{
    set_has_http_requst(true);
}

lv_obj_t* get_flush_btn_obj()
{
    return flush_btn;
}

void flush_page_anim_in(uint32_t delay, int flag)
{
    if(flag == 1)
    {
        obj_set_anim(flush_btn, -260, 90, 500, delay, anim_set_x_cb, NULL, lv_anim_path_bounce);
    }
    else if(flag == 0)
    {
        obj_set_anim(flush_btn, 440, 90, 500, delay, anim_set_x_cb, NULL, lv_anim_path_bounce);
    }
}

void flush_page_anim_out(uint32_t delay, int flag)
{
    if(flag == 0)
    {
        obj_set_anim(flush_btn, 90, -260, 500, delay, anim_set_x_cb, lv_obj_del_anim_ready_cb, lv_anim_path_ease_out);
    }
    else if(flag == 1)
    {
        obj_set_anim(flush_btn, 90, 440, 500, delay, anim_set_x_cb, lv_obj_del_anim_ready_cb, lv_anim_path_ease_out);
    }
}

void flush_page()
{
    
    lv_group_t *group = get_group();

    flush_btn = lv_btn_create(bg_screen);
    lv_obj_add_event_cb(flush_btn, flush_btn_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_set_size(flush_btn, 60, 30);
    lv_obj_set_pos(flush_btn, 90, 105);


    label = lv_label_create(flush_btn);
    lv_label_set_text(label, "flush!");
    lv_obj_center(label);

    lv_group_add_obj(group, flush_btn);
}
