#include "time_app.h"

TIME_PAGE_T time_page_;

static lv_obj_t *time_screen_cont_1;
static lv_obj_t *time_label;
static lv_obj_t *time_screen_cont_2;
static lv_obj_t *temperature_label;
static lv_obj_t *weather_label;
static lv_obj_t *city_label;
static lv_obj_t *time_return_btn;
static lv_style_t time_return_btn_style;

void time_page_update()
{
   if(get_now_show_page().page == TIME_PAGE && get_now_show_page().status == 1)
    { 
        time_page_.second = get_system_data().second;
        time_page_.minute = get_system_data().minute;
        time_page_.hour = get_system_data().hour;
    
        lv_label_set_text_fmt(time_label, "%d:%d:%d", time_page_.hour, time_page_.minute, time_page_.second);
    }
}

static void time_return_btn_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *label = lv_event_get_user_data(e);

    switch(code)
    {
        case LV_EVENT_FOCUSED:
            lv_style_set_text_opa(&time_return_btn_style, LV_OPA_COVER);
            lv_obj_add_style(label, &time_return_btn_style, LV_STATE_FOCUSED);
            break;
        case LV_EVENT_DEFOCUSED:
            lv_style_set_text_opa(&time_return_btn_style, LV_OPA_30);
            lv_obj_add_style(label, &time_return_btn_style, LV_EVENT_DEFOCUSED);
            break;
        case LV_EVENT_CLICKED:
            lv_group_remove_obj(time_return_btn);
            lv_obj_del(time_return_btn);
            lv_obj_del(time_screen_cont_1);
            lv_obj_del(time_screen_cont_2);
            back_to_menu_page(1);
            break;
        default:
            break;
    }
}

void time_app_page()
{
	lv_group_t *group = get_group();
    
    memset(&time_page_, 0, sizeof(TIME_PAGE_T));
    
    time_return_btn = lv_btn_create(bg_screen);
    lv_obj_remove_style_all(time_return_btn);
    lv_obj_set_size(time_return_btn, 20, 20);
    lv_obj_set_style_radius(time_return_btn, 10, 0);
    lv_obj_set_style_bg_color(time_return_btn, lv_color_white(), LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(time_return_btn, LV_OPA_30, LV_STATE_PRESSED);
    lv_obj_set_pos(time_return_btn, 5, 5);

    lv_style_init(&time_return_btn_style);
    lv_style_set_text_color(&time_return_btn_style, lv_color_white());
    lv_style_set_text_opa(&time_return_btn_style, LV_OPA_50);
    lv_style_set_text_font(&time_return_btn_style, &lv_font_montserrat_20);

    lv_obj_t *left_icon_label = lv_label_create(time_return_btn);
    lv_label_set_text(left_icon_label, LV_SYMBOL_LEFT);
    lv_obj_add_style(left_icon_label, &time_return_btn_style, 0);
    lv_obj_add_event_cb(time_return_btn, time_return_btn_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(time_return_btn, time_return_btn_cb, LV_EVENT_FOCUSED, left_icon_label);
    lv_obj_add_event_cb(time_return_btn, time_return_btn_cb, LV_EVENT_DEFOCUSED, left_icon_label);
    lv_obj_align(left_icon_label, LV_ALIGN_CENTER, 0, 0);

    lv_group_add_obj(group, time_return_btn);

    
    time_screen_cont_1 = lv_obj_create(bg_screen);
    lv_obj_remove_style_all(time_screen_cont_1);
    lv_obj_set_size(time_screen_cont_1, 180, 60); 
    lv_obj_set_style_radius(time_screen_cont_1, 10, 0);
    lv_obj_set_style_bg_opa(time_screen_cont_1, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(time_screen_cont_1, lv_color_hex(MY_COLOR_DODGER_BLUE), 0);
	lv_obj_align_to(time_screen_cont_1, bg_screen, LV_ALIGN_TOP_LEFT, 30, 20);


  
    time_label = lv_label_create(time_screen_cont_1);
    lv_label_set_text(time_label, "This is a demo");
    lv_obj_set_style_text_color(time_label, lv_color_white(), 0);
    lv_obj_set_style_text_opa(time_label, LV_OPA_COVER, 0);
    lv_obj_set_style_text_font(time_label, &lv_font_montserrat_20, 0);
    lv_obj_align_to(time_label, time_screen_cont_1, LV_ALIGN_CENTER, 0, 0);


	time_screen_cont_2 = lv_obj_create(bg_screen);
    lv_obj_remove_style_all(time_screen_cont_2);
    lv_obj_set_size(time_screen_cont_2, 180, 130); 
    lv_obj_set_pos(time_screen_cont_2, 30, 90);
    lv_obj_set_style_radius(time_screen_cont_2, 10, 0);
    lv_obj_set_style_bg_opa(time_screen_cont_2, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(time_screen_cont_2, lv_color_hex(MY_COLOR_DODGER_BLUE), 0);


    
    temperature_label = lv_label_create(time_screen_cont_2);
    lv_label_set_text(temperature_label, "WLAN1111111");
    lv_obj_set_style_text_color(temperature_label, lv_color_white(), 0);
    lv_obj_set_style_text_opa(temperature_label, LV_OPA_COVER, 0);
    lv_obj_set_style_text_font(temperature_label, &lv_font_montserrat_20, 0);
    lv_obj_align_to(temperature_label, time_screen_cont_2, LV_ALIGN_TOP_MID, 0, 15);

	weather_label = lv_label_create(time_screen_cont_2);
    lv_label_set_text(weather_label, "WLAN2222222");
    lv_obj_set_style_text_color(weather_label, lv_color_white(), 0);
    lv_obj_set_style_text_opa(weather_label, LV_OPA_COVER, 0);
    lv_obj_set_style_text_font(weather_label, &lv_font_montserrat_20, 0);
    lv_obj_align_to(weather_label, time_screen_cont_2, LV_ALIGN_BOTTOM_MID, 0, -15);

    ui.status = 1;

}