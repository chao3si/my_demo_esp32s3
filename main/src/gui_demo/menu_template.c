#include "menu_template.h"

//static lv_timer_t *timer;
PAGE_MANAGE_T ui;


static lv_obj_t *left_btn;
static lv_style_t left_btn_style;
static lv_obj_t *right_btn;
static lv_style_t right_btn_style;

static void manage_page(int flag)
{
    switch (ui.page)    //先去除当前page信息
    {
        case TIME_PAGE:
            lv_group_remove_obj(get_time_btn_obj());
            time_page_anim_out(0, flag);
            break;
        case WIFI_PAGE:
            lv_group_remove_obj(get_wifi_btn_obj());
            wifi_page_anim_out(0, flag);
            break;
        case BILIBILI_PAGE:
            lv_group_remove_obj(get_bilibili_btn_obj());
            bilibili_page_anim_out(0, flag);
            break;
        case FLUSH_PAGE:
            lv_group_remove_obj(get_flush_btn_obj());
            flush_page_anim_out(0, flag);
            break;
        default:
            break;
    }   

    if(flag == 1)
    {
        ui.page++;
        if(ui.page > TOTAL_PAGE)
        {
            ui.page = 1;
        }
    }
    else if(flag == 0)
    {
        ui.page--;
        if(ui.page < 1)
        {
            ui.page = TOTAL_PAGE;
        }
    }

    switch (ui.page)
    {
        case TIME_PAGE:
            time_page();
            time_page_anim_in(500, flag);
            break;
        case WIFI_PAGE:
            wifi_page();
            wifi_page_anim_in(500, flag);
            break;
        case BILIBILI_PAGE:
            bilibili_page();
            bilibili_page_anim_in(500, flag);
            break;
        case FLUSH_PAGE:
            flush_page();
            flush_page_anim_in(500, flag);
            break;
        default:
            break;
    }
}

static void left_btn_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *label = lv_event_get_user_data(e);

    switch(code)
    {
        case LV_EVENT_FOCUSED:
            lv_style_set_text_opa(&left_btn_style, LV_OPA_COVER);
            lv_obj_add_style(label, &left_btn_style, LV_STATE_FOCUSED);
            break;
        case LV_EVENT_DEFOCUSED:
            lv_style_set_text_opa(&left_btn_style, LV_OPA_30);
            lv_obj_add_style(label, &left_btn_style, LV_EVENT_DEFOCUSED);
            break;
        case LV_EVENT_CLICKED:
            manage_page(0);
            break;
        default:
            break;
    }
}

static void right_btn_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *label = lv_event_get_user_data(e);

    switch(code)
    {
        case LV_EVENT_FOCUSED:
            lv_style_set_text_opa(&right_btn_style, LV_OPA_COVER);
            lv_obj_add_style(label, &right_btn_style, LV_STATE_FOCUSED);
            break;
        case LV_EVENT_DEFOCUSED:
            lv_style_set_text_opa(&right_btn_style, LV_OPA_30);
            lv_obj_add_style(label, &right_btn_style, LV_EVENT_DEFOCUSED);
            break;
        case LV_EVENT_CLICKED:
            manage_page(1);
            break;
        default:
            break;
    }
}

PAGE_MANAGE_T get_now_show_page()
{
    return ui;
}

void menu_page_remove(uint32_t delay)
{
    lv_group_remove_obj(left_btn);
    lv_group_remove_obj(right_btn);

    obj_set_anim(left_btn, 5, -120, 100, delay, anim_set_x_cb, lv_obj_del_anim_ready_cb, lv_anim_path_ease_out);
    obj_set_anim(right_btn, 205, 330, 100, delay, anim_set_x_cb, lv_obj_del_anim_ready_cb, lv_anim_path_ease_out);
    switch (ui.page) 
    {
        case TIME_PAGE:
            lv_group_remove_obj(get_time_btn_obj());
            time_page_anim_out(0, 1);
            break;
        case WIFI_PAGE:
            lv_group_remove_obj(get_wifi_btn_obj());
            wifi_page_anim_out(0, 1);
            break;
        case BILIBILI_PAGE:
            lv_group_remove_obj(get_bilibili_btn_obj());
            bilibili_page_anim_out(0, 1);
            break;
        case FLUSH_PAGE:
            lv_group_remove_obj(get_flush_btn_obj());
            flush_page_anim_out(0, 1);
            break;
        default:
            break;
    }   
}

void back_to_menu_page(int flag)
{
    switch (ui.page)
    {
        case TIME_PAGE:
            time_page();
            time_page_anim_in(100, flag);
            break;
        case WIFI_PAGE:
            wifi_page();
            wifi_page_anim_in(100, flag);
            break;
        case BILIBILI_PAGE:
            bilibili_page();
            bilibili_page_anim_in(100, flag);
            break;
        case FLUSH_PAGE:
            break;
        default:
            break;
    }
    menu_page();
    ui.status = 0;  //退出app页标志
}

void menu_page(void)
{
    lv_group_t *group = get_group();
	
	/*左边按钮*/
	left_btn = lv_btn_create(bg_screen);
    lv_obj_remove_style_all(left_btn);
    lv_obj_set_size(left_btn, 30, 30);
    lv_obj_set_style_radius(left_btn, 15, 0);
    lv_obj_set_style_bg_color(left_btn, lv_color_white(), LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(left_btn, LV_OPA_30, LV_STATE_PRESSED);
    lv_obj_set_pos(left_btn, 5, 105);

    lv_style_init(&left_btn_style);
    lv_style_set_text_color(&left_btn_style, lv_color_white());
    lv_style_set_text_opa(&left_btn_style, LV_OPA_50);
    lv_style_set_text_font(&left_btn_style, &lv_font_montserrat_20);

    lv_obj_t *left_icon_label = lv_label_create(left_btn);
    lv_label_set_text(left_icon_label, LV_SYMBOL_LEFT);
    lv_obj_add_style(left_icon_label, &left_btn_style, 0);
    lv_obj_add_event_cb(left_btn, left_btn_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(left_btn, left_btn_cb, LV_EVENT_FOCUSED, left_icon_label);
    lv_obj_add_event_cb(left_btn, left_btn_cb, LV_EVENT_DEFOCUSED, left_icon_label);
    lv_obj_align(left_icon_label, LV_ALIGN_CENTER, 0, 0);
	
	/*右边按钮*/
	right_btn = lv_btn_create(bg_screen);
    lv_obj_remove_style_all(right_btn);
    lv_obj_set_size(right_btn, 30, 30);
    lv_obj_set_style_radius(right_btn, 15, 0);
    lv_obj_set_style_bg_color(right_btn, lv_color_white(), LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(right_btn, LV_OPA_30, LV_STATE_PRESSED);
    lv_obj_set_pos(right_btn, 205, 105);

    lv_style_init(&right_btn_style);
    lv_style_set_text_color(&right_btn_style, lv_color_white());
    lv_style_set_text_opa(&right_btn_style, LV_OPA_50);
    lv_style_set_text_font(&right_btn_style, &lv_font_montserrat_20);

    lv_obj_t *right_icon_label = lv_label_create(right_btn);
    lv_label_set_text(right_icon_label, LV_SYMBOL_RIGHT);
    lv_obj_add_style(right_icon_label, &right_btn_style, 0);
    lv_obj_add_event_cb(right_btn, right_btn_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(right_btn, right_btn_cb, LV_EVENT_FOCUSED, right_icon_label);
    lv_obj_add_event_cb(right_btn, right_btn_cb, LV_EVENT_DEFOCUSED, right_icon_label);
    lv_obj_align(right_icon_label, LV_ALIGN_CENTER, 0, 0);

    lv_group_add_obj(group, left_btn);
    lv_group_add_obj(group, right_btn);
}