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

#define WAIT_US_TIMER           GPTD3       //  Encoder 및 RGB ANI가 부드러워짐

/* key matrix size (rows in specific keyboard variant) */
#define MATRIX_COLS 16
#define MATRIX_ROWS 8

#define MATRIX_ROW_PINS {A0, A1, A2, A3, A4, A5, A6, A7 } 
#define MATRIX_COL_PINS {C4, C5, B0, B1, B2, B10, B11, B12, B13, B14, A15, C6, C7, C10, C11, C12 }

/* COL2ROW, ROW2COL*/    
#define DIODE_DIRECTION ROW2COL

// Dynamic EEPROM
// Something sensible or else VIA may crash, Users may enable more if they wish
#define DYNAMIC_KEYMAP_EEPROM_MAX_ADDR  4095

/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
#define DEBOUNCE 5

/* Bootmagic Lite key configuration */
#define EARLY_INIT_PERFORM_BOOTLOADER_JUMP TRUE
#define BOOTMAGIC_LITE_ROW              2
#define BOOTMAGIC_LITE_COLUMN           7

// #define USB_SUSPEND_WAKEUP_DELAY 200   

/* USB Hub Control pins  */
#define GPIO_KM_PWEN        C1      // Hub Device Power on/off
#define GPIO_KM_OE          C2      // Hub Host Connection on/off
#define GPIO_KM_SEL         C0      // Hub Host Selection
#define GPIO_TEST_DEBUG     C3      // C3-LED / C9-GPIO PAD

/* Encoder used pins */
#define ENCODERS_PAD_A { C13 }
#define ENCODERS_PAD_B { C14 } 
/* Specifies the number of pulses the encoder registers between each detent */
#define ENCODER_RESOLUTION 4

#ifdef RGB_MATRIX_ENABLE
/* RGB Underglow or WS2812 RGB Matirx  <spi driver > */
#define WS2812_DI_PIN   B15
    #define WS2812_SPI_DRIVER           SPID2    
    #define WS2812_SPI_MOSI_PAL_MODE    0      // 0 for F072-B15, 5 for F4x1(F103?) // MOSI pin "alternate function", see the respective datasheet for the appropriate values for your MCU. default: 5   
    #define WS2812_SPI_SCK_PIN          B13     //
    #define WS2812_SPI_SCK_PAL_MODE     5       // F072, enable = 0 / F401 enable = 5  (disable은 반대인가?)  (enable되어도 매트릭스 할당되면 매트릭스로 동작함)
    #define WS2812_SPI_DIVISOR          16      //4 4와 8만 되나?       //16    
    #define WS2812_SPI_USE_CIRCULAR_BUFFER    // fix Fliker (이 기능을 켜면 access가 많아짐)

#define RGB_MATRIX_LED_COUNT 111
//    #define RGBLED_NUM 111

 #define RGB_MATRIX_DEFAULT_VAL 60 
#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 220       // @default RGB mode, Max brightness => 200-> 440mA / 220-> 500mA / 255 -> 620mA
#define RGB_MATRIX_CENTER   {94,21}
#define RGB_MATRIX_FRAMEBUFFER_EFFECTS
#define RGB_MATRIX_KEYPRESSES
#define RGB_DISABLE_WHEN_USB_SUSPENDED  // turn off effects when suspended
    #define RGBLIGHT_HUE_STEP 8
    #define RGBLIGHT_SAT_STEP 8
    #define RGBLIGHT_VAL_STEP 8
//#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_GRADIENT_LEFT_RIGHT      // RGB_MATRIX_ALPHAS_MODS

/*
// RGB Matrix Animation modes. Explicitly enabled
// For full list of effects, see:
// https://docs.qmk.fm/#/feature_rgb_matrix?id=rgb-matrix-effects
*/
#define ENABLE_RGB_MATRIX_SOLID_COLOR               // 1
#define ENABLE_RGB_MATRIX_ALPHAS_MODS           
#define ENABLE_RGB_MATRIX_GRADIENT_UP_DOWN
#define ENABLE_RGB_MATRIX_GRADIENT_LEFT_RIGHT
#define ENABLE_RGB_MATRIX_BREATHING                 // 5    // Single hue brightness cycling animation
#define ENABLE_RGB_MATRIX_BAND_SAT                          // 바탕이 흰색 
#define ENABLE_RGB_MATRIX_BAND_VAL                          // 바탕이 안켜짐
#define ENABLE_RGB_MATRIX_BAND_PINWHEEL_SAT
#define ENABLE_RGB_MATRIX_BAND_PINWHEEL_VAL
#define ENABLE_RGB_MATRIX_BAND_SPIRAL_SAT           // 10
#define ENABLE_RGB_MATRIX_BAND_SPIRAL_VAL                   // Single hue spinning spiral fades brightness
#define ENABLE_RGB_MATRIX_CYCLE_ALL                         // Full keyboard solid hue cycling through full gradient
#define ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT                  // Full gradient scrolling left to right
#define ENABLE_RGB_MATRIX_CYCLE_UP_DOWN                     // Full gradient scrolling top to bottom
#define ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON    // 15   // 이것포함 아래 2개 모드가 스펙과 순서가 틀림 - 스펙이 틀린듯)
#define ENABLE_RGB_MATRIX_CYCLE_OUT_IN                      // Full gradient scrolling out to in
#define ENABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL
#define ENABLE_RGB_MATRIX_CYCLE_PINWHEEL                    // Full gradient spinning pinwheel around center of keyboard
#define ENABLE_RGB_MATRIX_CYCLE_SPIRAL                      // Full gradient spinning spiral around center of keyboard

#define ENABLE_RGB_MATRIX_RAINBOW_BEACON            // 20   // ? 
#define ENABLE_RGB_MATRIX_RAINBOW_PINWHEELS                 // Full dual gradients spinning two halfs of keyboard
#define ENABLE_RGB_MATRIX_DUAL_BEACON                   

#define ENABLE_RGB_MATRIX_RAINDROPS                      // Randomly changes a single key's hue
#define ENABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
#define ENABLE_RGB_MATRIX_HUE_BREATHING              // 25 
#define ENABLE_RGB_MATRIX_HUE_PENDULUM              
#define ENABLE_RGB_MATRIX_HUE_WAVE 
#define ENABLE_RGB_MATRIX_PIXEL_RAIN
#define ENABLE_RGB_MATRIX_PIXEL_FLOW

#define ENABLE_RGB_MATRIX_PIXEL_FRACTAL             // 30

#define ENABLE_RGB_MATRIX_TYPING_HEATMAP        //RGB_MATRIX_FRAMEBUFFER_EFFECTS
#define ENABLE_RGB_MATRIX_DIGITAL_RAIN          //RGB_MATRIX_FRAMEBUFFER_EFFECTS

#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE             // Pulses keys hit to hue & value then fades value out
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE                    // Static single hue, pulses keys hit to shifted hue then fades to current hue
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE       // 35   // Hue & value pulse near a single key hit then fades value out
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE  
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS              // Hue & value pulse the same column and row of a single key hit then fades value out
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS 
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS          
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS // 40   //
#define ENABLE_RGB_MATRIX_SPLASH                            // Full gradient & value pulse away from a single key hit then fades value out
#define ENABLE_RGB_MATRIX_MULTISPLASH                       //
#define ENABLE_RGB_MATRIX_SOLID_SPLASH
#define ENABLE_RGB_MATRIX_SOLID_MULTISPLASH         // 44    // Hue & value pulse away from multiple key hits then fades value out

#endif
