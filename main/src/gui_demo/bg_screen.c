#include "bg_screen.h"

lv_obj_t *bg_screen;

void bg_screen_create(void)
{
    bg_screen = lv_obj_create(lv_scr_act());
	lv_obj_set_size(bg_screen, 240, 240);
	
	static lv_style_t style_screen;

	lv_style_init(&style_screen);
	lv_style_set_pad_hor(&style_screen, 0);
	lv_style_set_pad_ver(&style_screen, 0);
	lv_style_set_border_width(&style_screen, 0);
	lv_style_set_radius(&style_screen, 0);
	lv_style_set_bg_color(&style_screen, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_opa(&style_screen, 255);
	lv_obj_add_style(bg_screen, &style_screen, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_clear_flag(bg_screen, LV_OBJ_FLAG_SCROLLABLE);
}