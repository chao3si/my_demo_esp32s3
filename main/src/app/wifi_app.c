#include "wifi_app.h"

static lv_obj_t *wifi_return_btn;

static lv_obj_t *wifi_connect_cont;
static lv_obj_t *wifi_switch_btn;

static lv_obj_t *wifi_state_label;
static lv_obj_t *http_server_status_label;
static lv_obj_t *wifi_ip_addr_label;

static lv_style_t wifi_return_btn_style;
static lv_style_t wifi_switch_btn_focused_style;
static lv_style_t wifi_switch_btn_default_style;

static lv_timer_t * timer;


static void timer_cb(lv_timer_t *timer)
{
    if(get_wifi_ap_isopen())
    {
        switch (get_wifi_ap_status())
        {
            case WIFI_AP_MODE_INIT:
                lv_label_set_text(http_server_status_label, "server closed");
                break;
            case WIFI_SET_AP_MODE_SUCCESS:
                lv_label_set_text(http_server_status_label, "server on");
                break;            
            case WIFI_SET_AP_MODE_FAILED:
                lv_label_set_text(http_server_status_label, "server failed");
                break;
        }
    }
    else{
        lv_label_set_text(http_server_status_label, "server close");
    }

    if(get_wifi_sta_isopen())
    {
        switch (get_wifi_sta_status())
        {
            case WIFI_STA_MODE_INIT:
                lv_label_set_text(wifi_state_label, "OPENED");
                break;
            case WIFI_STA_MODE_CONNECT_SUCCESS:
                lv_label_set_text(wifi_state_label, "CONNEC");
                break;            
            case WIFI_STA_MODE_CONNECT_FAILED:
                lv_label_set_text(wifi_state_label, "UNCON");
                break;
        }
    }
    else{
        lv_label_set_text(wifi_state_label, "CLOSED");
    }
}

static void wifi_return_btn_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *label = lv_event_get_user_data(e);

    switch(code)
    {
        case LV_EVENT_FOCUSED:
            lv_style_set_text_opa(&wifi_return_btn_style, LV_OPA_COVER);
            lv_obj_add_style(label, &wifi_return_btn_style, LV_STATE_FOCUSED);
            break;
        case LV_EVENT_DEFOCUSED:
            lv_style_set_text_opa(&wifi_return_btn_style, LV_OPA_30);
            lv_obj_add_style(label, &wifi_return_btn_style, LV_EVENT_DEFOCUSED);
            break;
        case LV_EVENT_CLICKED:
            lv_group_remove_obj(wifi_return_btn);
            lv_group_remove_obj(wifi_switch_btn);
            lv_obj_del(wifi_return_btn);
            lv_obj_del(wifi_connect_cont);
            lv_obj_del(wifi_switch_btn);
            lv_timer_del(timer);
            back_to_menu_page(1);
            break;
        default:
            break;
    }
}

static void wifi_switch_btn_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *label = lv_event_get_user_data(e);


    bool status = get_wifi_sta_isopen();
    ESP_LOGI("wifi_switch_btn_cb", "status = %d", status);

    switch(code)
    {
        case LV_EVENT_FOCUSED:
            lv_style_set_bg_opa(&wifi_switch_btn_focused_style, LV_OPA_80);
            lv_obj_add_style(label, &wifi_switch_btn_focused_style, LV_STATE_FOCUSED);
            break;
        case LV_EVENT_DEFOCUSED:
            break;
        case LV_EVENT_CLICKED:
            if(status == false)
            {
                wifi_send_event(WIFI_STA_START);
                lv_label_set_text(wifi_state_label, "OPENED");
                lv_obj_set_style_text_color(wifi_state_label, lv_color_white(), 0);
                lv_obj_set_style_text_opa(wifi_state_label, LV_OPA_80, 0);
                lv_style_set_bg_color(&wifi_switch_btn_default_style, lv_color_hex(MY_COLOR_SPRING_GREEN));
                lv_obj_add_style(label, &wifi_switch_btn_default_style, LV_STATE_DEFAULT | LV_STATE_FOCUSED);
                lv_obj_add_style(wifi_switch_btn, &wifi_switch_btn_default_style, LV_STATE_DEFAULT);
                set_wifi_sta_isopen(true);
                set_wifi_ap_isopen(true);
            }
            else
            {
                wifi_send_event(WIFI_STA_STOP);
                lv_label_set_text(wifi_state_label, "CLOSED");
                lv_obj_set_style_text_color(wifi_state_label, lv_color_white(), 0);
                lv_obj_set_style_text_opa(wifi_state_label, LV_OPA_80, 0);
                lv_style_set_bg_color(&wifi_switch_btn_default_style, lv_color_hex(MY_COLOR_DODGER_BLUE));
                lv_obj_add_style(label, &wifi_switch_btn_default_style, LV_STATE_DEFAULT | LV_STATE_FOCUSED);
                lv_obj_add_style(wifi_switch_btn, &wifi_switch_btn_default_style, LV_STATE_DEFAULT);
                set_wifi_sta_isopen(false);
                set_wifi_ap_isopen(false);
            }

            break;
        default:
            break;
    }
}

void wifi_app_page()
{
	lv_group_t *group = get_group();
    
    //memset(&wifi_page_, 0, sizeof(wifi_PAGE_T));
    
    wifi_return_btn = lv_btn_create(bg_screen);
    lv_obj_remove_style_all(wifi_return_btn);
    lv_obj_set_size(wifi_return_btn, 20, 20);
    lv_obj_set_style_radius(wifi_return_btn, 10, 0);
    lv_obj_set_style_bg_color(wifi_return_btn, lv_color_white(), LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(wifi_return_btn, LV_OPA_30, LV_STATE_PRESSED);
    lv_obj_set_pos(wifi_return_btn, 5, 5);

    lv_style_init(&wifi_return_btn_style);
    lv_style_set_text_color(&wifi_return_btn_style, lv_color_white());
    lv_style_set_text_opa(&wifi_return_btn_style, LV_OPA_50);
    lv_style_set_text_font(&wifi_return_btn_style, &lv_font_montserrat_20);

    lv_obj_t *left_icon_label = lv_label_create(wifi_return_btn);
    lv_label_set_text(left_icon_label, LV_SYMBOL_LEFT);
    lv_obj_add_style(left_icon_label, &wifi_return_btn_style, 0);
    lv_obj_add_event_cb(wifi_return_btn, wifi_return_btn_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(wifi_return_btn, wifi_return_btn_cb, LV_EVENT_FOCUSED, left_icon_label);
    lv_obj_add_event_cb(wifi_return_btn, wifi_return_btn_cb, LV_EVENT_DEFOCUSED, left_icon_label);
    lv_obj_align(left_icon_label, LV_ALIGN_CENTER, 0, 0);

    lv_group_add_obj(group, wifi_return_btn);

    /* wifi switch */
    //static lv_style_t wifi_switch_btn_default_style;

    lv_style_init(&wifi_switch_btn_default_style);
    lv_style_set_radius(&wifi_switch_btn_default_style, 10);
    lv_style_set_bg_color(&wifi_switch_btn_default_style, lv_color_hex(MY_COLOR_DODGER_BLUE));
    lv_style_set_bg_opa(&wifi_switch_btn_default_style, LV_OPA_COVER);
    
    //static lv_style_t wifi_switch_btn_focused_style;
    lv_style_init(&wifi_switch_btn_focused_style);
    lv_style_set_bg_opa(&wifi_switch_btn_focused_style, LV_OPA_80);

    wifi_switch_btn = lv_btn_create(bg_screen);
    lv_obj_remove_style_all(wifi_switch_btn);
    lv_obj_set_size(wifi_switch_btn, 180, 60);
    lv_obj_add_style(wifi_switch_btn, &wifi_switch_btn_default_style, 0);
    lv_obj_add_style(wifi_switch_btn, &wifi_switch_btn_focused_style, LV_STATE_FOCUSED);
    lv_obj_align_to(wifi_switch_btn, bg_screen, LV_ALIGN_TOP_LEFT, 30, 20);

    lv_obj_t *wifi_open_label = lv_label_create(wifi_switch_btn);
    lv_label_set_text(wifi_open_label, "WIFI state");
    lv_obj_set_style_text_color(wifi_open_label, lv_color_white(), 0);
    lv_obj_set_style_text_opa(wifi_open_label, LV_OPA_COVER, 0);
    lv_obj_set_style_text_font(wifi_open_label, &lv_font_montserrat_20, 0);
    lv_obj_align_to(wifi_open_label, wifi_switch_btn, LV_ALIGN_TOP_LEFT, 10, 5);

    wifi_state_label = lv_label_create(wifi_switch_btn);

                

    if(get_wifi_sta_isopen())
    {
        lv_obj_set_style_text_color(wifi_state_label, lv_color_white(), 0);
        lv_obj_set_style_text_opa(wifi_state_label, LV_OPA_80, 0);
        lv_style_set_bg_color(&wifi_switch_btn_default_style, lv_color_hex(MY_COLOR_SPRING_GREEN));
        lv_obj_add_style(wifi_state_label, &wifi_switch_btn_default_style, LV_STATE_DEFAULT | LV_STATE_FOCUSED);
        lv_obj_add_style(wifi_switch_btn, &wifi_switch_btn_default_style, LV_STATE_DEFAULT);
        switch (get_wifi_sta_status())
        {
            case WIFI_STA_MODE_INIT:
                lv_label_set_text(wifi_state_label, "OPENED");
                break;
            case WIFI_STA_MODE_CONNECT_SUCCESS:
                lv_label_set_text(wifi_state_label, "CONNEC");
                break;            
            case WIFI_STA_MODE_CONNECT_FAILED:
                lv_label_set_text(wifi_state_label, "UNCON");
                break;
        }
    }
    else{
        lv_obj_set_style_text_color(wifi_state_label, lv_color_white(), 0);
        lv_obj_set_style_text_opa(wifi_state_label, LV_OPA_80, 0);
        lv_style_set_bg_color(&wifi_switch_btn_default_style, lv_color_hex(MY_COLOR_DODGER_BLUE));
        lv_obj_add_style(wifi_state_label, &wifi_switch_btn_default_style, LV_STATE_DEFAULT | LV_STATE_FOCUSED);
        lv_obj_add_style(wifi_switch_btn, &wifi_switch_btn_default_style, LV_STATE_DEFAULT);
        lv_label_set_text(wifi_state_label, "CLOSED");
    }

    lv_obj_set_style_text_color(wifi_state_label, lv_color_white(), 0);
    lv_obj_set_style_text_opa(wifi_state_label, LV_OPA_80, 0);
    lv_obj_set_style_text_font(wifi_state_label, &lv_font_montserrat_20, 0);
    lv_obj_align_to(wifi_state_label, wifi_switch_btn, LV_ALIGN_BOTTOM_RIGHT, -10, -5);

    lv_obj_add_event_cb(wifi_switch_btn, wifi_switch_btn_cb, LV_EVENT_CLICKED, wifi_state_label);

    /* wifi cont */
    wifi_connect_cont = lv_obj_create(bg_screen);
    lv_obj_remove_style_all(wifi_connect_cont);
    lv_obj_set_size(wifi_connect_cont, 180, 130); 
    lv_obj_set_pos(wifi_connect_cont, 30, 90);
    lv_obj_set_style_radius(wifi_connect_cont, 10, 0);
    lv_obj_set_style_bg_opa(wifi_connect_cont, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(wifi_connect_cont, lv_color_hex(MY_COLOR_DODGER_BLUE), 0);
    //lv_obj_align_to(wifi_connect_cont, wifi_switch_btn, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    /* WLAN */
    lv_obj_t *wifi_wlan_label = lv_label_create(wifi_connect_cont);
    lv_label_set_text(wifi_wlan_label, "WLAN");
    lv_obj_set_style_text_color(wifi_wlan_label, lv_color_white(), 0);
    lv_obj_set_style_text_opa(wifi_wlan_label, LV_OPA_COVER, 0);
    lv_obj_set_style_text_font(wifi_wlan_label, &lv_font_montserrat_20, 0);
    lv_obj_align_to(wifi_wlan_label, wifi_connect_cont, LV_ALIGN_TOP_MID, 0, 5);

    /* ssid cont*/
    lv_obj_t *ssid_cont = lv_obj_create(wifi_connect_cont);
    lv_obj_remove_style_all(ssid_cont);
    lv_obj_set_size(ssid_cont, 170, 40);
    lv_obj_set_style_radius(ssid_cont, 10, 0);
    lv_obj_set_style_bg_color(ssid_cont, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(ssid_cont, LV_OPA_40, 0);
    lv_obj_align_to(ssid_cont, wifi_connect_cont, LV_ALIGN_TOP_LEFT, 5, 30);

    /* ssid icon */
    lv_obj_t *wifi_icon_label = lv_label_create(ssid_cont);
    lv_label_set_text(wifi_icon_label, LV_SYMBOL_WIFI);
    lv_obj_set_style_text_font(wifi_icon_label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_opa(wifi_icon_label, LV_OPA_80, 0);
    lv_obj_set_style_text_color(wifi_icon_label, lv_color_white(), 0);
    lv_obj_align_to(wifi_icon_label, ssid_cont, LV_ALIGN_LEFT_MID, 10, 0);

    /* ssid label*/
    http_server_status_label = lv_label_create(ssid_cont);
    lv_label_set_long_mode(http_server_status_label, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_width(http_server_status_label, 170);
    lv_obj_set_style_text_font(http_server_status_label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_opa(http_server_status_label, LV_OPA_80, 0);
    lv_obj_set_style_text_color(http_server_status_label, lv_color_white(), 0);
    //lv_label_set_text_fmt(http_server_status_label, " ");
    lv_obj_align_to(http_server_status_label, wifi_icon_label, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

    if(get_wifi_ap_isopen())
    {
        switch (get_wifi_ap_status())
        {
            case WIFI_AP_MODE_INIT:
                lv_label_set_text(http_server_status_label, "server closed");
                break;
            case WIFI_SET_AP_MODE_SUCCESS:
                lv_label_set_text(http_server_status_label, "server on");
                break;            
            case WIFI_SET_AP_MODE_FAILED:
                lv_label_set_text(http_server_status_label, "server failed");
                break;
        }
    }
    else{
        lv_label_set_text(http_server_status_label, "server close");
    }
    /* ip cont*/
    lv_obj_t *ip_cont = lv_obj_create(wifi_connect_cont);
    lv_obj_remove_style_all(ip_cont);
    lv_obj_set_size(ip_cont, 170, 40);
    lv_obj_set_style_radius(ip_cont, 10, 0);
    lv_obj_set_style_bg_color(ip_cont, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(ip_cont, LV_OPA_40, 0);
    lv_obj_align_to(ip_cont, ssid_cont, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);

    /* ip icon*/
    lv_obj_t *wifi_ip_label = lv_label_create(ip_cont);
    lv_label_set_text_fmt(wifi_ip_label, "IP");
    lv_obj_set_style_text_font(wifi_ip_label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_opa(wifi_ip_label, LV_OPA_80, 0);
    lv_obj_set_style_text_color(wifi_ip_label, lv_color_white(), 0);
    lv_obj_align_to(wifi_ip_label, ip_cont, LV_ALIGN_TOP_LEFT, 10, 10);

    /* ip label */
    wifi_ip_addr_label = lv_label_create(ip_cont);
    lv_obj_set_style_text_font(wifi_ip_addr_label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_opa(wifi_ip_addr_label, LV_OPA_80, 0);
    lv_obj_set_style_text_color(wifi_ip_addr_label, lv_color_white(), 0);
    lv_obj_align_to(wifi_ip_addr_label, wifi_ip_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);


    /*定时回调，用于检测wifi状态*/
    timer = lv_timer_create(timer_cb, 500, NULL);

    /* 添加组 */
    lv_group_add_obj(group, wifi_return_btn);
    lv_group_add_obj(group, wifi_switch_btn);

    ui.status = 1;

}