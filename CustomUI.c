//
// Created by 13651 on 2024/11/16.
//

#include "CustomUI.h"

lv_obj_t *main_scr;
lv_obj_t *scheme_scr;
lv_obj_t *calib_scr;
lv_obj_t *charging_scr;

UI_Channel ChannelA;
UI_Channel ChannelB;
UI_Channel ChannelC;
UI_Channel ChannelD;

lv_obj_t *lv_channel_a;
lv_obj_t *lv_channel_b;
lv_obj_t *lv_channel_c;
lv_obj_t *lv_channel_d;

SchemePage schemePages[UI_SCHEME_PAGE_NUM];
pSchemeSet page1[UI_SCHEME_SET_NUM_PER_PAGE];
pSchemeSet page2[UI_SCHEME_SET_NUM_PER_PAGE];
pSchemeSet page3[UI_SCHEME_SET_NUM_PER_PAGE];
uint8_t current_page_num = 0;


void Channel_Struct_Init(UI_Channel *channel, char *name, uint8_t index) {
    channel->name = name;
    channel->index = index;
//    channel->p_uiplan = NULL;
    channel->pPlan = NULL;
    channel->timer.timer_sec = 0;
    channel->timer.timer_min = 0;
    channel->timer.state = UI_TIMER_STATE_STOP;
    channel->state = UI_CHANNEL_STATE_NOT_ADDED;
}

//void button_event_cb(lv_event_t *event) {
//    lv_event_code_t code = lv_event_get_code(event);
//    if (code == LV_EVENT_CLICKED) {
//        LV_LOG_USER("Button clicked");
//    }
//}

void addCallbackforImgBtn(lv_obj_t *btn) {
    lv_obj_add_event_cb(btn, ImgBtnPressedCallback, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(btn, ImgBtnReleasedCallback, LV_EVENT_RELEASED, NULL);
}

lv_obj_t *create_current_warning_modal(lv_obj_t *current_container)
{
    lv_obj_t *modal_container = lv_obj_create(lv_scr_act());
    lv_obj_set_size(modal_container, lv_obj_get_width(lv_scr_act()), lv_obj_get_height(lv_scr_act()));
    lv_obj_set_style_bg_opa(modal_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_opa(modal_container, LV_OPA_TRANSP, 0);
    lv_obj_set_user_data(modal_container, current_container);

    lv_obj_t *modal_bg = lv_obj_create(modal_container);
    lv_obj_set_size(modal_bg, 292, 178);
    lv_obj_set_style_bg_color(modal_bg, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(modal_bg, LV_OPA_80, 0);
    lv_obj_set_style_border_width(modal_bg, 0, 0);
    lv_obj_set_style_pad_all(modal_bg, 0, 0);
    lv_obj_center(modal_bg);
    lv_obj_add_flag(modal_bg, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t *modal_title = lv_obj_create(modal_bg);
    lv_obj_set_size(modal_title, 292, 70);
    lv_obj_set_style_bg_opa(modal_title, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(modal_title, 0, 0);

    lv_obj_t *warning_icon = lv_img_create(modal_title);
    lv_img_set_src(warning_icon, &WarningIcon_fit);
    lv_obj_align(warning_icon, LV_ALIGN_OUT_LEFT_MID, 0, 0);

    lv_obj_t *modal_title_label = lv_label_create(modal_title);
    lv_label_set_text(modal_title_label, "提示");
    lv_obj_set_style_text_color(modal_title_label, lv_color_white(), 0);
    lv_obj_set_style_text_font(modal_title_label, &AliPuHui_30, 0);
    lv_obj_align_to(modal_title_label, warning_icon, LV_ALIGN_OUT_RIGHT_MID, 10, 0);

    lv_obj_t *modal_content = lv_obj_create(modal_bg);
    lv_obj_set_size(modal_content, 292, LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(modal_content, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(modal_content, 0, 0);
    lv_obj_center(modal_content);

    lv_obj_t *modal_content_label = lv_label_create(modal_content);
    lv_label_set_text(modal_content_label, "电流已超过50\n是否继续上调？");
    lv_obj_set_style_text_color(modal_content_label, lv_color_white(), 0);
    lv_obj_set_style_text_font(modal_content_label, &AliPuHui_24, 0);
    lv_obj_center(modal_content_label);

    lv_obj_t *modal_btn_container = lv_obj_create(modal_bg);
    lv_obj_set_size(modal_btn_container, 292, 60);
    lv_obj_set_style_bg_opa(modal_btn_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_opa(modal_btn_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(modal_btn_container, 0, 0);
    lv_obj_align(modal_btn_container, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_clear_flag(modal_btn_container, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *modal_btn_cancel_label = lv_label_create(modal_btn_container);
    lv_label_set_text(modal_btn_cancel_label, "取消");
    lv_obj_set_style_text_color(modal_btn_cancel_label, lv_color_white(), 0);
    lv_obj_set_style_text_font(modal_btn_cancel_label, &AliPuHui_24, 0);
    lv_obj_align(modal_btn_cancel_label, LV_ALIGN_LEFT_MID, 30, 0);
    lv_obj_add_event_cb(modal_btn_cancel_label, CurrentWarningModalCancelCallback, LV_EVENT_CLICKED, modal_container);
    lv_obj_add_flag(modal_btn_cancel_label, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t *modal_btn_confirm_label = lv_label_create(modal_btn_container);
    lv_label_set_text(modal_btn_confirm_label, "继续");
    lv_obj_set_style_text_color(modal_btn_confirm_label, lv_color_white(), 0);
    lv_obj_set_style_text_font(modal_btn_confirm_label, &AliPuHui_24, 0);
    lv_obj_align(modal_btn_confirm_label, LV_ALIGN_RIGHT_MID, -30, 0);
    lv_obj_add_event_cb(modal_btn_confirm_label, CurrentWarningModalConfirmCallback, LV_EVENT_CLICKED, modal_container);
    lv_obj_add_flag(modal_btn_confirm_label, LV_OBJ_FLAG_CLICKABLE);

    return modal_container;
}

void set_channel_current_by_force(lv_obj_t *current_container, uint8_t current)
{
    lv_obj_t *channel = lv_obj_get_parent(current_container);
    UI_Channel *ui_ch = (UI_Channel *) lv_obj_get_user_data(channel);
    ui_ch->pPlan->current_mA = current;
    if (ui_ch->state == UI_CHANNEL_STATE_ADDED)
    {
        lv_obj_t *current_arc = lv_obj_get_child(current_container, 2);
        lv_arc_set_value(current_arc, current);
        lv_obj_t *current_label = lv_obj_get_child(current_arc, 0);
        lv_label_set_text_fmt(current_label, "%d", current);
    }
}

void refresh_channel_current(lv_obj_t *current_container, int8_t difference)
{
    lv_obj_t *channel = lv_obj_get_parent(current_container);
    UI_Channel *ui_ch = (UI_Channel *) lv_obj_get_user_data(channel);
    int8_t current = ui_ch->pPlan->current_mA;
    if (current + difference < 0)
        current = 0;
    else if (current + difference == 51 && difference > 0)
    {
        create_current_warning_modal(current_container);
    }
    else if (current + difference > 100)
        current = 100;
    else
        current += difference;
    ui_ch->pPlan->current_mA = current;
//    ui_ch->pPlan->g_chanelConfig[ui_ch->index].configParameter.current = current;
    if (ui_ch->state == UI_CHANNEL_STATE_ADDED)
    {
        lv_obj_t *current_arc = lv_obj_get_child(current_container, 2);
        lv_arc_set_value(current_arc, current);
        lv_obj_t *current_label = lv_obj_get_child(current_arc, 0);
        lv_label_set_text_fmt(current_label, "%d", current);
    }
}

void set_channel_timer_state(lv_obj_t *channel, UI_ChannelTimerState state)
{
    UI_Channel *ch = (UI_Channel *) lv_obj_get_user_data(channel);
    lv_obj_t *timer_container = lv_obj_get_child(channel, 1);
    ch->timer.state = state;
    if (ch->timer.state == UI_TIMER_STATE_STOP)
    {
        lv_obj_set_style_bg_color(timer_container, lv_color_hex(0xEFF4FE), 0);
        //TODO: Implement timer stop logic
    }
    else if (ch->timer.state == UI_TIMER_STATE_START)
    {
        lv_obj_set_style_bg_color(timer_container, lv_color_hex(0xA9EAE3), 0);
        //TODO: Implement timer start logic
    }
}

void set_channel_state(lv_obj_t *channel, UI_ChannelState state) {
    UI_Channel *ch = (UI_Channel *) lv_obj_get_user_data(channel);
    ch->prev_state = ch->state;
    ch->state = state;
    if (state == UI_CHANNEL_STATE_DISABLED) {
        if (ch->prev_state == UI_CHANNEL_STATE_ADDED)
        {
            lv_obj_t *current_adjust_container = lv_obj_get_child(channel, 2);
            lv_obj_t *add_btn = lv_obj_get_child(current_adjust_container, 0);
            lv_obj_t *sub_btn = lv_obj_get_child(current_adjust_container, 1);
            lv_obj_clear_flag(add_btn, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(sub_btn, LV_OBJ_FLAG_CLICKABLE);
        }
        else if (ch->prev_state == UI_CHANNEL_STATE_NOT_ADDED)
        {
            lv_obj_t *name_label = lv_obj_get_child(channel, 0);
            lv_obj_set_style_text_color(name_label, lv_color_hex(0xCDCDCD), 0);

            lv_obj_t *add_scheme_btn = lv_obj_get_child(lv_obj_get_child(channel, 1), 0);
            lv_obj_clear_flag(add_scheme_btn, LV_OBJ_FLAG_CLICKABLE);

            lv_obj_t *add_scheme_btn_label = lv_obj_get_child(add_scheme_btn, 0);
            lv_obj_set_style_text_color(add_scheme_btn_label, lv_color_hex(0xCDCDCD), 0);

        }

    }
    else if (state == UI_CHANNEL_STATE_NOT_ADDED) {
        lv_obj_clean(channel);
        lv_obj_set_style_bg_color(channel, lv_color_hex(0xf5f8ff), 0);

        //----- channel name -----
        lv_obj_t *label = lv_label_create(channel);
        lv_label_set_text(label, ch->name);
        lv_obj_set_style_text_font(label, &AliPuHui_20, 0);
        lv_obj_align(label, LV_ALIGN_TOP_LEFT, 10, 10);

        //----- add scheme button container -----
        lv_obj_t *add_scheme_container = lv_obj_create(channel);
        lv_obj_set_size(add_scheme_container, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
        lv_obj_set_style_pad_all(add_scheme_container, 0, 0);
        lv_obj_set_style_bg_opa(add_scheme_container, LV_OPA_TRANSP, 0);
        lv_obj_align(add_scheme_container, LV_ALIGN_CENTER, 0, 10);

        //----- add scheme button -----
        lv_obj_t *add_scheme_btn = lv_btn_create(add_scheme_container);
        lv_obj_remove_style_all(add_scheme_btn);
        lv_obj_set_style_bg_opa(add_scheme_btn, LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_opa(add_scheme_btn, LV_OPA_COVER, 0);
        lv_obj_set_size(add_scheme_btn, 170, 60);
        lv_obj_align(add_scheme_btn, LV_ALIGN_CENTER, 0, 0);
        lv_obj_add_event_cb(add_scheme_btn, AddSchemeBtnCallback, LV_EVENT_CLICKED, NULL);

        //----- scheme button label -----
        lv_obj_t *add_scheme_label = lv_label_create(add_scheme_btn);
        lv_label_set_text(add_scheme_label, "+ 添加方案");
        lv_obj_set_style_text_font(add_scheme_label, &AliPuHui_20, 0);
        lv_obj_set_style_text_color(add_scheme_label, lv_color_hex(0x1e827e), 0);
        lv_obj_align(add_scheme_label, LV_ALIGN_CENTER, 0, 0);

    }
    else if (state == UI_CHANNEL_STATE_ADDED) {
        if (ch->prev_state == UI_CHANNEL_STATE_DISABLED)
        {
            lv_obj_t *current_adjust_container = lv_obj_get_child(channel, 2);
            lv_obj_t *add_btn = lv_obj_get_child(current_adjust_container, 0);
            lv_obj_t *sub_btn = lv_obj_get_child(current_adjust_container, 1);
            lv_obj_add_flag(add_btn, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(sub_btn, LV_OBJ_FLAG_CLICKABLE);
            return;
        }
        lv_obj_clean(channel);
        lv_obj_set_style_bg_color(channel, lv_color_hex(0xffffff), 0);

        //----- channel name -----
        lv_obj_t *label = lv_label_create(channel);
        lv_label_set_text(label, ch->name);
        lv_obj_set_style_text_font(label, &AliPuHui_20, 0);
        lv_obj_align(label, LV_ALIGN_TOP_LEFT, 10, 10);
        lv_obj_add_event_cb(label, ChannelLabelClickCallback, LV_EVENT_CLICKED, channel);
        lv_obj_add_flag(label, LV_OBJ_FLAG_CLICKABLE);

        //----- channel timer container -----
        lv_obj_t *timer_container = lv_obj_create(channel);
        lv_obj_set_style_bg_color(timer_container, lv_color_hex(0xeff4fe), 0);
        lv_obj_set_style_border_opa(timer_container, LV_OPA_TRANSP, 0);
        lv_obj_set_size(timer_container, 100, 50);
        lv_obj_align_to(timer_container, label, LV_ALIGN_OUT_BOTTOM_MID, 20, 10);
        lv_obj_set_style_radius(timer_container, LV_RADIUS_CIRCLE, 0);
        lv_obj_set_scrollbar_mode(timer_container, LV_SCROLLBAR_MODE_OFF);
        lv_obj_clear_flag(timer_container, LV_OBJ_FLAG_SCROLLABLE);

        //----- channel timer label -----
        lv_obj_t *timer_label = lv_label_create(timer_container);
        lv_obj_set_size(timer_label, 100, 50);
        lv_label_set_text_fmt(timer_label, "%02d:%02d", ch->timer.timer_min, ch->timer.timer_sec);
        lv_obj_set_style_text_font(timer_label, &lv_font_montserrat_30, 0);
        lv_obj_align(timer_label, LV_ALIGN_CENTER, 10, 8);
        lv_obj_add_event_cb(timer_label, TimerLabelClickCallback, LV_EVENT_CLICKED, channel);
        lv_obj_add_flag(timer_label, LV_OBJ_FLAG_CLICKABLE);

        //----- channel current adjustment container -----
        static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(2), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
        static lv_coord_t row_dsc[] = {LV_GRID_CONTENT, LV_GRID_TEMPLATE_LAST};
        lv_obj_t *adjust_container = lv_obj_create(channel);
        lv_obj_set_size(adjust_container, 280, LV_PCT(100));
        lv_obj_align(adjust_container, LV_ALIGN_RIGHT_MID, 0, 0);
        lv_obj_set_scrollbar_mode(adjust_container, LV_SCROLLBAR_MODE_OFF);
        lv_obj_clear_flag(adjust_container, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_style_bg_opa(adjust_container, LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_opa(adjust_container, LV_OPA_TRANSP, 0);

        //----- channel current add button -----
        lv_obj_t *add_btn = lv_imgbtn_create(adjust_container);
        lv_imgbtn_set_src(add_btn, LV_IMGBTN_STATE_RELEASED, NULL, &AddButton_White_120_fit, NULL);
        lv_obj_set_size(add_btn, AddButton_White_120_fit.header.w, AddButton_White_120_fit.header.h);
        lv_obj_align(add_btn, LV_ALIGN_BOTTOM_LEFT, 0, 0);
        lv_obj_add_event_cb(add_btn, AddCurrentBtnCallback, LV_EVENT_CLICKED, NULL);
        addCallbackforImgBtn(add_btn);


        //----- channel current subtract button -----
        lv_obj_t *sub_btn = lv_imgbtn_create(adjust_container);
        lv_imgbtn_set_src(sub_btn, LV_IMGBTN_STATE_RELEASED, NULL, &SubtractButton_White_120_fit, NULL);
        lv_obj_set_size(sub_btn, SubtractButton_White_120_fit.header.w, SubtractButton_White_120_fit.header.h);
        lv_obj_align(sub_btn, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
        lv_obj_add_event_cb(sub_btn, SubCurrentBtnCallback, LV_EVENT_CLICKED, NULL);
        addCallbackforImgBtn(sub_btn);
        //    lv_obj_set_grid_cell(sub_btn, LV_GRID_ALIGN_SPACE_BETWEEN, 2, 1, LV_GRID_ALIGN_CENTER, 0, 1);

        //----- channel current arc -----
        lv_obj_t *current_arc = lv_arc_create(adjust_container);
        lv_arc_set_bg_angles(current_arc, 0, 270);
        lv_arc_set_value(current_arc, ch->pPlan->current_mA);
        lv_arc_set_rotation(current_arc, 135);
        lv_obj_remove_style(current_arc, NULL, LV_PART_KNOB);
        lv_obj_clear_flag(current_arc, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_size(current_arc, 100, 100);
        lv_obj_align(current_arc, LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_style_arc_width(current_arc, UI_CURRENT_ARC_LINE_WIDTH, LV_PART_MAIN);
        lv_obj_set_style_arc_width(current_arc, UI_CURRENT_ARC_LINE_WIDTH, LV_PART_INDICATOR);

        //----- channel current value label -----
        lv_obj_t *current_label = lv_label_create(current_arc);
        lv_label_set_text_fmt(current_label, "%d", ch->pPlan->current_mA);
        lv_obj_set_style_text_font(current_label, &AliPuHui_40, 0);
        lv_obj_align(current_label, LV_ALIGN_CENTER, 0, 0);

        //----- channel current unit label -----
        lv_obj_t *ma_label = lv_label_create(current_arc);
        lv_label_set_text(ma_label, "mA");
        lv_obj_set_style_text_font(ma_label, &AliPuHui_20, 0);
        lv_obj_align_to(ma_label, current_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    }
    else if (state == UI_CHANNEL_STATE_DROPPED) {
        lv_obj_t *modal_bg = lv_obj_create(channel);
//        lv_obj_set_style_pad_all(channel, 0, 0);
        lv_obj_set_size(modal_bg, lv_obj_get_width(channel), lv_obj_get_height(channel));
        lv_obj_center(modal_bg);
        lv_obj_set_style_bg_color(modal_bg, lv_color_black(), 0);
        lv_obj_set_style_bg_opa(modal_bg, LV_OPA_80, 0);
        lv_obj_set_style_border_width(modal_bg, 0, 0);
        lv_obj_set_style_pad_all(modal_bg, 0, 0);
        lv_obj_set_style_radius(modal_bg, 8, 0);

        lv_obj_t *warning_icon = lv_img_create(modal_bg);
        lv_img_set_src(warning_icon, &WarningIcon_fit);
        lv_obj_align(warning_icon, LV_ALIGN_CENTER, -70, 0);

        lv_obj_t *label = lv_label_create(modal_bg);
        lv_label_set_text(label, "电极片脱落");
        lv_obj_align(label, LV_ALIGN_CENTER, 20, 0);
        lv_obj_set_style_text_font(label, &AliPuHui_24, 0);
        lv_obj_set_style_text_color(label, lv_color_white(), 0);

        lv_obj_t *obj = lv_obj_create(modal_bg);
        lv_obj_set_size(obj,LV_PCT(100),LV_PCT(100));
        lv_obj_set_style_bg_opa(obj, LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_opa(obj, LV_OPA_COVER, 0);
        lv_obj_align(obj, LV_ALIGN_CENTER, 0, 0);
        lv_obj_add_event_cb(obj, DropModalDelCallback, LV_EVENT_CLICKED, channel);
    }
}

void clear_all_channels()
{
    Channel_Struct_Init(&ChannelA, "通道A", 0);
    Channel_Struct_Init(&ChannelB, "通道B", 1);
    Channel_Struct_Init(&ChannelC, "通道C", 2);
    Channel_Struct_Init(&ChannelD, "通道D", 3);

    set_channel_state(lv_channel_a, UI_CHANNEL_STATE_NOT_ADDED);
    set_channel_state(lv_channel_b, UI_CHANNEL_STATE_NOT_ADDED);
    set_channel_state(lv_channel_c, UI_CHANNEL_STATE_NOT_ADDED);
    set_channel_state(lv_channel_d, UI_CHANNEL_STATE_NOT_ADDED);
}

lv_obj_t *get_channel_by_index(uint8_t index)
{
    lv_obj_t *container = lv_obj_get_child(main_scr, 5);
    return lv_obj_get_child(container, index);
}

lv_obj_t *create_channel(lv_obj_t *parent, UI_Channel *ch) {
    //----- channel container -----
    lv_obj_t *channel = lv_obj_create(parent);
    lv_obj_set_size(channel, 420, 50);
    lv_obj_clear_flag(channel, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_opa(channel, LV_OPA_TRANSP, 0);
    lv_obj_set_flex_grow(channel, 1);

    lv_obj_set_user_data(channel, ch);
    set_channel_state(channel, UI_CHANNEL_STATE_NOT_ADDED);
    return channel;
}

void set_battery_level(uint8_t soc)
{
    lv_obj_t *scr = lv_disp_get_scr_act(NULL);
    lv_obj_t *top_bar = lv_obj_get_child(scr, 0);
    lv_obj_t *empty_battery = lv_obj_get_child(top_bar, 1);
    lv_obj_t *battery_level1 = lv_obj_get_child(empty_battery, 0);
    lv_obj_t *battery_level2 = lv_obj_get_child(empty_battery, 1);
    lv_obj_t *battery_level3 = lv_obj_get_child(empty_battery, 2);
    lv_obj_t *battery_level4 = lv_obj_get_child(empty_battery, 3);

    if (75 < soc && soc <= 100) {
        lv_obj_clear_flag(battery_level1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(battery_level2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(battery_level3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(battery_level4, LV_OBJ_FLAG_HIDDEN);
    }
    else if (50 < soc && soc <= 75) {
        lv_obj_clear_flag(battery_level1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(battery_level2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(battery_level3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(battery_level4, LV_OBJ_FLAG_HIDDEN);
    }
    else if (25 < soc && soc <= 50) {
        lv_obj_clear_flag(battery_level1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(battery_level2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(battery_level3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(battery_level4, LV_OBJ_FLAG_HIDDEN);
    }
    else if (5 < soc && soc <= 25) {
        lv_obj_clear_flag(battery_level1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(battery_level2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(battery_level3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(battery_level4, LV_OBJ_FLAG_HIDDEN);
    }
    else {
        lv_obj_add_flag(battery_level1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(battery_level2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(battery_level3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(battery_level4, LV_OBJ_FLAG_HIDDEN);
    }
}

lv_obj_t *create_top_bar(lv_obj_t *scr, bool is_main_scr)
{

    lv_obj_t *top_bar = lv_obj_create(scr);
    lv_obj_set_size(top_bar, lv_obj_get_width(scr), 60);
    lv_obj_align(top_bar, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_bg_opa(top_bar, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_opa(top_bar, LV_OPA_TRANSP, 0);
    lv_obj_clear_flag(top_bar, LV_OBJ_FLAG_SCROLLABLE);
    if (is_main_scr)
    {
        lv_obj_t *child_lock_btn = lv_imgbtn_create(top_bar);
        lv_imgbtn_set_src(child_lock_btn, LV_IMGBTN_STATE_RELEASED, NULL, &LockIconTransparent_fit, NULL);
        lv_obj_set_size(child_lock_btn, LockIconTransparent_fit.header.w, LockIconTransparent_fit.header.h);
        lv_obj_align(child_lock_btn, LV_ALIGN_LEFT_MID, 0, 0);
        addCallbackforImgBtn(child_lock_btn);
        lv_obj_add_event_cb(child_lock_btn, ChildLockBtnCallback, LV_EVENT_CLICKED, NULL);
        static bool is_locked = false;
        lv_obj_set_user_data(child_lock_btn, &is_locked);
    }
    else
    {
        lv_obj_t *back_btn = lv_imgbtn_create(top_bar);
        lv_imgbtn_set_src(back_btn, LV_IMGBTN_STATE_RELEASED, NULL, &BackButton_fit, NULL);
        lv_obj_set_size(back_btn, BackButton_fit.header.w, BackButton_fit.header.h);
        lv_obj_align(back_btn, LV_ALIGN_LEFT_MID, 0, 0);
        addCallbackforImgBtn(back_btn);
        lv_obj_add_event_cb(back_btn, BackBtnCallback, LV_EVENT_CLICKED, NULL);
    }
    lv_obj_t *battery_icon = lv_img_create(top_bar);
    lv_img_set_src(battery_icon, &EmptyBattery_fit);
    lv_obj_align(battery_icon, LV_ALIGN_RIGHT_MID, 0, 0);

    lv_obj_t *battery_level_1 = lv_img_create(battery_icon);
    lv_img_set_src(battery_level_1, &BatteryBar_fit);
    lv_obj_align(battery_level_1, LV_ALIGN_LEFT_MID, 3, 0);

    lv_obj_t *battery_level_2 = lv_img_create(battery_icon);
    lv_img_set_src(battery_level_2, &BatteryBar_fit);
    lv_obj_align_to(battery_level_2, battery_level_1, LV_ALIGN_OUT_RIGHT_MID, 2, 0);

    lv_obj_t *battery_level_3 = lv_img_create(battery_icon);
    lv_img_set_src(battery_level_3, &BatteryBar_fit);
    lv_obj_align_to(battery_level_3, battery_level_2, LV_ALIGN_OUT_RIGHT_MID, 2, 0);

    lv_obj_t *battery_level_4 = lv_img_create(battery_icon);
    lv_img_set_src(battery_level_4, &BatteryBar_fit);
    lv_obj_align_to(battery_level_4, battery_level_3, LV_ALIGN_OUT_RIGHT_MID, 2, 0);

    lv_obj_t *bluetooth_btn = lv_imgbtn_create(top_bar);
    lv_imgbtn_set_src(bluetooth_btn, LV_IMGBTN_STATE_RELEASED, NULL, &BluetoothIcon_fit, NULL);
    lv_obj_set_size(bluetooth_btn, BluetoothIcon_fit.header.w, BluetoothIcon_fit.header.h);
    lv_obj_align_to(bluetooth_btn, battery_icon, LV_ALIGN_OUT_LEFT_MID, -15, 0);
    lv_obj_add_event_cb(bluetooth_btn, BluetoothBtnCallback, LV_EVENT_CLICKED, NULL);
    addCallbackforImgBtn(bluetooth_btn);

    lv_obj_t *calib_btn = lv_imgbtn_create(top_bar);
    lv_imgbtn_set_src(calib_btn, LV_IMGBTN_STATE_RELEASED, NULL, &CalibIcon_fit, NULL);
    lv_obj_set_size(calib_btn, CalibIcon_fit.header.w, CalibIcon_fit.header.h);
    lv_obj_align_to(calib_btn, bluetooth_btn, LV_ALIGN_OUT_LEFT_MID, -15, 0);
    lv_obj_add_event_cb(calib_btn, CalibBtnCallback, LV_EVENT_CLICKED, NULL);
    addCallbackforImgBtn(calib_btn);

    lv_obj_t *setting_btn = lv_imgbtn_create(top_bar);
    lv_imgbtn_set_src(setting_btn, LV_IMGBTN_STATE_RELEASED, NULL, &SettingIcon_fit, NULL);
    lv_obj_set_size(setting_btn, SettingIcon_fit.header.w, SettingIcon_fit.header.h);
    lv_obj_align_to(setting_btn, calib_btn, LV_ALIGN_OUT_LEFT_MID, -15, 0);
    addCallbackforImgBtn(setting_btn);

    return top_bar;
}

lv_obj_t *create_main_scr() {
    lv_obj_t *scr = lv_img_create(NULL);
    lv_img_set_src(scr, &BG2_fit);
    lv_obj_align(scr, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *top_bar = create_top_bar(scr, true);

    lv_obj_t *clear_btn = lv_imgbtn_create(scr);
    lv_imgbtn_set_src(clear_btn, LV_IMGBTN_STATE_RELEASED, NULL, &Clear_fit, NULL);
    lv_obj_set_size(clear_btn, Clear_fit.header.w, Clear_fit.header.h);
    lv_obj_align(clear_btn, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    lv_obj_add_event_cb(clear_btn, ClearBtnCallback, LV_EVENT_CLICKED, NULL);
    addCallbackforImgBtn(clear_btn);

    lv_obj_t *sync_adjust_btn = lv_imgbtn_create(scr);
    lv_imgbtn_set_src(sync_adjust_btn, LV_IMGBTN_STATE_RELEASED, NULL, &SyncAdjust_fit, NULL);
    lv_obj_set_size(sync_adjust_btn, SyncAdjust_fit.header.w, SyncAdjust_fit.header.h);
    lv_obj_align(sync_adjust_btn, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    lv_obj_add_event_cb(sync_adjust_btn, SyncAdjustBtnCallback, LV_EVENT_CLICKED, NULL);
    addCallbackforImgBtn(sync_adjust_btn);

    lv_obj_t *stimulation_start_btn = lv_imgbtn_create(scr);
    lv_imgbtn_set_src(stimulation_start_btn, LV_IMGBTN_STATE_RELEASED, NULL, &StartButton_Green_fit, NULL);
    lv_imgbtn_set_src(stimulation_start_btn, LV_IMGBTN_STATE_DISABLED, NULL, &StartButton_Gray_fit, NULL);
    lv_obj_align(stimulation_start_btn, LV_ALIGN_BOTTOM_MID, 0, -50);
    lv_obj_set_size(stimulation_start_btn, StartButton_Green_fit.header.w, StartButton_Green_fit.header.h);
    addCallbackforImgBtn(stimulation_start_btn);
    static bool is_stimulation_running = false;
    lv_obj_set_user_data(stimulation_start_btn, &is_stimulation_running);
    lv_obj_add_event_cb(stimulation_start_btn, StimulationStartBtnCallback, LV_EVENT_CLICKED, NULL);

    lv_obj_t *stimulation_start_label = lv_label_create(scr);
    lv_label_set_text(stimulation_start_label, "全部开始");
    lv_obj_set_style_text_font(stimulation_start_label, &AliPuHui_20, 0);
    lv_obj_align(stimulation_start_label, LV_ALIGN_BOTTOM_MID, 0, -25);

    lv_obj_t *container = lv_obj_create(scr);
    lv_obj_set_size(container, LV_SIZE_CONTENT, 660);
    lv_obj_align(container, LV_ALIGN_TOP_MID, 0, 60);
    lv_obj_set_style_bg_color(container, lv_color_hex(0xFF0000), 0);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN_WRAP);

    Channel_Struct_Init(&ChannelA, "通道A", 0);
    Channel_Struct_Init(&ChannelB, "通道B", 1);
    Channel_Struct_Init(&ChannelC, "通道C", 2);
    Channel_Struct_Init(&ChannelD, "通道D", 3);

    lv_channel_a = create_channel(container, &ChannelA);
    lv_channel_b = create_channel(container, &ChannelB);
    lv_channel_c = create_channel(container, &ChannelC);
    lv_channel_d = create_channel(container, &ChannelD);

    lv_obj_set_style_bg_opa(container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_opa(container, LV_OPA_TRANSP, 0);
    return scr;
}

lv_obj_t *create_scheme_set(lv_obj_t *parent, SchemeSet *schemeSet) {
    //----- Scheme Dropdown -----
    lv_obj_t *scheme_set_dropdown = lv_dropdown_create(parent);
    lv_obj_set_size(scheme_set_dropdown, LV_PCT(100), 80);
    lv_obj_align(scheme_set_dropdown, LV_ALIGN_CENTER, 0, 0);
    lv_dropdown_set_text(scheme_set_dropdown, "");
    lv_dropdown_set_symbol(scheme_set_dropdown, NULL);
    lv_dropdown_clear_options(scheme_set_dropdown);
    lv_obj_set_flex_flow(scheme_set_dropdown, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(scheme_set_dropdown, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_add_event_cb(scheme_set_dropdown, SchemeDropdownCallback, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_set_user_data(scheme_set_dropdown, schemeSet);

    //----- Scheme Tag Bullet Point -----
    lv_obj_t *title_bullet_point = lv_label_create(scheme_set_dropdown);
    lv_label_set_text(title_bullet_point, "•");
    lv_obj_set_style_text_font(title_bullet_point, &AliPuHui_40, 0);
    lv_obj_set_style_text_color(title_bullet_point, lv_color_hex(0x8c9bbc), 0);

    //----- Scheme Tag -----
    lv_obj_t *scheme_tag = lv_label_create(scheme_set_dropdown);
    lv_label_set_text(scheme_tag, schemeSet->name);
    lv_obj_set_style_text_font(scheme_tag, &AliPuHui_30, 0);

    // ----- Scheme Dropdown List -----
    char option_str[256] = {0};
    lv_obj_t *scheme_list = lv_dropdown_get_list(scheme_set_dropdown);
    lv_obj_set_style_text_font(scheme_list, &AliPuHui_20, 0);
    for (int i = 0; i < schemeSet->plan_num; i++) {
//        sprintf(option_str, "%s\n", schemeSet->plans[i].name);
        lv_dropdown_add_option(scheme_set_dropdown, schemeSet->plans[i]->name, i);
    }


    return scheme_set_dropdown;
}

void init_scheme_pages()
{
//    page1[6] = {schemeSet1, schemeSet2, schemeSet3, schemeSet4, schemeSet5, schemeSet6};
    page1[0] = &schemeSet1;
    page1[1] = &schemeSet2;
    page1[2] = &schemeSet3;
    page1[3] = &schemeSet4;
    page1[4] = &schemeSet5;
    page1[5] = &schemeSet6;

//    page2[6] = {schemeSet7, schemeSet8, schemeSet9, schemeSet10, schemeSet11, schemeSet12};
    page2[0] = &schemeSet7;
    page2[1] = &schemeSet8;
    page2[2] = &schemeSet9;
    page2[3] = &schemeSet10;
    page2[4] = &schemeSet11;
    page2[5] = &schemeSet12;

//    page3[6] = {schemeSet1, schemeSet2, schemeSet3, schemeSet4, schemeSet5};
    page3[0] = &schemeSet1;
    page3[1] = &schemeSet2;
    page3[2] = &schemeSet3;
    page3[3] = &schemeSet4;
    page3[4] = &schemeSet5;
}

void set_scheme_set_page(lv_obj_t *container, uint8_t page)
{
    page = page % 3;
    lv_obj_clean(container);
    SchemePage current_page = schemePages[page];
    for (uint8_t i = 0; i < UI_SCHEME_SET_NUM_PER_PAGE; ++i) {
        if (current_page[i])
            create_scheme_set(container, current_page[i]);
    }

}

lv_obj_t *create_scheme_scr() {
    lv_obj_t *scr = lv_img_create(NULL);
    lv_img_set_src(scr, &BG3_fit);

    lv_obj_t *top_bar = create_top_bar(scr, false);

    //----- Scheme Container -----
    lv_obj_t *scheme_container = lv_obj_create(scr);
    lv_obj_set_size(scheme_container, 450, 760);
    lv_obj_align(scheme_container, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_opa(scheme_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_opa(scheme_container, LV_OPA_TRANSP, 0);


    //----- Scheme Title -----
    lv_obj_t *scheme_title = lv_label_create(scheme_container);
    lv_label_set_text(scheme_title, "方案选择");
    lv_obj_set_style_text_font(scheme_title, &AliPuHui_30, 0);
    lv_obj_align(scheme_title, LV_ALIGN_TOP_LEFT, 0, 0);

    //----- Scheme List Container -----
    lv_obj_t *scheme_set_list_container = lv_obj_create(scheme_container);
    lv_obj_set_size(scheme_set_list_container, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(scheme_set_list_container, LV_FLEX_FLOW_COLUMN_WRAP);
    lv_obj_align(scheme_set_list_container, LV_ALIGN_TOP_MID, 0, 40);
    lv_obj_set_style_bg_opa(scheme_set_list_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_opa(scheme_set_list_container, LV_OPA_TRANSP, 0);

    init_scheme_pages();

    schemePages[0] = page1;
    schemePages[1] = page2;
    schemePages[2] = page3;
    set_scheme_set_page(scheme_set_list_container, current_page_num);

    //----- Page Indicator Container -----
    lv_obj_t *page_indicator_container = lv_obj_create(scheme_container);
    lv_obj_set_size(page_indicator_container, LV_PCT(100), 50);
    lv_obj_align(page_indicator_container, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_flex_flow(page_indicator_container, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(page_indicator_container, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_opa(page_indicator_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_opa(page_indicator_container, LV_OPA_TRANSP, 0);
    lv_obj_set_scrollbar_mode(page_indicator_container, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(page_indicator_container, LV_OBJ_FLAG_SCROLLABLE);

    //----- Previous Page Button -----
    lv_obj_t *prev_page_btn = lv_btn_create(page_indicator_container);
    lv_obj_set_size(prev_page_btn, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_style_bg_color(prev_page_btn, lv_color_hex(0x2ea69b), 0);

    lv_obj_t *prev_page_label = lv_label_create(prev_page_btn);
    lv_label_set_text(prev_page_label, "上一页");
    lv_obj_set_style_text_font(prev_page_label, &AliPuHui_20, 0);
    lv_obj_add_event_cb(prev_page_btn, PrevPageBtnCallback, LV_EVENT_CLICKED, NULL);
    lv_obj_set_user_data(prev_page_btn, scheme_set_list_container);

    //----- Current Page Label -----
    lv_obj_t *current_page_label = lv_label_create(page_indicator_container);
    lv_label_set_text_fmt(current_page_label, "第%d:%d页", current_page_num + 1, UI_SCHEME_PAGE_NUM);
    lv_obj_set_style_text_font(current_page_label, &AliPuHui_20, 0);

    //----- Next Page Button -----
    lv_obj_t *next_page_btn = lv_btn_create(page_indicator_container);
    lv_obj_set_size(next_page_btn, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_style_bg_color(next_page_btn, lv_color_hex(0x2ea69b), 0);

    lv_obj_t *next_page_label = lv_label_create(next_page_btn);
    lv_label_set_text(next_page_label, "下一页");
    lv_obj_set_style_text_font(next_page_label, &AliPuHui_20, 0);
    lv_obj_add_event_cb(next_page_btn, NextPageBtnCallback, LV_EVENT_CLICKED, NULL);
    lv_obj_set_user_data(next_page_btn, scheme_set_list_container);

    return scr;
}

lv_obj_t *create_calib_scr()
{
    lv_obj_t *scr = lv_img_create(NULL);
    lv_img_set_src(scr, &BG3_fit);

    lv_obj_t *top_bar = create_top_bar(scr, false);

    lv_obj_t *posture_demo_img = lv_img_create(scr);
    lv_img_set_src(posture_demo_img, &PostureDemo_fit);
//    lv_obj_set_size(posture_demo_img, 450, 760);
    lv_obj_align(posture_demo_img, LV_ALIGN_CENTER, 0, -50);

    lv_obj_t *progress_bar = lv_bar_create(scr);
    lv_obj_set_size(progress_bar, 300, 20);
    lv_obj_align(progress_bar, LV_ALIGN_CENTER, 0, 100);
    static lv_grad_dsc_t grad_dsc;
    grad_dsc.dir = LV_GRAD_DIR_HOR;
    grad_dsc.stops_count = 2;
    grad_dsc.stops[0].color = lv_color_hex(0xC7E1E1);
    grad_dsc.stops[1].color = lv_color_hex(0x319E94);
    grad_dsc.stops[0].frac = 0;
    grad_dsc.stops[1].frac = 255;
    lv_bar_set_range(progress_bar, 0, 100);
    lv_bar_set_value(progress_bar, 80, LV_ANIM_OFF);
    lv_obj_set_style_bg_grad(progress_bar, &grad_dsc, LV_PART_INDICATOR);
    lv_obj_add_event_cb(progress_bar, ProgressBarIndicatorCallback, LV_EVENT_DRAW_PART_END, NULL);

    lv_obj_t *label = lv_label_create(scr);
    lv_label_set_text(label, "第一次");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 160);
    lv_obj_set_style_text_font(label, &AliPuHui_30, 0);

    lv_obj_t *start_btn_container = lv_obj_create(scr);
    lv_obj_set_size(start_btn_container, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_align(start_btn_container, LV_ALIGN_BOTTOM_MID, 0, -150);
    lv_obj_set_style_bg_opa(start_btn_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_opa(start_btn_container, LV_OPA_TRANSP, 0);

    lv_obj_t *calib_start_btn = lv_imgbtn_create(start_btn_container);
    lv_imgbtn_set_src(calib_start_btn, LV_IMGBTN_STATE_RELEASED, NULL, &StartIconTransparent_fit, NULL);
    lv_obj_set_size(calib_start_btn, StartIconTransparent_fit.header.w, StartIconTransparent_fit.header.h);
    addCallbackforImgBtn(calib_start_btn);
    lv_obj_align(calib_start_btn, LV_ALIGN_LEFT_MID, 0, 0);

    lv_obj_t *calib_start_label = lv_label_create(start_btn_container);
    lv_label_set_text(calib_start_label, "开始校准");
    lv_obj_set_style_text_font(calib_start_label, &AliPuHui_30, 0);
    lv_obj_align_to(calib_start_label, calib_start_btn, LV_ALIGN_OUT_RIGHT_MID, 20, 0);

    return scr;
}

lv_obj_t *create_charging_scr()
{
    lv_obj_t *scr = lv_img_create(NULL);
    lv_img_set_src(scr, &BG1_fit);

    lv_obj_t *charging_ring = lv_img_create(scr);
    lv_img_set_src(charging_ring, &ChargeRing_fit);
    lv_obj_align(charging_ring, LV_ALIGN_CENTER, 0, -90);

    lv_obj_t *soc_label = lv_label_create(charging_ring);
    lv_label_set_text(soc_label, "80");
    lv_obj_set_style_text_font(soc_label, &AliPuHui_70, 0);
    lv_obj_align(soc_label, LV_ALIGN_CENTER, -3, 5);

    lv_obj_t *percent_label = lv_label_create(charging_ring);
    lv_label_set_text(percent_label, "%");
    lv_obj_set_style_text_font(percent_label, &lv_font_montserrat_30, 0);
    lv_obj_align_to(percent_label, soc_label, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, -10);

    lv_obj_t *lighting_icon = lv_img_create(scr);
    lv_img_set_src(lighting_icon, &LightningSymbol_fit);
    lv_obj_align(lighting_icon, LV_ALIGN_CENTER, 0, 178);

    lv_obj_t *charging_label = lv_label_create(scr);
    lv_label_set_text(charging_label, "正在充电");
    lv_obj_set_style_text_font(charging_label, &AliPuHui_30, 0);
    lv_obj_set_style_text_color(charging_label, lv_color_hex(0x9AC7C0), 0);
    lv_obj_align_to(charging_label, lighting_icon, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);

    return scr;
}

void CustomUI() {
    main_scr = create_main_scr();
    scheme_scr = create_scheme_scr();
    calib_scr = create_calib_scr();
    charging_scr = create_charging_scr();
//    lv_scr_load(charging_scr);
    lv_scr_load(main_scr);
}