/* Copyright 2021 kuenhlee, Don Kjer, Tyler Tidman
 * Copyright 2021 Simon Arlott
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

 #include "config_common.h"   // 이것이 없으면  매트릭스 핀설정을 json파일에서 해줘야

#define WAIT_US_TIMER           GPTD3
//#define STM32_GPT_USE_TIM3 TRUE		// Board 디렉토리 파일에 디파인 있음

/* key matrix size (rows in specific keyboard variant) */
#define MATRIX_COLS 16
#define MATRIX_ROWS 8

#define MATRIX_ROW_PINS { C0, C1, C2, C3, C4, C5, C6, C7}       
                    // {A0, A1, A2, A3, A4, A5, A6, A7 }
#define MATRIX_COL_PINS { B0, B1, B2, B3, B4, B5, B8, B9, B10, B13, B14, B15, C9, C10, C11, C12}
                    //{C4, C5, B0, B1, B2, B10, B11, B12, B13, B14, B15, C6, C7, C10, C11, C12 }

/* COL2ROW, ROW2COL*/    
#define DIODE_DIRECTION ROW2COL

// Dynamic EEPROM
// Something sensible or else VIA may crash, Users may enable more if they wish
#define DYNAMIC_KEYMAP_EEPROM_MAX_ADDR  4095      // 4095     // 16383

//#define DYNAMIC_KEYMAP_LAYER_COUNT 4



/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
#define DEBOUNCE 5

/* Bootmagic Lite key configuration */
#define EARLY_INIT_PERFORM_BOOTLOADER_JUMP TRUE
#define BOOTMAGIC_LITE_ROW                 0
#define BOOTMAGIC_LITE_COLUMN              0

#define TAPPING_TERM 175                //////////////////////////////////////////////////

/* LED indicator pins */
#define LED_CAPS_LOCK_PIN   A14     //C9
#define LED_SCROLL_LOCK_PIN A15     //A8
#define LED_WIN_LOCK_PIN    C13     //A9
#define LED_MR_LOCK_PIN     A0      //A10
#define LED_PIN_ON_STATE    0

#define LED_NUM_LOCK_PIN    A13     //C8

/* Original hardware "reset" button on pin D2 */
#define HARDWARE_RESET_PIN  D2
//------------------------------------- Add ----------------------------------

// #define FORCE_NKRO

#define TAPPING_TERM 175 //////////////////////////////////////////////////
#define TAPPING_TERM_PER_KEY
#define AUTO_SHIFT_TIMEOUT  170
#define AUTO_SHIFT_REEPAT
#define NO_AUTO_SHIFT_SPECIAL

#define BOTH_SHIFTS_TURNS_ON_CAPS_WORD

#define ONESHOT_TAP_TOGGLE 2  /* Tapping this number of times holds the key until tapped once again. */
#define ONESHOT_TIMEOUT 5000  /* Time (in ms) before the one shot key is released */

#define LEADER_TIMEOUT 500
#define LEADER_PER_KEY_TIMING
//#define LEADER_NO_TIMEOUT

//-------------------------------------------------------------------------------
#ifdef RGB_MATRIX_ENABLE

#    define DRIVER_ADDR_1 0b1110111
#    define DRIVER_ADDR_2 0b1110100
#    define DRIVER_ADDR_3 0b1110101
#    define DRIVER_ADDR_4 0b1110110
#    define DRIVER_COUNT 4
#    define DRIVER_1_LED_TOTAL 16
#    define DRIVER_2_LED_TOTAL 32
#    define DRIVER_3_LED_TOTAL 32
#    define DRIVER_4_LED_TOTAL 32
//#    define DRIVER_LED_TOTAL (DRIVER_1_LED_TOTAL + DRIVER_2_LED_TOTAL + DRIVER_3_LED_TOTAL + DRIVER_4_LED_TOTAL )
#    define RGB_MATRIX_LED_COUNT (DRIVER_1_LED_TOTAL + DRIVER_2_LED_TOTAL + DRIVER_3_LED_TOTAL + DRIVER_4_LED_TOTAL )

    //#define ISSI_TIMEOUT    100     // option
    #define ISSI_PERSISTENCE 3     // option

#    define RGB_MATRIX_LED_PROCESS_LIMIT 18
#    define RGB_MATRIX_LED_FLUSH_LIMIT 16
#    define RGB_DISABLE_AFTER_TIMEOUT 0          // number of ticks to wait until disabling effects
#    define RGB_DISABLE_WHEN_USB_SUSPENDED       // turn off effects when suspended
#    define RGB_MATRIX_KEYPRESSES
#    define RGB_MATRIX_MAXIMUM_BRIGHTNESS 200
//#    define RGB_MATRIX_CENTER { 103, 32 }

/*
// RGB Matrix Animation modes. Explicitly enabled
// For full list of effects, see:
// https://docs.qmk.fm/#/feature_rgb_matrix?id=rgb-matrix-effects
#    define RGB_MATRIX_STARTUP_MODE RGB_MATRIX_CYCLE_LEFT_RIGHT // Sets the default mode, if none has been set

#    define ENABLE_RGB_MATRIX_BREATHING           // Single hue brightness cycling animation
#    define ENABLE_RGB_MATRIX_BAND_SPIRAL_VAL     // Single hue spinning spiral fades brightness
#    define ENABLE_RGB_MATRIX_CYCLE_ALL           // Full keyboard solid hue cycling through full gradient
#    define ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT    // Full gradient scrolling left to right
#    define ENABLE_RGB_MATRIX_CYCLE_UP_DOWN       // Full gradient scrolling top to bottom
#    define ENABLE_RGB_MATRIX_CYCLE_OUT_IN        // Full gradient scrolling out to in
#    define ENABLE_RGB_MATRIX_CYCLE_PINWHEEL      // Full gradient spinning pinwheel around center of keyboard
#    define ENABLE_RGB_MATRIX_CYCLE_SPIRAL        // Full gradient spinning spiral around center of keyboard
#    define ENABLE_RGB_MATRIX_RAINBOW_PINWHEELS   // Full dual gradients spinning two halfs of keyboard
#    define ENABLE_RGB_MATRIX_RAINDROPS           // Randomly changes a single key's hue

// enabled only of RGB_MATRIX_KEYPRESSES or RGB_MATRIX_KEYRELEASES is defined
#    define ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE   // Pulses keys hit to hue & value then fades value out
#    define ENABLE_RGB_MATRIX_SOLID_REACTIVE          // Static single hue, pulses keys hit to shifted hue then fades to current hue
#    define ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE     // Hue & value pulse near a single key hit then fades value out
#    define ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS    // Hue & value pulse the same column and row of a single key hit then fades value out
#    define ENABLE_RGB_MATRIX_SPLASH                  // Full gradient & value pulse away from a single key hit then fades value out
#    define ENABLE_RGB_MATRIX_SOLID_MULTISPLASH       // Hue & value pulse away from multiple key hits then fades value out
*/
#define ENABLE_RGB_MATRIX_ALPHAS_MODS
#define ENABLE_RGB_MATRIX_GRADIENT_UP_DOWN
#define ENABLE_RGB_MATRIX_GRADIENT_LEFT_RIGHT
#define ENABLE_RGB_MATRIX_BREATHING
#define ENABLE_RGB_MATRIX_BAND_SAT
#define ENABLE_RGB_MATRIX_BAND_VAL
#define ENABLE_RGB_MATRIX_BAND_PINWHEEL_SAT
#define ENABLE_RGB_MATRIX_BAND_PINWHEEL_VAL
#define ENABLE_RGB_MATRIX_BAND_SPIRAL_SAT
#define ENABLE_RGB_MATRIX_BAND_SPIRAL_VAL
#define ENABLE_RGB_MATRIX_CYCLE_ALL
#define ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
#define ENABLE_RGB_MATRIX_CYCLE_UP_DOWN
#define ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
#define ENABLE_RGB_MATRIX_CYCLE_OUT_IN
#define ENABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL
#define ENABLE_RGB_MATRIX_CYCLE_PINWHEEL
#define ENABLE_RGB_MATRIX_CYCLE_SPIRAL
#define ENABLE_RGB_MATRIX_DUAL_BEACON
#define ENABLE_RGB_MATRIX_RAINBOW_BEACON///////////////////////////////////
#define ENABLE_RGB_MATRIX_RAINBOW_PINWHEELS
#define ENABLE_RGB_MATRIX_RAINDROPS
#define ENABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
#define ENABLE_RGB_MATRIX_HUE_BREATHING
#define ENABLE_RGB_MATRIX_HUE_PENDULUM
#define ENABLE_RGB_MATRIX_HUE_WAVE
#define ENABLE_RGB_MATRIX_FRACTAL
#define ENABLE_RGB_MATRIX_PIXEL_RAIN
#define ENABLE_RGB_MATRIX_PIXEL_FLOW

#define ENABLE_RGB_MATRIX_PIXEL_FRACTAL

#define ENABLE_RGB_MATRIX_TYPING_HEATMAP
#define ENABLE_RGB_MATRIX_DIGITAL_RAIN

#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
#define ENABLE_RGB_MATRIX_SPLASH
#define ENABLE_RGB_MATRIX_MULTISPLASH
#define ENABLE_RGB_MATRIX_SOLID_SPLASH
#define ENABLE_RGB_MATRIX_SOLID_MULTISPLASH
#endif
//-----------------------------------------------------
#ifdef LED_MATRIX_ENABLE
    #define BACKLIGHT_LEVELS 5
    /* LED Matrix Driver Configuration */
    #define LED_DRIVER_COUNT 1
    #define LED_DRIVER_ADDR_1 0b1110111 // 0b1110100  //0b1110111
    //#define LED_DRIVER_ADDR_2 0b1110110   //0b1110100

    //#define ISSI_TIMEOUT    100     // option
    #define ISSI_PERSISTENCE 3     // option

    #define LED_DRIVER_1_LED_TOTAL 144     //105       // 104
    //#define DRIVER_2_LED_TOTAL 37
    #define DRIVER_LED_TOTAL LED_DRIVER_1_LED_TOTAL       //  + DRIVER_2_LED_TOTAL)

    //#define LED_MATRIX_ROWS 15
    //#define LED_MATRIX_COLS 7

    /* Enable caps-lock LED */
    //#define CAPS_LOCK_LED_INDEX 0
    //#define SCROLL_LOCK_LED_INDEX 1     //14
    //#define NUM_LOCK_LED_INDEX 2        //104

    #define LED_MATRIX_KEYPRESSES // reacts to keypresses
    //#define LED_MATRIX_KEYRELEASES // reacts to keyreleases (instead of keypresses)
    //#define LED_MATRIX_FRAMEBUFFER_EFFECTS // enable framebuffer effects
    //#define LED_DISABLE_TIMEOUT 0 // number of milliseconds to wait until led automatically turns off
    //#define LED_DISABLE_AFTER_TIMEOUT 0 // OBSOLETE: number of ticks to wait until disabling effects
    #define LED_DISABLE_WHEN_USB_SUSPENDED // turn off effects when suspended
    //#define LED_MATRIX_LED_PROCESS_LIMIT (DRIVER_LED_TOTAL + 4) / 5 // limits the number of LEDs to process in an animation per task run (increases keyboard responsiveness)
    //#define LED_MATRIX_LED_FLUSH_LIMIT 16 // limits in milliseconds how frequently an animation will update the LEDs. 16 (16ms) is equivalent to limiting to 60fps (increases keyboard responsiveness)
    #define LED_MATRIX_MAXIMUM_BRIGHTNESS 255 // limits maximum brightness of LEDs
    #define LED_MATRIX_STARTUP_MODE LED_MATRIX_BREATHING // Sets the default mode, if none has been set
    #define LED_MATRIX_STARTUP_VAL LED_MATRIX_MAXIMUM_BRIGHTNESS/2 // Sets the default brightness value, if none has been set
    #define LED_MATRIX_STARTUP_SPD 127
    // #define LED_MATRIX_CENTER {112, 32}

    #define ENABLE_LED_MATRIX_ALPHAS_MODS
    #define ENABLE_LED_MATRIX_BREATHING
    #define ENABLE_LED_MATRIX_BAND
    #define ENABLE_LED_MATRIX_BAND_PINWHEEL
    #define ENABLE_LED_MATRIX_BAND_SPIRAL
    #define ENABLE_LED_MATRIX_CYCLE_LEFT_RIGHT
    #define ENABLE_LED_MATRIX_CYCLE_UP_DOWN
    #define ENABLE_LED_MATRIX_CYCLE_OUT_IN
    #define ENABLE_LED_MATRIX_DUAL_BEACON
    #if defined(LED_MATRIX_KEYPRESSES)   // #if defined(LED_MATRIX_KEYREACTIVE_ENABLED)
        #define ENABLE_LED_MATRIX_SOLID_REACTIVE_SIMPLE
        #define ENABLE_LED_MATRIX_SOLID_REACTIVE_WIDE
        #define ENABLE_LED_MATRIX_SOLID_REACTIVE_MULTIWIDE
        #define ENABLE_LED_MATRIX_SOLID_REACTIVE_CROSS
        #define ENABLE_LED_MATRIX_SOLID_REACTIVE_MULTICROSS
        #define ENABLE_LED_MATRIX_SOLID_REACTIVE_NEXUS
        #define ENABLE_LED_MATRIX_SOLID_REACTIVE_MULTINEXUS
        #define ENABLE_LED_MATRIX_SPLASH
        #define ENABLE_LED_MATRIX_MULTISPLASH
    #endif
    #define ENABLE_LED_MATRIX_WAVE_LEFT_RIGHT
    #define ENABLE_LED_MATRIX_WAVE_UP_DOWN
    #define ENABLE_LED_MATRIX_EFFECT_MAX
#endif

#ifdef OLED_ENABLE
/*  OLED Configulation */
// #define OLED_DISPLAY_ADDRESS 0x3C
// #define OLED_COM_PINS 0x12           // 크기가 작아짐.. 어디다 사용? 128x64에 사용하는 건가? 뭐지?
// #define OLED_IC OLED_IC_SSD1306      //안해도 default로 되어 있는듯

/* Required for SH1106 Oled Driver (ex; YwRobot 1.3inch) */
//#define OLED_IC OLED_IC_SH1106      //
//#define OLED_COLUMN_OFFSET 2        // SH1106 screen is a little off to the left

/* Default Oled init */
//#define OLED_TIMEOUT 20000              // Turns off OLED after said amount of milliseconds
//#define OLED_DISPLAY_128X64
//#define OLED_BRIGHTNESS 255

//#define OLED_UPDATE_INTERVAL 33 // ~30fps
#endif

//#define I2C_DRIVER I2CD1
//#define I2C1_SCL_PIN B6
//#define I2C1_SDA_PIN B7
// #define I2C1_SCL_PAL_MODE 1
// #define I2C1_SDA_PAL_MODE 1

// #define I2C1_TIMINGR_PRESC 2U  // 키보드 westm/westm9/rev1 = 00, 03, 01, 03, 09
// #define I2C1_TIMINGR_SCLDEL 0U  //2 0 0 9 14--> 100K/Digital Filter off
// #define I2C1_TIMINGR_SDADEL 0U
// #define I2C1_TIMINGR_SCLH 9U
// #define I2C1_TIMINGR_SCLL 14U
