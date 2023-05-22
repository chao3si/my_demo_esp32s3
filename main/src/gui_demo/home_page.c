#include "home_page.h"

static lv_obj_t *home_btn;

static void home_page_anim_out(uint32_t delay)
{
    obj_set_anim(home_btn, 90, -200, 500, delay, anim_set_x_cb, lv_obj_del_anim_ready_cb, lv_anim_path_ease_out);
}

static void home_btn_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    //lv_obj_t *label = lv_event_get_user_data(e);

    switch(code)
    {
        case LV_EVENT_CLICKED:
            ui.page = TIME_PAGE;
            ui.status = 0;
            lv_group_remove_obj(home_btn);
            home_page_anim_out(0);
            menu_page();
            time_page();
            break;
        default:
            break;
    }
}

void home_page()
{
    lv_group_t *group = get_group();
    /*中间部件*/
    lv_obj_t * label;

    home_btn = lv_btn_create(bg_screen);
    lv_obj_add_event_cb(home_btn, home_btn_cb, LV_EVENT_ALL, NULL);
    lv_obj_set_size(home_btn, 60, 30);
    lv_obj_set_pos(home_btn, 90, 105);
    
    label = lv_label_create(home_btn);
    lv_label_set_text(label, "Hello!");
    lv_obj_center(label);

    lv_group_add_obj(group, home_btn);
}