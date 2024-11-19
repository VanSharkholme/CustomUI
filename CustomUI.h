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

LV_FONT_DECLARE(AliPuHui_20)
LV_FONT_DECLARE(AliPuHui_30)
LV_FONT_DECLARE(AliPuHui_40)
LV_FONT_DECLARE(AliPuHui_50)

#define UI_TIMER_STOP_STATE LV_STATE_USER_1
#define UI_TIMER_START_STATE LV_STATE_USER_2

#define UI_CURRENT_ARC_LINE_WIDTH 6

void CustomUI();

#endif //LVGL_CUSTOMUI_H
