//
// Created by 13651 on 2024/11/16.
//

#ifndef LVGL_CUSTOMUI_H
#define LVGL_CUSTOMUI_H

#include "lvgl/lvgl.h"
#include "stdio.h"
#include "Callbacks.h"

LV_IMG_DECLARE(AddButton_Green_fit)
LV_IMG_DECLARE(AddButton_White_120_fit)
LV_IMG_DECLARE(AddButton_White_140_fit)
LV_IMG_DECLARE(ArcProgress_fit)
LV_IMG_DECLARE(BatteryIcon_fit)
LV_IMG_DECLARE(BG1_fit)
LV_IMG_DECLARE(BG2_fit)
LV_IMG_DECLARE(BG3_fit)
LV_IMG_DECLARE(BluetoothIcon_fit)
LV_IMG_DECLARE(CancelButtonBig_fit)
LV_IMG_DECLARE(ChargeRing_fit)
LV_IMG_DECLARE(Clear_fit)
LV_IMG_DECLARE(ConfirmButtonBig_fit)
LV_IMG_DECLARE(HorizontalProgressBar_fit)
LV_IMG_DECLARE(LightningSymbol_fit)
LV_IMG_DECLARE(LockButton_Black_fit)
LV_IMG_DECLARE(LockIconTransparent_fit)
LV_IMG_DECLARE(MenuIcon_fit)
LV_IMG_DECLARE(PauseButton_fit)
LV_IMG_DECLARE(PauseIconTransparent_fit)
LV_IMG_DECLARE(PostureDemo_fit)
LV_IMG_DECLARE(StartButton_Gray_fit)
LV_IMG_DECLARE(StartButton_Green_fit)
LV_IMG_DECLARE(StartIconTransparent_fit)
LV_IMG_DECLARE(SubtractButton_fit)
LV_IMG_DECLARE(SubtractButton_White_120_fit)
LV_IMG_DECLARE(SubtractButton_White_140_fit)
LV_IMG_DECLARE(SyncAdjust_fit)
LV_IMG_DECLARE(WarningIcon_fit)
LV_IMG_DECLARE(BackButton_fit)

LV_FONT_DECLARE(AliPuHui_20)
LV_FONT_DECLARE(AliPuHui_24)
LV_FONT_DECLARE(AliPuHui_28)
LV_FONT_DECLARE(AliPuHui_30)
LV_FONT_DECLARE(AliPuHui_40)
LV_FONT_DECLARE(AliPuHui_50)

#define UI_TIMER_STOP_STATE LV_STATE_USER_1
#define UI_TIMER_START_STATE LV_STATE_USER_2

#define UI_CURRENT_ARC_LINE_WIDTH 6

#define UI_SCHEME_PAGE_NUM 3
#define UI_SCHEME_SET_NUM_PER_PAGE 6

typedef enum {
    UI_CHANNEL_STATE_DISABLED,
    UI_CHANNEL_STATE_NOT_ADDED,
    UI_CHANNEL_STATE_ADDED,
    UI_CHANNEL_STATE_DROPPED
} ChannelState;

typedef enum {
    UI_TIMER_STATE_STOP,
    UI_TIMER_STATE_START,
    UI_TIMER_STATE_PAUSE
} ChannelTimerState;

typedef struct {
    uint8_t timer_min;
    uint8_t timer_sec;
    ChannelTimerState state;
} ChannelTimer;

typedef struct {
    char *name;
    uint8_t current_mA;
    uint8_t timer_min;
    uint8_t timer_sec;
// todo: add more attributes
} Plan;

typedef struct {
    char *name;
    ChannelState state;
    ChannelTimer timer;
    Plan *plan;
} Channel;


typedef struct {
    char *name;
    Plan *plans;
    uint8_t plan_num;
} SchemeSet;

typedef SchemeSet * pSchemeSet;
typedef pSchemeSet * SchemePage;

extern lv_obj_t *main_scr;
extern lv_obj_t *scheme_scr;
extern SchemeSet schemeSet1;
extern SchemeSet schemeSet2;
extern SchemeSet schemeSet3;
extern SchemeSet schemeSet4;
extern SchemeSet schemeSet5;
extern SchemeSet schemeSet6;
extern SchemeSet schemeSet7;
extern SchemeSet schemeSet8;
extern SchemeSet schemeSet9;
extern SchemeSet schemeSet10;
extern SchemeSet schemeSet11;
extern SchemeSet schemeSet12;

extern uint8_t current_page_num;

void clear_all_channels();
void set_channel_state(lv_obj_t *channel, ChannelState state);
void set_scheme_set_page(lv_obj_t *container, uint8_t page);
void CustomUI();

#endif //LVGL_CUSTOMUI_H
