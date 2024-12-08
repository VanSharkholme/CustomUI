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
//    UI_Channel *ch = (UI_Channel*)lv_obj_get_user_data(channel);
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
    UI_Channel *ch = (UI_Channel *)lv_obj_get_user_data(channel);
    uint8_t plan_index = lv_dropdown_get_selected(scheme_set_dropdown);
    ch->pPlan = lv_mem_alloc(sizeof(Plan));
    memcpy(ch->pPlan, scheme_set->plans[plan_index], sizeof(Plan));
    ch->timer.timer_min = scheme_set->plans[plan_index]->total_time_min;
    ch->timer.timer_sec = 0;
    set_channel_state(channel, UI_CHANNEL_STATE_ADDED);
    lv_scr_load(main_scr);
}

void ClearBtnCallback(lv_event_t *event)
{
    clear_all_channels();
}

void PrevPageBtnCallback(lv_event_t *event)
{
    current_page_num = ((current_page_num - 1) % UI_SCHEME_PAGE_NUM + UI_SCHEME_PAGE_NUM) % UI_SCHEME_PAGE_NUM;
    lv_obj_t *btn = lv_event_get_target(event);
    lv_obj_t *container = lv_obj_get_parent(btn);
    lv_obj_t *page_num_label = lv_obj_get_child(container, 1);
    lv_obj_t *scheme_set_list_container = lv_obj_get_user_data(btn);
    set_scheme_set_page(scheme_set_list_container, current_page_num);
    lv_label_set_text_fmt(page_num_label, "第%d:%d页", current_page_num + 1, UI_SCHEME_PAGE_NUM);
}

void NextPageBtnCallback(lv_event_t *event)
{
    current_page_num = (current_page_num + 1) % UI_SCHEME_PAGE_NUM;
    lv_obj_t *btn = lv_event_get_target(event);
    lv_obj_t *container = lv_obj_get_parent(btn);
    lv_obj_t *page_num_label = lv_obj_get_child(container, 1);
    lv_obj_t *scheme_set_list_container = lv_obj_get_user_data(btn);
    set_scheme_set_page(scheme_set_list_container, current_page_num);
    lv_label_set_text_fmt(page_num_label, "第%d:%d页", current_page_num + 1, UI_SCHEME_PAGE_NUM);
}

void AddCurrentBtnCallback(lv_event_t *event)
{
    lv_obj_t *btn = lv_event_get_target(event);
    lv_obj_t *current_container = lv_obj_get_parent(btn);
    refresh_channel_current(current_container, 1);
}

void SubCurrentBtnCallback(lv_event_t *event)
{
    lv_obj_t *btn = lv_event_get_target(event);
    lv_obj_t *current_container = lv_obj_get_parent(btn);
    refresh_channel_current(current_container, -1);
}

void SyncConfirmBtnCallback(lv_event_t *event)
{
    lv_obj_t *btn = lv_event_get_target(event);
    lv_obj_t *modal_bg = lv_obj_get_parent(btn);
    lv_obj_t *sync_adjust_container = lv_obj_get_child(modal_bg, 0);
    lv_obj_t *sync_adjust_bg = lv_obj_get_child(sync_adjust_container, 0);
    lv_obj_t *difference_label = lv_obj_get_child(sync_adjust_bg, 0);
    int8_t *difference = (int8_t *)lv_obj_get_user_data(difference_label);
    for (int i = 0; i < 4; ++i) {
        lv_obj_t *channel = get_channel_by_index(i);
        UI_Channel *ch = (UI_Channel*)lv_obj_get_user_data(channel);
        if (ch->state == UI_CHANNEL_STATE_ADDED) {
            lv_obj_t *adjust_container = lv_obj_get_child(channel, 2);
            refresh_channel_current(adjust_container, *difference);
        }
    }

    lv_obj_del_async(modal_bg);
}

void CancelBtnCallback(lv_event_t *event)
{
    lv_obj_t *btn = lv_event_get_target(event);
    lv_obj_t *modal_bg = lv_obj_get_parent(btn);
    lv_obj_del_async(modal_bg);
}

void SyncAddBtnCallback(lv_event_t *event)
{
    lv_obj_t *btn = lv_event_get_target(event);
    lv_obj_t *sync_adjust_container = lv_obj_get_parent(btn);
    lv_obj_t *sync_adjust_bg = lv_obj_get_child(sync_adjust_container, 0);
    lv_obj_t *difference_label = lv_obj_get_child(sync_adjust_bg, 0);
    int8_t *difference = (int8_t *)lv_obj_get_user_data(difference_label);
    *difference += 1;
    if (*difference > 0)
        lv_label_set_text_fmt(difference_label, "+%d", *difference);
    else
        lv_label_set_text_fmt(difference_label, "%d", *difference);
}

void SyncSubBtnCallback(lv_event_t *event)
{
    lv_obj_t *btn = lv_event_get_target(event);
    lv_obj_t *sync_adjust_container = lv_obj_get_parent(btn);
    lv_obj_t *sync_adjust_bg = lv_obj_get_child(sync_adjust_container, 0);
    lv_obj_t *difference_label = lv_obj_get_child(sync_adjust_bg, 0);
    int8_t *difference = (int8_t *)lv_obj_get_user_data(difference_label);
    *difference -= 1;
    if (*difference > 0)
        lv_label_set_text_fmt(difference_label, "+%d", *difference);
    else
        lv_label_set_text_fmt(difference_label, "%d", *difference);
}

void SyncAdjustBtnCallback(lv_event_t *event)
{
    lv_obj_t *modal_bg = lv_obj_create(main_scr);
    lv_obj_set_size(modal_bg, lv_obj_get_width(main_scr), lv_obj_get_height(main_scr));
    lv_obj_set_style_bg_color(modal_bg, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(modal_bg, LV_OPA_80, 0);
    lv_obj_set_style_border_width(modal_bg, 0, 0);
    lv_obj_add_flag(modal_bg, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t *sync_adjust_container = lv_obj_create(modal_bg);
    lv_obj_set_size(sync_adjust_container, 300, 400);
    lv_obj_align(sync_adjust_container, LV_ALIGN_CENTER, 0, -120);
    lv_obj_set_style_bg_opa(sync_adjust_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_opa(sync_adjust_container, LV_OPA_TRANSP, 0);

    lv_obj_t *sync_adjust_bg = lv_img_create(sync_adjust_container);
    lv_img_set_src(sync_adjust_bg, &SyncAdjust_bg_fit);
    lv_obj_center(sync_adjust_bg);

    lv_obj_t *difference_label = lv_label_create(sync_adjust_bg);
    static int8_t difference = 0;
    difference = 0;
    lv_label_set_text_fmt(difference_label, "%d", difference);
    lv_obj_set_style_text_font(difference_label, &AliPuHui_50, 0);
    lv_obj_align(difference_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_user_data(difference_label, &difference);
    lv_obj_set_style_text_color(difference_label, lv_color_white(), 0);

    lv_obj_t *unit_label = lv_label_create(sync_adjust_bg);
    lv_label_set_text(unit_label, "mA");
    lv_obj_set_style_text_font(unit_label, &lv_font_montserrat_16, 0);
    lv_obj_align_to(unit_label, difference_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_style_text_color(unit_label, lv_color_white(), 0);

    lv_obj_t *sync_add_btn = lv_imgbtn_create(sync_adjust_container);
    lv_imgbtn_set_src(sync_add_btn, LV_IMGBTN_STATE_RELEASED, NULL, &AddButton_White_140_fit, NULL);
    lv_imgbtn_set_src(sync_add_btn, LV_IMGBTN_STATE_PRESSED, NULL, &AddButton_Green_fit, NULL);
    lv_obj_set_size(sync_add_btn, AddButton_White_140_fit.header.w, AddButton_White_140_fit.header.h);
    lv_obj_align(sync_add_btn, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    lv_obj_add_event_cb(sync_add_btn, SyncAddBtnCallback, LV_EVENT_CLICKED, NULL);

    lv_obj_t *sync_sub_btn = lv_imgbtn_create(sync_adjust_container);
    lv_imgbtn_set_src(sync_sub_btn, LV_IMGBTN_STATE_RELEASED, NULL, &SubtractButton_White_140_fit, NULL);
    lv_imgbtn_set_src(sync_sub_btn, LV_IMGBTN_STATE_PRESSED, NULL, &SubtractButton_fit, NULL);
    lv_obj_set_size(sync_sub_btn, SubtractButton_White_140_fit.header.w, SubtractButton_White_140_fit.header.h);
    lv_obj_align(sync_sub_btn, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    lv_obj_add_event_cb(sync_sub_btn, SyncSubBtnCallback, LV_EVENT_CLICKED, NULL);

    lv_obj_t *confirm_btn = lv_imgbtn_create(modal_bg);
    lv_imgbtn_set_src(confirm_btn, LV_IMGBTN_STATE_RELEASED, NULL, &ConfirmButtonBig_fit, NULL);
    lv_obj_set_size(confirm_btn, ConfirmButtonBig_fit.header.w, ConfirmButtonBig_fit.header.h);
    lv_obj_align(confirm_btn, LV_ALIGN_BOTTOM_MID, 0, -220);
    lv_obj_add_event_cb(confirm_btn, SyncConfirmBtnCallback, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(confirm_btn, ImgBtnPressedCallback, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(confirm_btn, ImgBtnReleasedCallback, LV_EVENT_RELEASED, NULL);

    lv_obj_t *cancel_btn = lv_imgbtn_create(modal_bg);
    lv_imgbtn_set_src(cancel_btn, LV_IMGBTN_STATE_RELEASED, NULL, &CancelButtonBig_fit, NULL);
    lv_obj_set_size(cancel_btn, CancelButtonBig_fit.header.w, CancelButtonBig_fit.header.h);
    lv_obj_align(cancel_btn, LV_ALIGN_BOTTOM_MID, 0, -140);
    lv_obj_add_event_cb(cancel_btn, CancelBtnCallback, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(cancel_btn, ImgBtnPressedCallback, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(cancel_btn, ImgBtnReleasedCallback, LV_EVENT_RELEASED, NULL);

}

void CalibBtnCallback(lv_event_t *event)
{
    lv_scr_load(calib_scr);
}

void TextAreaCallback(lv_event_t *event)
{
    lv_event_code_t code = lv_event_get_code(event);
    lv_obj_t *textarea = lv_event_get_target(event);
    lv_obj_t *keyboard = lv_event_get_user_data(event);
    if (code == LV_EVENT_FOCUSED) {
        lv_keyboard_set_textarea(keyboard, textarea);
        lv_obj_clear_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
    }
    else if (code == LV_EVENT_DEFOCUSED || code == LV_EVENT_LEAVE) {
        lv_keyboard_set_textarea(keyboard, NULL);
        lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
    }
}

void BTConfirmBtnCallback(lv_event_t *event)
{
    lv_obj_t *btn = lv_event_get_target(event);
    lv_obj_t *modal_bg = lv_obj_get_parent(btn);
    lv_obj_t *bluetooth_container = lv_obj_get_child(modal_bg, 0);
    lv_obj_t *bluetooth_name_input = lv_obj_get_child(bluetooth_container, 1);
    char *bluetooth_name = (char *)lv_textarea_get_text(bluetooth_name_input);
    LV_LOG_USER("bluetooth name: %s", bluetooth_name);
    //TODO: save bluetooth name to flash
    lv_obj_del_async(modal_bg);
}

void BluetoothBtnCallback(lv_event_t *event)
{
    lv_obj_t *modal_bg = lv_obj_create(main_scr);
    lv_obj_set_size(modal_bg, lv_obj_get_width(main_scr), lv_obj_get_height(main_scr));
    lv_obj_set_style_bg_color(modal_bg, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(modal_bg, LV_OPA_80, 0);
    lv_obj_set_style_border_width(modal_bg, 0, 0);
    lv_obj_set_style_pad_all(modal_bg, 0, 0);
    lv_obj_add_flag(modal_bg, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t *bluetooth_container = lv_obj_create(modal_bg);
    lv_obj_set_size(bluetooth_container, 400, 400);
    lv_obj_align(bluetooth_container, LV_ALIGN_CENTER, 0, -100);
    lv_obj_set_style_bg_opa(bluetooth_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_opa(bluetooth_container, LV_OPA_TRANSP, 0);
    lv_obj_add_flag(bluetooth_container, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t *tag = lv_label_create(bluetooth_container);
    lv_label_set_text(tag, "蓝牙设置");
    lv_obj_set_style_text_font(tag, &AliPuHui_30, 0);
    lv_obj_set_style_text_color(tag, lv_color_white(), 0);

    lv_obj_t *keyboard = lv_keyboard_create(modal_bg);
    lv_obj_set_size(keyboard,LV_PCT(100), 300);
    lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
    lv_obj_remove_event_cb(keyboard, lv_keyboard_def_event_cb);
    lv_obj_add_event_cb(keyboard, lv_keyboard_def_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *bluetooth_name_input = lv_textarea_create(bluetooth_container);
    lv_textarea_set_placeholder_text(bluetooth_name_input, "请输入蓝牙名称");
    lv_obj_set_style_text_font(bluetooth_name_input, &AliPuHui_28, 0);
    lv_obj_set_style_bg_color(bluetooth_name_input, lv_color_hex(0xe3e3e3), 0);
    lv_obj_set_style_radius(bluetooth_name_input, 0, 0);
    lv_obj_set_size(bluetooth_name_input,LV_PCT(100), 55);
    lv_obj_align(bluetooth_name_input, LV_ALIGN_CENTER, 0, -80);
    lv_obj_set_scroll_dir(bluetooth_name_input, LV_DIR_HOR);
    lv_obj_add_event_cb(bluetooth_name_input, TextAreaCallback, LV_EVENT_ALL, keyboard);

    lv_obj_t *cancel_btn = lv_imgbtn_create(modal_bg);
    lv_imgbtn_set_src(cancel_btn, LV_IMGBTN_STATE_RELEASED, NULL, &CancelButtonBig_fit, NULL);
    lv_obj_set_size(cancel_btn, CancelButtonBig_fit.header.w, CancelButtonBig_fit.header.h);
    lv_obj_align_to(cancel_btn, keyboard, LV_ALIGN_OUT_TOP_MID, 0, -50);
    lv_obj_add_event_cb(cancel_btn, CancelBtnCallback, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(cancel_btn, ImgBtnPressedCallback, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(cancel_btn, ImgBtnReleasedCallback, LV_EVENT_RELEASED, NULL);
    lv_obj_t *confirm_btn = lv_imgbtn_create(modal_bg);

    lv_imgbtn_set_src(confirm_btn, LV_IMGBTN_STATE_RELEASED, NULL, &ConfirmButtonBig_fit, NULL);
    lv_obj_set_size(confirm_btn, ConfirmButtonBig_fit.header.w, ConfirmButtonBig_fit.header.h);
    lv_obj_align_to(confirm_btn, cancel_btn, LV_ALIGN_OUT_TOP_MID, 0, -20);
    lv_obj_add_event_cb(confirm_btn, BTConfirmBtnCallback, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(confirm_btn, ImgBtnPressedCallback, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(confirm_btn, ImgBtnReleasedCallback, LV_EVENT_RELEASED, NULL);

}

void ChildLockBtnCallback(lv_event_t *event)
{
    lv_obj_t *btn = lv_event_get_target(event);
    bool *is_locked = (bool *)lv_obj_get_user_data(btn);
    *is_locked = !(*is_locked);
    if (*is_locked) {
        lv_imgbtn_set_src(btn, LV_IMGBTN_STATE_RELEASED, NULL, &LockButton_Black_fit, NULL);
        lv_obj_align(btn, LV_ALIGN_LEFT_MID, -10, 0);
        lv_obj_set_size(btn, LockButton_Black_fit.header.w, LockButton_Black_fit.header.h);
        for (int i = 0; i < 4; ++i) {
            lv_obj_t *channel = get_channel_by_index(i);
            set_channel_state(channel, UI_CHANNEL_STATE_DISABLED);
        }

        lv_obj_t *clear_btn = lv_obj_get_child(main_scr, 1);
        lv_obj_clear_flag(clear_btn, LV_OBJ_FLAG_CLICKABLE);

        lv_obj_t *sync_adjust_btn = lv_obj_get_child(main_scr, 2);
        lv_obj_clear_flag(sync_adjust_btn, LV_OBJ_FLAG_CLICKABLE);

        lv_obj_t *start_btn = lv_obj_get_child(main_scr, 3);
        lv_imgbtn_set_src(start_btn, LV_IMGBTN_STATE_RELEASED, NULL, &StartButton_Gray_fit, NULL);
        lv_obj_clear_flag(start_btn, LV_OBJ_FLAG_CLICKABLE);
    }
    else {
        lv_imgbtn_set_src(btn, LV_IMGBTN_STATE_RELEASED, NULL, &LockIconTransparent_fit, NULL);
        lv_obj_set_size(btn, LockIconTransparent_fit.header.w, LockIconTransparent_fit.header.h);
        lv_obj_align(btn, LV_ALIGN_LEFT_MID, 0, 0);
        for (int i = 0; i < 4; ++i) {
            lv_obj_t *channel = get_channel_by_index(i);
            UI_Channel *ch = (UI_Channel*)lv_obj_get_user_data(channel);
            set_channel_state(channel, ch->prev_state);
        }

        lv_obj_t *clear_btn = lv_obj_get_child(main_scr, 1);
        lv_obj_add_flag(clear_btn, LV_OBJ_FLAG_CLICKABLE);

        lv_obj_t *sync_adjust_btn = lv_obj_get_child(main_scr, 2);
        lv_obj_add_flag(sync_adjust_btn, LV_OBJ_FLAG_CLICKABLE);

        lv_obj_t *start_btn = lv_obj_get_child(main_scr, 3);
        lv_imgbtn_set_src(start_btn, LV_IMGBTN_STATE_RELEASED, NULL, &StartButton_Green_fit, NULL);
        lv_obj_add_flag(start_btn, LV_OBJ_FLAG_CLICKABLE);
    }
}

void TimerLabelClickCallback(lv_event_t *event)
{
    lv_obj_t *channel = lv_event_get_user_data(event);
    UI_Channel *ch = (UI_Channel*)lv_obj_get_user_data(channel);
    if (ch->timer.state == UI_TIMER_STATE_STOP)
        set_channel_timer_state(channel, UI_TIMER_STATE_START);
    else
        set_channel_timer_state(channel, UI_TIMER_STATE_STOP);

}

void StimulationStartBtnCallback(lv_event_t *event)
{
    lv_obj_t *btn = lv_event_get_target(event);
    bool *is_stimulation_running = (bool *)lv_obj_get_user_data(btn);
    *is_stimulation_running = !(*is_stimulation_running);
    if (*is_stimulation_running) {
        lv_imgbtn_set_src(btn, LV_IMGBTN_STATE_RELEASED, NULL, &PauseButton_fit, NULL);
        for (int i = 0; i < 4; ++i) {
            lv_obj_t *channel = get_channel_by_index(i);
            UI_Channel *ch = (UI_Channel *) lv_obj_get_user_data(channel);
            if (i == 2) { // 模拟阻抗测试未通过
                if (ch->state == UI_CHANNEL_STATE_ADDED && ch->timer.state == UI_TIMER_STATE_STOP)
                    set_channel_state(channel, UI_CHANNEL_STATE_DROPPED);
            }
            else {
                if (ch->state == UI_CHANNEL_STATE_ADDED && ch->timer.state == UI_TIMER_STATE_STOP)
                    set_channel_timer_state(channel, UI_TIMER_STATE_START);
            }
        }
    }
    else {
        lv_imgbtn_set_src(btn, LV_IMGBTN_STATE_RELEASED, NULL, &StartButton_Green_fit, NULL);
        for (int i = 0; i < 4; ++i) {
            lv_obj_t *channel = get_channel_by_index(i);
            UI_Channel *ch = (UI_Channel *) lv_obj_get_user_data(channel);
            if (ch->state == UI_CHANNEL_STATE_ADDED && ch->timer.state == UI_TIMER_STATE_START)
                set_channel_timer_state(channel, UI_TIMER_STATE_STOP);
        }
    }
}

void DropModalDelCallback(lv_event_t *event)
{
    lv_obj_t *channel = lv_event_get_user_data(event);
    UI_Channel *ch = (UI_Channel*)lv_obj_get_user_data(channel);
    if (ch->state == UI_CHANNEL_STATE_DROPPED && 1) {// 1代表阻抗测试通过
        set_channel_state(channel, UI_CHANNEL_STATE_ADDED);
    }
}

void ModalDelCallback(lv_event_t *event)
{
    lv_obj_t *obj = lv_event_get_target(event);
    lv_obj_del_async(obj);
}

void ChannelLabelClickCallback(lv_event_t *event)
{
    lv_obj_t *channel = lv_event_get_user_data(event);
    UI_Channel *ch = (UI_Channel*)lv_obj_get_user_data(channel);
    Plan *pPlan = ch->pPlan;

    lv_obj_t *modal_container = lv_obj_create(main_scr);
    lv_obj_set_size(modal_container, lv_obj_get_width(main_scr), lv_obj_get_height(main_scr));
    lv_obj_set_style_bg_opa(modal_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_pad_all(modal_container, 0, 0);
    lv_obj_set_style_border_width(modal_container, 0, 0);
    lv_obj_add_flag(modal_container, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(modal_container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(modal_container, ModalDelCallback, LV_EVENT_CLICKED, modal_container);

    lv_obj_t *modal_bg = lv_obj_create(modal_container);
    lv_obj_set_size(modal_bg,LV_PCT(100), 500);
    lv_obj_align(modal_bg, LV_ALIGN_BOTTOM_MID, 0, 40);
    lv_obj_set_style_bg_color(modal_bg, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(modal_bg, LV_OPA_80, 0);
    lv_obj_set_style_border_width(modal_bg, 0, 0);
    lv_obj_set_style_pad_all(modal_bg, 0, 0);
    lv_obj_set_style_radius(modal_bg, 50, 0);

    lv_obj_t *title_label = lv_label_create(modal_bg);
    lv_label_set_text(title_label, "方案详情:");
    lv_obj_set_style_text_font(title_label, &AliPuHui_30, 0);
    lv_obj_set_style_text_color(title_label, lv_color_white(), 0);
    lv_obj_align(title_label, LV_ALIGN_TOP_LEFT, 30, 30);

    lv_obj_t *plan_name_label = lv_label_create(modal_bg);
    lv_label_set_text_fmt(plan_name_label, "方案名称: %s", pPlan->name);
    lv_obj_set_style_text_font(plan_name_label, &AliPuHui_28, 0);
    lv_obj_set_style_text_color(plan_name_label, lv_color_white(), 0);
    lv_obj_align(plan_name_label, LV_ALIGN_TOP_LEFT, 30, 80);

    lv_obj_t *wave_type_label = lv_label_create(modal_bg);
    lv_label_set_text(wave_type_label, "波形:");
    lv_obj_set_style_text_font(wave_type_label, &AliPuHui_28, 0);
    lv_obj_set_style_text_color(wave_type_label, lv_color_white(), 0);
    lv_obj_align(wave_type_label, LV_ALIGN_TOP_LEFT, 30, 120);

    lv_obj_t *freq_label = lv_label_create(modal_bg);
    lv_label_set_text_fmt(freq_label, "频率: %d Hz", pPlan->freq_min);
    lv_obj_set_style_text_font(freq_label, &AliPuHui_28, 0);
    lv_obj_set_style_text_color(freq_label, lv_color_white(), 0);
    lv_obj_align(freq_label, LV_ALIGN_TOP_LEFT, 30, 160);

    lv_obj_t *pulse_width_label = lv_label_create(modal_bg);
    lv_label_set_text_fmt(pulse_width_label, "脉宽: %d us", pPlan->pulse_width);
    lv_obj_set_style_text_font(pulse_width_label, &AliPuHui_28, 0);
    lv_obj_set_style_text_color(pulse_width_label, lv_color_white(), 0);
    lv_obj_align(pulse_width_label, LV_ALIGN_TOP_LEFT, 30, 200);
}

void ProgressBarIndicatorCallback(lv_event_t * event)
{
    lv_obj_draw_part_dsc_t * dsc = lv_event_get_draw_part_dsc(event);
    if(dsc->part != LV_PART_INDICATOR) return;

    lv_obj_t * obj = lv_event_get_target(event);

    lv_draw_label_dsc_t label_dsc;
    lv_draw_label_dsc_init(&label_dsc);
    label_dsc.font = LV_FONT_DEFAULT;

    char buf[8];
    lv_snprintf(buf, sizeof(buf), "%d", (int)lv_bar_get_value(obj));

    lv_point_t txt_size;
    lv_txt_get_size(&txt_size, buf, label_dsc.font, label_dsc.letter_space, label_dsc.line_space, LV_COORD_MAX,
                    label_dsc.flag);

    lv_area_t txt_area;
    /*If the indicator is long enough put the text inside on the right*/
    if(lv_area_get_width(dsc->draw_area) > txt_size.x + 20) {
        txt_area.x2 = dsc->draw_area->x2 - 5;
        txt_area.x1 = txt_area.x2 - txt_size.x + 1;
        label_dsc.color = lv_color_white();
    }
        /*If the indicator is still short put the text out of it on the right*/
    else {
        txt_area.x1 = dsc->draw_area->x2 + 5;
        txt_area.x2 = txt_area.x1 + txt_size.x - 1;
        label_dsc.color = lv_color_black();
    }

    txt_area.y1 = dsc->draw_area->y1 + (lv_area_get_height(dsc->draw_area) - txt_size.y) / 2;
    txt_area.y2 = txt_area.y1 + txt_size.y - 1;

    lv_draw_label(dsc->draw_ctx, &label_dsc, &txt_area, buf, NULL);
}

void CurrentWarningModalCancelCallback(lv_event_t *event)
{
    lv_obj_t *obj = lv_event_get_user_data(event);
    lv_obj_del_async(obj);
}

void CurrentWarningModalConfirmCallback(lv_event_t *event)
{
    lv_obj_t *obj = lv_event_get_user_data(event);
    lv_obj_t *current_container = lv_obj_get_user_data(obj);
    set_channel_current_by_force(current_container, 51);
    lv_obj_del_async(obj);

}
