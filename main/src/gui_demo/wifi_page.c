#include "wifi_page.h"

static lv_obj_t * wifi_btn;

static void wifi_btn_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *label = lv_event_get_user_data(e);

    switch(code)
    {
        case LV_EVENT_CLICKED:
            menu_page_remove(0);
            wifi_app_page();
            break;
        default:
            break;
    }
}

lv_obj_t* get_wifi_btn_obj()
{
    return wifi_btn;
}

void wifi_page_anim_in(uint32_t delay, int flag)
{
    if(flag == 1)
    {
        obj_set_anim(wifi_btn, -260, 90, 500, delay, anim_set_x_cb, NULL, lv_anim_path_bounce);
    }
    else if(flag == 0)
    {
        obj_set_anim(wifi_btn, 440, 90, 500, delay, anim_set_x_cb, NULL, lv_anim_path_bounce);
    }
}

void wifi_page_anim_out(uint32_t delay, int flag)
{
    if(flag == 0)
    {
        obj_set_anim(wifi_btn, 90, -260, 500, delay, anim_set_x_cb, lv_obj_del_anim_ready_cb, lv_anim_path_ease_out);
    }
    else if(flag == 1)
    {
        obj_set_anim(wifi_btn, 90, 440, 500, delay, anim_set_x_cb, lv_obj_del_anim_ready_cb, lv_anim_path_ease_out);
    }
}

void wifi_page()
{
    lv_group_t *group = get_group();
    
    /*中间部件*/
    lv_obj_t * label;

    wifi_btn = lv_btn_create(bg_screen);
    lv_obj_add_event_cb(wifi_btn, wifi_btn_cb, LV_EVENT_ALL, NULL);
    lv_obj_set_size(wifi_btn, 60, 30);
    lv_obj_set_pos(wifi_btn, 90, 105);


    label = lv_label_create(wifi_btn);
    lv_label_set_text(label, "wifi!");
    lv_obj_center(label);

    lv_group_add_obj(group, wifi_btn);

}