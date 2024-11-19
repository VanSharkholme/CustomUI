//
// Created by 13651 on 2024/11/19.
//

#include "Callbacks.h"

void ImgBtnPressedCallback(lv_event_t *event)
{
    lv_obj_t *btn = lv_event_get_target(event);
    lv_obj_set_style_img_recolor(btn, lv_color_black(), 0);
    lv_obj_set_style_img_recolor_opa(btn, LV_OPA_30, 0);
}

void ImgBtnReleasedCallback(lv_event_t *event)
{
    lv_obj_t *btn = lv_event_get_target(event);
    lv_obj_set_style_img_recolor_opa(btn, LV_OPA_TRANSP, 0);
}
