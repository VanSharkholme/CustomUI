//
// Created by 13651 on 2024/11/19.
//

#ifndef LVGL_CALLBACKS_H
#define LVGL_CALLBACKS_H

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#include "CustomUI.h"


void ImgBtnPressedCallback(lv_event_t *event);
void ImgBtnReleasedCallback(lv_event_t *event);
void AddSchemeBtnCallback(lv_event_t *event);
void BackBtnCallback(lv_event_t *event);
void SchemeDropdownCallback(lv_event_t *event);
void ClearButtonCallback(lv_event_t *event);
void PrevPageBtnCallback(lv_event_t *event);
void NextPageBtnCallback(lv_event_t *event);


#endif //LVGL_CALLBACKS_H
