//
// Created by 13651 on 2024/11/16.
//

#include "CustomUI.h"

void button_event_cb(lv_event_t *event) {
    lv_event_code_t code = lv_event_get_code(event);
    if (code == LV_EVENT_CLICKED) {
        LV_LOG_USER("Button clicked");
    }
}

void addCallbackforImgBtn(lv_obj_t *btn)
{
    lv_obj_add_event_cb(btn, ImgBtnPressedCallback, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(btn, ImgBtnReleasedCallback, LV_EVENT_RELEASED, NULL);
}

lv_obj_t *create_channel(lv_obj_t *parent, char *name) {
    //----- channel container -----
    lv_obj_t *channel = lv_obj_create(parent);
    lv_obj_set_size(channel, 420, LV_SIZE_CONTENT);
    lv_obj_set_flex_grow(channel, 1);
//    lv_obj_set_style_bg_color(channel, lv_color_hex(0xFFFFFF), 0);

    //----- channel name -----
    lv_obj_t *label = lv_label_create(channel);
    lv_label_set_text(label, name);
    lv_obj_set_style_text_font(label, &AliPuHui_20, 0);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 10, 10);

    //----- channel timer container -----
    lv_obj_t *timer_container = lv_obj_create(channel);
    lv_obj_set_style_bg_color(timer_container, lv_color_hex(0xeff4fe), LV_STATE_USER_1);
    lv_obj_set_style_bg_color(timer_container, lv_color_hex(0xA9EAE3), LV_STATE_USER_2);
    lv_obj_add_state(timer_container, UI_TIMER_STOP_STATE);
    lv_obj_set_style_border_opa(timer_container, LV_OPA_TRANSP, 0);
    lv_obj_set_size(timer_container, 100, 50);
    lv_obj_align_to(timer_container, label, LV_ALIGN_OUT_BOTTOM_MID, 20, 10);
    lv_obj_set_style_radius(timer_container, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_scrollbar_mode(timer_container, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(timer_container, LV_OBJ_FLAG_SCROLLABLE);

    //----- channel timer label -----
    lv_obj_t *timer_label = lv_label_create(timer_container);
//    lv_label_set_text(timer_label, "00:00");
    lv_label_set_text_fmt(timer_label, "%02d:%02d", 0, 0);
    lv_obj_set_style_text_font(timer_label, &AliPuHui_30, 0);
    lv_obj_align(timer_label, LV_ALIGN_CENTER, 0, 0);

    //----- channel current adjustment container -----
    static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(2), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {LV_GRID_CONTENT, LV_GRID_TEMPLATE_LAST};
    lv_obj_t *adjust_container = lv_obj_create(channel);
//    lv_obj_set_grid_dsc_array(adjust_container, col_dsc, row_dsc);
    lv_obj_set_size(adjust_container, 280,LV_PCT(100));
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
    addCallbackforImgBtn(add_btn);

    //    lv_obj_set_grid_cell(add_btn, LV_GRID_ALIGN_SPACE_BETWEEN, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);

    //----- channel current subtract button -----
    lv_obj_t *sub_btn = lv_imgbtn_create(adjust_container);
    lv_imgbtn_set_src(sub_btn, LV_IMGBTN_STATE_RELEASED, NULL, &SubtractButton_White_120_fit, NULL);
    lv_obj_set_size(sub_btn, SubtractButton_White_120_fit.header.w, SubtractButton_White_120_fit.header.h);
    lv_obj_align(sub_btn, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    addCallbackforImgBtn(sub_btn);
    //    lv_obj_set_grid_cell(sub_btn, LV_GRID_ALIGN_SPACE_BETWEEN, 2, 1, LV_GRID_ALIGN_CENTER, 0, 1);

    //----- channel current arc -----
    lv_obj_t *current_arc = lv_arc_create(adjust_container);
    lv_arc_set_bg_angles(current_arc, 0, 270);
    lv_arc_set_value(current_arc, 20);
    lv_arc_set_rotation(current_arc, 135);
    lv_obj_remove_style(current_arc, NULL, LV_PART_KNOB);
    lv_obj_clear_flag(current_arc, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_size(current_arc, 100, 100);
    lv_obj_align(current_arc, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_arc_width(current_arc, UI_CURRENT_ARC_LINE_WIDTH, LV_PART_MAIN);
    lv_obj_set_style_arc_width(current_arc, UI_CURRENT_ARC_LINE_WIDTH, LV_PART_INDICATOR);
//    lv_obj_set_grid_cell(current_arc, LV_GRID_ALIGN_SPACE_BETWEEN, 1, 2, LV_GRID_ALIGN_CENTER, 0, 1);

    //----- channel current value label -----
    lv_obj_t *current_label = lv_label_create(current_arc);
    lv_label_set_text(current_label, "20");
    lv_obj_set_style_text_font(current_label, &AliPuHui_40, 0);
    lv_obj_align(current_label, LV_ALIGN_CENTER, 0, 0);

    //----- channel current unit label -----
    lv_obj_t *ma_label = lv_label_create(current_arc);
    lv_label_set_text(ma_label, "mA");
    lv_obj_set_style_text_font(ma_label, &AliPuHui_20, 0);
    lv_obj_align_to(ma_label, current_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    return channel;
}



void CustomUI() {

    static lv_style_t btn_pressed_style;
    lv_style_init(&btn_pressed_style);


    lv_obj_t *bg = lv_img_create(lv_scr_act());
    lv_img_set_src(bg, &BG2_fit);
    lv_obj_align(bg, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *clear_btn = lv_imgbtn_create(bg);
    lv_imgbtn_set_src(clear_btn, LV_IMGBTN_STATE_RELEASED, NULL, &Clear_fit, NULL);
    lv_obj_set_size(clear_btn, Clear_fit.header.w, Clear_fit.header.h);
    lv_obj_align(clear_btn, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    addCallbackforImgBtn(clear_btn);

    lv_obj_t *sync_adjust_btn = lv_imgbtn_create(bg);
    lv_imgbtn_set_src(sync_adjust_btn, LV_IMGBTN_STATE_RELEASED, NULL, &SyncAdjust_fit, NULL);
    lv_obj_set_size(sync_adjust_btn, SyncAdjust_fit.header.w, SyncAdjust_fit.header.h);
    lv_obj_align(sync_adjust_btn, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    addCallbackforImgBtn(sync_adjust_btn);

    lv_obj_t *stimulation_start_btn = lv_imgbtn_create(bg);
    lv_imgbtn_set_src(stimulation_start_btn, LV_IMGBTN_STATE_RELEASED, NULL, &StartButton_Green_fit, NULL);
    lv_imgbtn_set_src(stimulation_start_btn, LV_IMGBTN_STATE_DISABLED, NULL, &StartButton_Gray_fit, NULL);
    lv_obj_align(stimulation_start_btn, LV_ALIGN_BOTTOM_MID, 0, -50);
    lv_obj_set_size(stimulation_start_btn, StartButton_Green_fit.header.w, StartButton_Green_fit.header.h);
    addCallbackforImgBtn(stimulation_start_btn);

    lv_obj_t *stimulation_start_label = lv_label_create(bg);
    lv_label_set_text(stimulation_start_label, "全部开始");
    lv_obj_set_style_text_font(stimulation_start_label, &AliPuHui_20, 0);
    lv_obj_align(stimulation_start_label, LV_ALIGN_BOTTOM_MID, 0, -25);

    lv_obj_add_event_cb(clear_btn, button_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *container = lv_obj_create(bg);
    lv_obj_set_size(container, LV_SIZE_CONTENT, 660);
    lv_obj_align(container, LV_ALIGN_TOP_MID, 0, 60);
    lv_obj_set_style_bg_color(container, lv_color_hex(0xFF0000), 0);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN_WRAP);

    lv_obj_t *channel_a = create_channel(container, "通道A");
    lv_obj_t *channel_b = create_channel(container, "通道B");
    lv_obj_t *channel_c = create_channel(container, "通道C");
    lv_obj_t *channel_d = create_channel(container, "通道D");

    lv_obj_set_style_bg_opa(container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_opa(container, LV_OPA_TRANSP, 0);
}