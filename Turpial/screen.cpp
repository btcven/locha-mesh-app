/**
 * @Copyright:
 * (c) Copyright 2019 locha.io project developers
 * Licensed under a MIT license, see LICENSE file in the root folder
 * for a full text
 */

#include <SSD1306.h>
#include <OLEDDisplayUi.h>
#include "hal/hardware.h"
#include "screen.h"

boolean screen_on;
unsigned int scr_timeToFadeOut = 15000;
unsigned int scr_elapsedTime;
unsigned long scr_timeStart;

SSD1306 display(SCR_ADD, SCR_SDA, SCR_SCL, SCR_RST);
OLEDDisplayUi ui(&display);

void cb_next_option()
{
    if (screen_on)
    {
        scr_timeStart = millis();
        ui.nextFrame();
    }
    else
    {
        scr_timeStart = millis();
        //display.wakeup();
        Serial.printf("[SCR] Wake up\n");
        screen_on = true;
    }
}
void cb_confirm_option()
{
    Serial.printf("confirm option/frame\n");
}
void frame_00(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x = 0, int16_t y = 0)
{
}
void frame_01(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x = 0, int16_t y = 0)
{
}

FrameCallback frames[] = {frame_00, frame_01};

void task_screen(void *params)
{
#if defined(HELTEC_V2)
    pinMode(Vext, OUTPUT);
    digitalWrite(Vext, LOW);
    delay(100);
#endif

    ui.setTargetFPS(25);
    ui.disableAutoTransition();
    ui.disableAllIndicators();
    ui.setFrames(frames, sizeof(frames));

    ui.init();
    display.flipScreenVertically();

    for (;;)
    {
        scr_elapsedTime = millis() - scr_timeStart;

        if (scr_elapsedTime >= scr_timeToFadeOut && screen_on == true)
        {
            Serial.printf("[SCR] Entering in sleep mode\n");
            screen_on = false;
        }

        int remainingTimeBudget = ui.update();
        delay(15);
    }
}