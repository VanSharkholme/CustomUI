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

void AddSchemeBtnCallback(lv_event_t *event)
{
    lv_obj_t *btn = lv_event_get_target(event);
    lv_obj_t *btn_container = lv_obj_get_parent(btn);
    lv_obj_t *channel = lv_obj_get_parent(btn_container);
//    Channel *ch = (Channel*)lv_obj_get_user_data(channel);
    lv_obj_set_user_data(scheme_scr, channel);
    lv_scr_load(scheme_scr);
}

void BackBtnCallback(lv_event_t *event)
{
    lv_scr_load(main_scr);
}

void SchemeDropdownCallback(lv_event_t *event)
{
    lv_obj_t *scheme_set_dropdown = lv_event_get_target(event);
    SchemeSet *scheme_set = (SchemeSet*) lv_obj_get_user_data(scheme_set_dropdown);
    lv_obj_t *channel = (lv_obj_t *) lv_obj_get_user_data(scheme_scr);
    Channel *ch = (Channel *)lv_obj_get_user_data(channel);
    uint8_t plan_index = lv_dropdown_get_selected(scheme_set_dropdown);
    ch->plan = &scheme_set->plans[plan_index];
    ch->timer.timer_sec = scheme_set->plans[plan_index].timer_sec;
    ch->timer.timer_min = scheme_set->plans[plan_index].timer_min;
    set_channel_state(channel, UI_CHANNEL_STATE_ADDED);
    lv_scr_load(main_scr);
}

void ClearButtonCallback(lv_event_t *event)
{
    clear_all_channels();
}
