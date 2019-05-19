/**
 * @Copyright:
 * (c) Copyright 2019 locha.io project developers
 * Licensed under a MIT license, see LICENSE file in the root folder
 * for a full text
 */

#include <SSD1306.h>
#include <OLEDDisplayUi.h>
#include "screen.h"
#include "hal/hardware.h"
#include "graphics.h"

boolean screen_on = true;
uint8_t brightness = 5;
unsigned int scr_timeToPoweroff = 15000;
unsigned long scr_timeStart;
unsigned long scr_elapsedTime;

// radio quality link
extern int Lora_RSSI;
extern int Lora_SNR;

// node id
extern char *id_node;

// Queues
extern uint8_t total_mensajes_salientes;
extern uint32_t outcoming_msgs_size;

// Routes
extern uint8_t total_rutas;
extern uint32_t route_table_size;

// Neighbours
extern uint8_t total_vecinos;
extern uint32_t vecinos_table_size;

// dev status.
extern bool deviceConnected;

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
        // display.wakeup();
        ESP_LOGD("SCR", "[SCR] Wake up");
        screen_on = true;
    }
}

void cb_confirm_option()
{
    ESP_LOGD("SCR", "confirm option/frame");
}

void splash_screen(OLEDDisplay *scr)
{
    scr->drawXbm(10, 0, splashScreen_width, splashScreen_height, splashScreen_bits);
    scr->setFont(ArialMT_Plain_24);
    scr->setTextAlignment(TEXT_ALIGN_CENTER);
    scr->drawString(64, 40, "Turpial");
    scr->display();
    delay(5000);
    scr->clear();
    scr->display();
}

void frame_RAD(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x = 0, int16_t y = 0)
{
    // show battery level or battery + bolt.

    // @param battery_level: percent level
    int16_t battery_level = -1;
    // battery pos. coord.
    int16_t batPosX = x + 108;
    int16_t batPosY = y;

    // if param battery_level = -1 then the battery is charging.
    if (battery_level >= 0)
    {
        int16_t level = floor(battery_level * 0.14);
        display->drawXbm(batPosX, batPosY, BAT_width, BAT_height, BAT_bits);
        display->fillRect(batPosX + 2, batPosY + 2, level, 5);
    }
    else
    {
        display->drawXbm(batPosX, batPosY, BAT_B_width, BAT_B_height, BAT_B_bits);
    }
    // show Ble logo if connection active.
    int16_t BLE_xPos = x + 120;
    int16_t BLE_yPos = y + 16;
    display->drawXbm(BLE_xPos, BLE_yPos, BLE_width, BLE_height, BLE_bits);
    if (deviceConnected)
    {
        display->fillRect(BLE_xPos - 5, BLE_yPos + 4, 4, 5);
    }

    // show Wifi AP logo if AP is Enabled
    int16_t WAP_xPos = x + 114;
    int16_t WAP_yPos = y + 36;
    display->drawXbm(WAP_xPos, WAP_yPos, WIFI_width, WIFI_height, WIFI_bits);

    // show WIFI ST logo if is connected.
    int16_t WST_xPos = x + 106;
    int16_t WST_yPos = y + 52;
    display->drawXbm(WST_xPos, WST_yPos, WST_width, WST_height, WST_bits);
    display->setFont(ArialMT_Plain_10);
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->drawString(WST_xPos + 9, WST_yPos, "ST");

    display->setFont(ArialMT_Plain_10);
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->drawString(x + 20, y + 0, "__ RADIO __");

    display->setFont(ArialMT_Plain_16);
    display->drawString(x + 8, y + 24, "RSSI");
    display->drawString(x + 8, y + 42, "SNR");

    // values for rssi & snr are right aligned
    display->setTextAlignment(TEXT_ALIGN_RIGHT);
    // radio iface last rssi value.
    display->drawString(x + 80, y + 24, (String)Lora_RSSI);
    // radio iface last snr value.
    display->drawString(x + 80, y + 42, (String)Lora_SNR);
}
void frame_WIFI(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x = 0, int16_t y = 0)
{
    // @param battery_level: percent level
    int16_t battery_level = -1;
    // battery pos. coord.
    int16_t batPosX = x + 108;
    int16_t batPosY = y;
    // if param battery_level = -1 then the battery is charging.
    if (battery_level >= 0)
    {
        int16_t level = floor(battery_level * 0.14);
        display->drawXbm(batPosX, batPosY, BAT_width, BAT_height, BAT_bits);
        display->fillRect(batPosX + 2, batPosY + 2, level, 5);
    }
    else
    {
        display->drawXbm(batPosX, batPosY, BAT_B_width, BAT_B_height, BAT_B_bits);
    }

    // show Ble logo if connection active.
    int16_t BLE_xPos = x + 120;
    int16_t BLE_yPos = y + 16;
    display->drawXbm(BLE_xPos, BLE_yPos, BLE_width, BLE_height, BLE_bits);
    // show RAD logo if enabled.
    int16_t RAD_xPos = x + 113;
    int16_t RAD_yPos = y + 32;
    display->drawXbm(RAD_xPos, RAD_yPos, RAD_width, RAD_height, RAD_bits);

    // title
    display->setFont(ArialMT_Plain_10);
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->drawString(x + 20, y + 0, "__ WIFI __");

    // Wifi station, active?
    display->drawString(x + 8, y + 16, "WST");
    display->drawString(x + 48, y + 16, "on");

    // Wifi AP?
    display->drawString(x + 8, y + 28, "WAP");
    display->drawString(x + 48, y + 28, "on");

    display->drawString(x + 8, y + 40, "Clients");
    display->setTextAlignment(TEXT_ALIGN_RIGHT);
    display->drawString(x + 60, y + 40, "0");
}

void frame_SYS(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x = 0, int16_t y = 0)
{
    // @param battery_level: percent level
    int16_t battery_level = -1;
    // battery pos. coord.
    int16_t batPosX = x + 108;
    int16_t batPosY = y;
    // if param battery_level = -1 then the battery is charging.
    if (battery_level >= 0)
    {
        int16_t level = floor(battery_level * 0.14);
        display->drawXbm(batPosX, batPosY, BAT_width, BAT_height, BAT_bits);
        display->fillRect(batPosX + 2, batPosY + 2, level, 5);
    }
    else
    {
        display->drawXbm(batPosX, batPosY, BAT_B_width, BAT_B_height, BAT_B_bits);
    }

    // show Ble logo if connection active.
    int16_t BLE_xPos = x + 120;
    int16_t BLE_yPos = y + 16;
    display->drawXbm(BLE_xPos, BLE_yPos, BLE_width, BLE_height, BLE_bits);

    // show Wifi AP logo if AP is Enabled
    int16_t WAP_xPos = x + 114;
    int16_t WAP_yPos = y + 28;
    display->drawXbm(WAP_xPos, WAP_yPos, WIFI_width, WIFI_height, WIFI_bits);

    // show WIFI ST logo if is connected.
    int16_t WST_xPos = x + 106;
    int16_t WST_yPos = y + 38;
    display->drawXbm(WST_xPos, WST_yPos, WST_width, WST_height, WST_bits);
    display->setFont(ArialMT_Plain_10);
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->drawString(WST_xPos + 9, WST_yPos, "ST");

    // show RAD logo if enabled.
    int16_t RAD_xPos = x + 113;
    int16_t RAD_yPos = y + 53;
    display->drawXbm(RAD_xPos, RAD_yPos, RAD_width, RAD_height, RAD_bits);

    // title
    display->setFont(ArialMT_Plain_10);
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->drawString(x + 20, y + 0, "__ SYSTEM __");

    // free RAM
    display->drawString(x + 0, y + 18, "Free RAM");
    display->drawProgressBar(x + 8, y + 32, 64, 5, 60);

    // free Disk
    display->drawString(x + 0, y + 42, "Free Disk");
    display->drawProgressBar(x + 8, y + 56, 64, 5, 60);
}
void frame_Queue(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x = 0, int16_t y = 0)
{
    // title
    display->setFont(ArialMT_Plain_10);
    display->setTextAlignment(TEXT_ALIGN_CENTER);
    display->drawString(x + 64, y + 0, (String)id_node);

    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->drawString(x, y + 14, "Out. msgs");
    display->drawString(x, y + 26, " ''  '' size");

    display->setTextAlignment(TEXT_ALIGN_RIGHT);
    display->drawString(x + 128, y + 14, (String)total_mensajes_salientes);
    display->drawString(x + 128, y + 26, (String)outcoming_msgs_size);
}

// display the nearest neighbours and routes table.
void frame_Neighbours(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x = 0, int16_t y = 0)
{
    // title
    display->setFont(ArialMT_Plain_10);
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->drawString(x + 20, y + 0, "__ Tables __");

    display->drawString(x, y + 12, "Neighbours");
    display->drawString(x, y + 24, "T. Size");
    display->drawHorizontalLine(x + 16, y + 36, 64);
    display->drawString(x, y + 38, "Routes");
    display->drawString(x, y + 50, "T. Size");

    display->setTextAlignment(TEXT_ALIGN_RIGHT);
    display->drawString(x + 128, y + 12, (String)total_vecinos);
    display->drawString(x + 128, y + 24, (String)vecinos_table_size);
    display->drawString(x + 128, y + 38, (String)total_rutas);
    display->drawString(x + 128, y + 50, (String)route_table_size);
}

FrameCallback frames[] = {frame_RAD, frame_Queue, frame_Neighbours};
// FrameCallback frames[] = {frame_RAD, frame_WIFI, frame_SYS};

void task_screen(void *params)
{
    // if v2
    pinMode(Vext, OUTPUT);
    digitalWrite(Vext, LOW); // OLED USE Vext as power supply, must turn ON Vext before OLED init
    delay(200);

    // end if v2
    ui.setTargetFPS(30);

    ui.disableAutoTransition();
    ui.disableAllIndicators();
    //ui.setFrameAnimation(SLIDE_LEFT);

    // Add frames
    ui.setFrames(frames, 3);

    // Initialising the UI will init the display too.
    ui.init();
    display.setBrightness(brightness);
    display.flipScreenVertically();

    splash_screen(&display);

    for (;;)
    {
        scr_elapsedTime = millis() - scr_timeStart;

        if (scr_elapsedTime >= scr_timeToPoweroff && screen_on == true)
        {
            Serial.printf("[SCR] Entering in sleep mode\n");
            screen_on = false;
        }

        int remainingTimeBudget = ui.update();
        delay(15 + remainingTimeBudget);
    }
    vTaskDelete(NULL);
}
