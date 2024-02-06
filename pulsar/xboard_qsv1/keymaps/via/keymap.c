/* Copyright 2021 Maxime Coirault, Don Kjer, Tyler Tidman
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

#include QMK_KEYBOARD_H

bool isWinLock = false;             // WIN Lock LED
bool isWinLockLedOn = false;        //
static uint16_t WinLock_timer;      //

static bool     INIT3S_on = false;
static bool     INIT3S_led_on = false;
static uint16_t INIT3S_timer; // for custom key, INIT_3S

uint16_t        startup_timer;
static bool     finished_timer  = false;
static bool     bootloader_mode = false;

static bool     kvm_pc_sel      = false;
static uint16_t kvm_timer;
static bool     kvm_sel_on      = false;

static bool isMacMode = false;

static uint8_t Status_LED_Bright;       // maybe caps, scroll, PC1/2, win/mac, dynamic macro, win lock
                                           // 0 = 초기값, 1~5 = 50~250의 5단계 밝기
#define kvm_deadtime    100
//------------------------------
typedef union {
    uint32_t raw;
    struct {
        bool    eeprom_kvm_pc_sel :1;
        uint8_t eeprom_Status_LED_Bright;       //
    };
} user_config_t;

user_config_t user_config;

//-=-------------------------------------------
    // Layer shorthand
    enum _layer { WIN_BASE = 0, WIN_FN, MAC_BASE, MAC_FN };

    enum K3x0_keycodes {
#ifdef VIA_ENABLE
        KC_TGUI = QK_KB_0,  // Ver 0.21.6에서 변경 (USER00,)  // Toggle between GUI Lock or Unlock
        KC_WIN_MODE,        // WINDOWS Keyboard로 변경
        KC_MAC_MODE,        // Machintosh Keyboard로 변경
        KC_TMODE,        // WINDOWS <--> Machintosh
// KC_MISSION_CONTROL, // MAC_key   // Ver 0.21.6에서 기본지원
// KC_LAUNCHPAD,       // MAC_key   // Ver 0.21.6에서 기본지원
        KC_SPOTLIGHT,       // MAC_key
        KC_DICTATION,       // MAC_key
        KC_DO_NOT_DISTURB,  // MAC_key
        KC_LOCK_SCREEN,     // MAC_key
        KC_LOPTN,           // MAC_key - left option   (win위치)
        KC_ROPTN,           // MAC_key - right option
        KC_LCMMD,           // MAC_key - left command  (ALT위치)
        KC_RCMMD,           // MAC_key - right command
        KC_TASK_VIEW,       // for win (gui + tap)
        KC_FILE_EXPLORER,   // for win (gui + e)
        KC_PC1,             // KM Switch PC1
        KC_PC2,             // KM Switch PC2
        KC_TPC,             // KM Switch PC1<-->PC2 Toggle
        INIT_3S,            // EE_CLR(EEPROM CLear)(공장초기화)를 3초이상 눌러야 동작
        KC_S_LED,            // Change Status LED Brightness
        NEW_SAFE_RANGE = SAFE_RANGE
#else
    KC_TGUI = SAFE_RANGE,   // Toggle between GUI Lock or Unlock
    KC_WIN_MODE,             // WINDOWS Keyboard로 변경
    KC_MAC_MODE,             // Machintosh Keyboard로 변경
    KC_TMODE,             // WINDOWS <--> Machintosh Toggle
// KC_MISSION_CONTROL, // MAC_key   // Ver 0.21.6에서 기본지원
// KC_,       // MAC_key   // Ver 0.21.6에서 기본지원
KC_SPOTLIGHT,           // MAC_key
KC_DICTATION,           // MAC_key      
KC_DO_NOT_DISTURB,      // MAC_key
KC_LOCK_SCREEN,         // MAC_key
    KC_LOPTN,               // MAC_key 불필요한듯
    KC_ROPTN,               // MAC_key 불필요한듯
    KC_LCMMD,               // MAC_key 불필요한듯
    KC_RCMMD,               // MAC_key 불필요한듯
    KC_TASK_VIEW,           // for win (gui + tap)
    KC_FILE_EXPLORER,       // for win (gui + e)
    KC_PC1,                 // KM Switch PC1
    KC_PC2,                 // KM Switch PC2
    KC_TPC,                 // KM Switch PC1<-->PC2 Toggle
    INIT_3S,            // EE_CLR(EEPROM CLear)(공장초기화)를 3초이상 눌러야 동작    
    KC_S_LED,            // Change Status LED Brightness
    NEW_SAFE_RANGE
#endif
    };

// #undef SAFE_RANGE
// #define SAFE_RANGE NEW_SAFE_RANGE

#define KC_WINM KC_WIN_MODE
#define KC_MACM KC_MAC_MODE

#define KC_MCTL KC_MISSION_CONTROL
#define KC_LPAD KC_LAUNCHPAD

#define KC_SPLT KC_SPOTLIGHT
#define KC_SIRI KC_DICTATION
#define KC_DOND KC_DO_NOT_DISTURB
#define KC_LOCK_ KC_LOCK_SCREEN

//#define KC_TASK KC_TASK_VIEW
//#define KC_FLXP KC_FILE_EXPLORER
#define KC_TASK LGUI(KC_TAB)
#define KC_FLXP LGUI(KC_E)
#define KC_DIC  LGUI(KC_H)      // WIN ; 받아쓰기
#define KC_LOCK2 LGUI(KC_L)     // WIN ; pc잠금
#define KC_SNAP SCMD(KC_5)     // MAC Capure ; commnd + shift + 5

void kvm_switch(bool pc_num){
    writePinHigh(GPIO_KM_OE);
    writePinLow(GPIO_KM_PWEN);
    if (!pc_num) writePinLow(GPIO_KM_SEL);
    else   writePinHigh(GPIO_KM_SEL);
    writePinLow(GPIO_KM_OE);

    kvm_timer = timer_read();
    kvm_sel_on = true;
};

void keyboard_pre_init_user(void) {
};
//------------------------------------------
void keyboard_post_init_user(void) {

    user_config.raw = eeconfig_read_user();                 // Read the user config from EEPROM
    kvm_pc_sel      = user_config.eeprom_kvm_pc_sel;
    kvm_switch(kvm_pc_sel);

    Status_LED_Bright = user_config.eeprom_Status_LED_Bright;   // 0 = 초기값, 1~5 = 50~250의 5단계 밝기
    if (Status_LED_Bright == 0) Status_LED_Bright = 5;
}

//-------------------------------------------------------------------------------------------------------
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Keymap _BASE: Base Layer (Default Layer)
      * ┌───┐   ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┐
      * │Esc│   │F1 │F2 │F3 │F4 │ │F5 │F6 │F7 │F8 │ │F9 │F10│F11│F12│ │PSc│Slk│Pse│
      * └───┘   └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┘
      * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐ ┌───┬───┬───┐ ┌───┬───┬───┬───┐
      * │ ` │ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │ 0 │ - │ = │ Backsp│ │Ins│Hom│PgU│ │Num│ / │ * │ - │
      * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤ ├───┼───┼───┤ ├───┼───┼───┼───┤
      * │ Tab │ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │ [ │ ] │  \  │ │Del│End│PgD│ │ 7 │ 8 │ 9 │   │
      * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬┈┈┈┈┤ └───┴───┴───┘ ├───┼───┼───┤ + │
      * │ Caps │ A │ S │ D │ F │ G │ H │ J │ K │ L │ ; │ ' │ # │Entr│               │ 4 │ 5 │ 6 │   │
      * ├────┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴───┴────┤     ┌───┐     ├───┼───┼───┼───┤
      * │Shft│ \ │ Z │ X │ C │ V │ B │ N │ M │ , │ . │ / │    Shift │     │ ↑ │     │ 1 │ 2 │ 3 │   │
      * ├────┼───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤ ┌───┼───┼───┐ ├───┴───┼───┤Ent│
      * │Ctrl│GUI │Alt │                        │ Alt│Func│ App│Ctrl│ │ ← │ ↓ │ → │ │   0   │ . │   │
      * └────┴────┴────┴────────────────────────┴────┴────┴────┴────┘ └───┴───┴───┘ └───────┴───┴───┘
        KC_ESC,           KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,     KC_PSCR, KC_SCRL, KC_PAUS,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,    KC_INS,  KC_HOME, KC_PGUP,  TD(TD_NLCK_CALC),KC_PSLS,KC_PAST,KC_PMNS,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,    KC_DEL,  KC_END,  KC_PGDN,    KC_P7,   KC_P8,   KC_P9,   KC_PPLS,
        RALT_T(KC_CAPS), KC_A, KC_S, KC_D,   KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_NUHS, KC_ENT,                                  KC_P4,   KC_P5,   KC_P6,
        OSM(MOD_LSFT), KC_NUBS, KC_Z, KC_X, KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT,             KC_UP,               KC_P1,   KC_P2,   KC_P3,   KC_PENT,
        OSM(MOD_LCTL), OSM(MOD_LGUI), OSM(MOD_LALT),          KC_SPC,       KC_RALT, LT(WIN_FN,KC_TPC) or MO(WIN_FN),,KC_APP, KC_RCTL,    KC_LEFT, KC_DOWN, KC_RGHT,    KC_P0,            KC_PDOT
      */
     [WIN_BASE] = LAYOUT_all(/* Base Layer */
                                                                                                                                                        KC_PC1, KC_PC2, KC_MUTE,
        KC_ESC,   KC_F1, KC_F2, KC_F3, KC_F4,    KC_F5, KC_F6, KC_F7, KC_F8,   KC_F9, KC_F10, KC_F11, KC_F12,  KC_TMODE,    KC_PSCR, KC_SCRL, KC_PAUS, 
        KC_GRV,  KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL,       KC_BSPC,     KC_INS, KC_HOME, KC_PGUP,     KC_P0, KC_PSLS, KC_PAST, KC_PMNS, 
        KC_TAB,     KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC,    KC_BSLS,     KC_DEL, KC_END, KC_PGDN,     KC_P7, KC_P8, KC_P9, KC_PPLS, 
        KC_CAPS,  KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_NUHS,    KC_ENT,                                  KC_P4, KC_P5, KC_P6, 
        KC_LSFT,     KC_NUBS, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH,    KC_RSFT,              KC_UP,              KC_P1, KC_P2, KC_P3, KC_PENT, 
        KC_LCTL, KC_LGUI, KC_LALT,                KC_SPC,       KC_RALT,  MO(WIN_FN), KC_APP, KC_RCTL,    KC_LEFT, KC_DOWN, KC_RGHT,    KC_P0, KC_PDOT),
    /* Keymap _FN: Function Layer
      * ┌───┐   ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┐
      * │Firm   │Ply│Stp│Prv│Nxt│ │Mut│Vo+│Vo-│   │ │   │   │   │   │ │   │   │   │
      * └───┘   └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┘
      * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐ ┌───┬───┬───┐ ┌───┬───┬───┬───┐
      * │   │   │   │   │   │   │   │   │   │   │   │   │   │       │ │   │   │   │ │   │   │   │   │
      * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤ ├───┼───┼───┤ ├───┼───┼───┼───┤
      * │     │   │   │   │   │   │   │   │   │   │   │   │   │     │ │   │   │   │ │   │   │   │   │
      * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬┈┈┈┈┤ └───┴───┴───┘ ├───┼───┼───┤   │
      * │      │   │   │   │   │   │   │   │   │   │   │   │   │    │               │   │   │   │   │
      * ├────┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴───┴────┤     ┌───┐     ├───┼───┼───┼───┤
      * │    │   │   │   │   │   │   │   │   │   │   │   │          │     │   │     │   │   │   │   │
      * ├────┼───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤ ┌───┼───┼───┐ ├───┴───┼───┤   │
      * │    │Lock│    │                        │    │Func│ Sys│    │ │   │   │   │ │       │   │   │
      * └────┴────┴────┴────────────────────────┴────┴────┴────┴────┘ └───┴───┴───┘ └───────┴───┴───┘
                                파일탐색기  soso                                                                    OK       xxx        xxx        MAC        MAC     xxx        WIN      WIN     (WIN+H) (WIN+L)
                        win-tap   win-e  내PC              브라우져홈        플레이어 Previous   play     Next     검색_서치 컨트롤패널 런치어시스트 미션컨트롤 런치패드 Favorites     뒤로     앞으로  받아쓰기 계정잠금
    KC_BRID, KC_BRIU, KC_TASK, KC_FLXP, KC_MYCM, KC_MAIL, KC_WOME, KC_CALC, KC_MSEL, KC_MPRV, KC_MPLY, KC_MNXT,  KC_WSCH,   KC_CPNL, KC_ASST,  KC_MCTL, KC_LPAD,   KC_WFAV,   KC_WBAK,  KC_WFWD, KC_DIC, KC_LOCK2  
    TD(TD_2),TD(TD_5),KC_MPRV, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU, KC_TASK,TD(TD_3),TD(TD_4), KC_BRIU, KC_BRID, 
    KC_BRID, KC_BRIU, KC_TASK, KC_FLXP,  KC_MYCM, KC_MSEL, KC_MPRV, KC_MPLY, KC_MNXT, KC_MAIL, KC_CALC,  KC_WSCH,   KC_WHOM, KC_WBAK,  KC_WFWD,    
          ESC       F1                          F4       F5                                 F9                           F12                Print      SCR       Pause
        INIT_3S,  KC_BRID, KC_BRIU, KC_TASK, KC_FLXP,  KC_DIC, KC_MSEL, KC_MPRV, KC_MPLY, KC_MNXT, KC_WHOM, KC_CALC,   KC_WSCH,  KC_TMODE,  KC_WBAK, KC_WFWD, KC_LOCK2, 
*/
     [WIN_FN] = LAYOUT_all(/* Function Layer */
                                                                                                                                            KC_PC1, KC_PC2, RGB_MOD,
        INIT_3S,    KC_BRID, KC_BRIU, KC_TASK, KC_WSCH, KC_DIC, KC_MSEL, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU, KC_TMODE,  KC_CALC, KC_WHOM,KC_LOCK2, 
        DM_RSTP, DM_REC1, DM_REC2, DM_PLY1, DM_PLY2, _______, _______, _______, _______, _______, _______,   NK_OFF,  NK_ON,   RGB_TOG,   RGB_SAI, RGB_HUI, RGB_MOD,      KC_CALC, KC_ACL0, KC_ACL1, KC_ACL2,
        _______, KC_BTN1, KC_MS_U, KC_BTN2, KC_WH_U, _______, _______, _______, _______, _______, _______, _______, _______,  _______,    RGB_SAD, RGB_HUD, RGB_RMOD,     KC_BTN4, KC_MS_U, KC_BTN5, KC_WH_U,
        _______, KC_MS_L, KC_MS_D, KC_MS_R, KC_WH_D, _______, _______, _______, _______, _______, _______, _______, _______,   _______,                                   KC_MS_L, KC_BTN3, KC_MS_R,
        _______, _______, KC_BTN4, KC_BTN3, KC_BTN5, _______, KC_S_LED, _______, _______,  _______, _______, _______,           _______,            RGB_VAI,               KC_WH_L, KC_MS_D, KC_WH_R, KC_WH_D, 
        _______, KC_TGUI, _______,                             _______,                            _______,_______, _______,    KC_TPC,   RGB_SPD, RGB_VAD, RGB_SPI,      KC_BTN1,       KC_BTN2),
/*                 
                                        미션컨트롤 스포트라이트 딕테이션 Xdisturb                                                        LAUNCHPAD         lock-screen
                        KC_BRID, KC_BRIU, KC_MCTL, KC_SPLT, KC_SIRI, KC_DOND, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU,    KC_LPAD, KC_SCRL, KC_LOCK_,
                                                            KC_DIC(WIN+H)                                                                               KC_LOCK2(WIN+L)
*/
    [MAC_BASE] = LAYOUT_all(/* Layer 3 */
                                                                                                                                                                            KC_PC1, KC_PC2, KC_MUTE,
        KC_ESC,    KC_BRID, KC_BRIU, KC_MCTL, KC_SPLT, KC_SIRI, KC_DOND, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU, KC_TMODE, KC_SNAP,KC_LPAD,KC_LOCK_, 
        KC_GRV,  KC_1,   KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,    KC_INS,  KC_HOME, KC_PGUP,     KC_P0, KC_PSLS, KC_PAST, KC_PMNS, 
        KC_TAB,  KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,    KC_DEL,  KC_END,  KC_PGDN,     KC_P7,    KC_P8,    KC_P9,   KC_PPLS, 
        KC_CAPS, KC_A,   KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_NUHS, KC_ENT,                                    KC_P4,    KC_P5,    KC_P6, 
        KC_LSFT, KC_NUBS, KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M, KC_COMM,    KC_DOT,  KC_SLSH,          KC_RSFT,             KC_UP,                KC_P1,    KC_P2,    KC_P3,   KC_PENT, 
        KC_LCTL, KC_LALT, KC_LGUI,                           KC_SPC,                             KC_RGUI,MO(MAC_FN),KC_RALT,KC_RCTL,    KC_LEFT, KC_DOWN, KC_RGHT,     KC_P0,              KC_PDOT),

    [MAC_FN]   = LAYOUT_all(/* Layer 4 */
                                                                                                                                                                            KC_PC1, KC_PC2, RGB_MOD,
        INIT_3S,    KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12, KC_TMODE,  KC_F13, KC_F14, KC_F15, 
        DM_RSTP, DM_REC1, DM_REC2, DM_PLY1, DM_PLY2, _______, _______, _______, _______, _______, _______, NK_OFF,   NK_ON,  RGB_TOG,    RGB_SAI, RGB_HUI, RGB_MOD,     KC_CALC, KC_ACL0, KC_ACL1, KC_ACL2, 
        _______, KC_BTN1, KC_MS_U, KC_BTN2, KC_WH_U, _______, _______, _______, _______, _______, _______, _______, _______, _______,    RGB_SAD, RGB_HUD, RGB_RMOD,    KC_BTN4, KC_MS_U, KC_BTN5, KC_WH_U, 
        _______, KC_MS_L, KC_MS_D, KC_MS_R, KC_WH_D, _______, _______, _______, _______, _______, _______, _______, _______, _______,                                   KC_MS_L, KC_BTN3, KC_MS_R, 
        _______, _______, KC_BTN4, KC_BTN3, KC_BTN5, _______, KC_S_LED, _______, _______, _______, _______, _______,          _______,             RGB_VAI,              KC_WH_L, KC_MS_D, KC_WH_R, KC_WH_D,  
        _______, _______, _______,                         _______,                               _______, _______, _______,  KC_TPC,    RGB_SPD, RGB_VAD, RGB_SPI,     KC_BTN1,       KC_BTN2)
        };

//-----------------------------------------------------------------
#ifdef JOYSTICK_ENABLE
    joystick_config_t joystick_axes[JOYSTICK_AXIS_COUNT] = {
    //    [0] = JOYSTICK_AXIS_IN_OUT(C3, B8, 900, 575, 285), 
    //    [0] = JOYSTICK_AXIS_IN(A0, 256, 512, 700),
        [0] = JOYSTICK_AXIS_VIRTUAL,
        [1] = JOYSTICK_AXIS_VIRTUAL    
    };

    static bool precision = false;
    static uint16_t precision_mod = 1023;
    static uint16_t axis_val = 2047;
#endif
//-----------------------------------------------------------------

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [WIN_BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [WIN_FN]   = { ENCODER_CCW_CW(RGB_VAD, RGB_VAI) },
    [MAC_BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [MAC_FN]   = { ENCODER_CCW_CW(RGB_VAD, RGB_VAI) }
};
//-----------------------------------------------------------------------------

#define HCS(report) host_consumer_send(record->event.pressed ? report : 0); return false
#define HSS(report) host_system_send(record->event.pressed ? report : 0); return false

static bool win_key_locked = false;
// static bool all_key_locked = false;

 static uint8_t mac_keycode[4] = {KC_LOPT, KC_ROPT, KC_LCMD, KC_RCMD};

// typedef struct PACKED {
//     uint8_t len;
//     uint8_t keycode[2];
// } key_combination_t;

// key_combination_t key_comb_list[2] = {
//     {2, {KC_LWIN, KC_TAB}},
//     {2, {KC_LWIN, KC_E}}
// };

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    finished_timer = true;

    switch (keycode) {
        case INIT_3S:
            if (record->event.pressed){
                INIT3S_timer = timer_read();
                INIT3S_on    = true;
                // return false;
            }
            else{
                INIT3S_on   = false;
                // return false;              
            }
            break;

        case KC_TGUI:
            if (record->event.pressed) { // Toggle GUI lock on key press
                win_key_locked = !win_key_locked;

                if (win_key_locked){                // LED를 Win키에 표시
                    isWinLock = true;               //
                    isWinLockLedOn = true;          //
                    WinLock_timer  = timer_read();  //
                }                                   //
                else {                              //
                    isWinLock = false;              //
                    isWinLockLedOn = false;         //
                }                                   //
            }
            break;
            
        case KC_LGUI:
            if (win_key_locked) { return false; }
            else{ return true; }

        case KC_MISSION_CONTROL:
            if (record->event.pressed) host_consumer_send(0x29F);
            else  host_consumer_send(0);
            return false; // Skip all further processing of this key
        case KC_LAUNCHPAD:
            if (record->event.pressed)  host_consumer_send(0x2A0);
            else   host_consumer_send(0);
            return false; // Skip all further processing of this key

        case KC_SPOTLIGHT:
            HCS(0x221);
        case KC_DICTATION:
            HCS(0xCF);
        case KC_DO_NOT_DISTURB:
            HSS(0x9B);
        case KC_LOCK_SCREEN:
            HCS(0x19E);

        case KC_LOPTN:
        case KC_ROPTN:
        case KC_LCMMD:
        case KC_RCMMD:
            if (record->event.pressed) {
                register_code(mac_keycode[keycode - KC_LOPTN]);
            } else {
                unregister_code(mac_keycode[keycode - KC_LOPTN]);
            }
            return false; // Skip all further processing of this key

        case KC_WIN_MODE:
            set_single_persistent_default_layer(0);
            isMacMode = false;
            return false;

        case KC_MAC_MODE:
            set_single_persistent_default_layer(2);
            isMacMode = true;            
            return false;

        case KC_TMODE:
            if (record->event.pressed) {
                switch (get_highest_layer(layer_state|default_layer_state)) {
                    case 0:
                    case 1:
                        set_single_persistent_default_layer(2);
                        isMacMode = true;                        

                        break;
                    case 2:
                    case 3:
                    default:
                        set_single_persistent_default_layer(0);
                        isMacMode = false;                    
                }
            }
            return false;

        case KC_PC1:
            if (record->event.pressed) { // Toggle GUI lock on key press
                if (kvm_pc_sel) {
                    kvm_pc_sel = 0;
                    kvm_switch(0);

                    user_config.eeprom_kvm_pc_sel = kvm_pc_sel; // 
                    eeconfig_update_user(user_config.raw); // Writes the new status to EEPROM                    
                }
            }
                return false;            
//           else {
//                return true;
//            }
        case KC_PC2:
            if (record->event.pressed) { // Toggle GUI lock on key press
                if (!kvm_pc_sel){
                    kvm_pc_sel = 1;
                    kvm_switch(1);     

                    user_config.eeprom_kvm_pc_sel = kvm_pc_sel; // 
                    eeconfig_update_user(user_config.raw); // Writes the new status to EEPROM        
                }
                return false;
            }
            else {
                return true;
            }
        case KC_TPC:
            if (record->event.pressed) { // Toggle GUI lock on key press
                kvm_pc_sel = !kvm_pc_sel;                   
                if (kvm_pc_sel) kvm_switch(1);
                else kvm_switch(0);
              
                user_config.eeprom_kvm_pc_sel = kvm_pc_sel; // 
                eeconfig_update_user(user_config.raw); // Writes the new status to EEPROM 
            }
            return false;             
            // else {
            //     return true;
            // }

        case KC_S_LED:              // Change Status LED Brightness
            if (record->event.pressed) { // Toggle GUI lock on key press
                if (++Status_LED_Bright > 5) {
                    Status_LED_Bright = 1;
                };
                user_config.eeprom_Status_LED_Bright = Status_LED_Bright; // 
                eeconfig_update_user(user_config.raw); // Writes the new status to EEPROM 
            }
            return false;

        default:
            return true;   // Process all other keycodes normally        
    }
    return true;
}
//-----------------------------------------------------------------------------

bool isRecording = false;           // dynamic macro LED
bool isRecordingLedOn = false;
static uint16_t recording_timer;

//---------------------------
void matrix_scan_user(void) {
    
    if (kvm_sel_on == true){
        if (timer_elapsed(kvm_timer) > kvm_deadtime ) {
            kvm_sel_on = false;
        }
    }

    if (INIT3S_on == true){
        if (timer_elapsed(INIT3S_timer) > 3000){
            INIT3S_on = false;
            INIT3S_led_on = true;
            
            INIT3S_timer = timer_read();
            // eeconfig_init();
            // //  eeconfig_init_user();
            // soft_reset_keyboard();
        }
    }
    if (INIT3S_led_on == true){
        if(timer_elapsed(INIT3S_timer) > 2000){

            // INIT3S_on = false;
            // INIT3S_led_on = true;
            eeconfig_init();
            //  eeconfig_init_user();
            soft_reset_keyboard();
        }
    }

    if (isRecording){
        if(timer_elapsed(recording_timer) > 500)
        {
            isRecordingLedOn = !isRecordingLedOn;
            recording_timer  = timer_read();
        }
    }
    else{  
    }
}

// void dynamic_macro_record_start_user(void) {        
void dynamic_macro_record_start_user(int8_t direction) {   // Ver 0.21.6에서 변경
    isRecording = true;
    isRecordingLedOn = true;

    recording_timer = timer_read();
}
void dynamic_macro_record_end_user(int8_t direction)
{
    isRecording = false;
    isRecordingLedOn = false;
}
//---------------------------------------------------------------------------
#ifdef RGB_MATRIX_ENABLE

// __attribute__ ((weak)) 
bool rgb_matrix_indicators_user(void)  {

    uint8_t Status_LED_Bright_value = Status_LED_Bright * 50;

    uint8_t layer = get_highest_layer(layer_state|default_layer_state);

    led_t host_leds = host_keyboard_led_state();

    if (host_leds.caps_lock) { 
        rgb_matrix_set_color(72, Status_LED_Bright_value, Status_LED_Bright_value, Status_LED_Bright_value);                     // 0X20            
    }
    if (host_leds.scroll_lock) {
        rgb_matrix_set_color(22, Status_LED_Bright_value, Status_LED_Bright_value, Status_LED_Bright_value);       // 0X20
    }        
    // if (host_keyboard_led_state().num_lock) {
    //     rgb_matrix_set_color(59, Status_LED_Bright_value, Status_LED_Bright_value, Status_LED_Bright_value);    
    // }

    if (INIT3S_led_on == true){
        rgb_matrix_set_color_all(0xff, 0, 0);
            // eeconfig_init();
            // //  eeconfig_init_user();
            // soft_reset_keyboard();        
    }

    if (isWinLock){         // WIN key Lock LED Blink
        // if(timer_elapsed(WinLock_timer) > 1000){     // Blink기능 삭제
        //     isWinLockLedOn = !isWinLockLedOn;
        //     WinLock_timer  = timer_read();
        // }
        // if (isWinLockLedOn) {
            if (isMacMode) {
                win_key_locked = false;      // rgb_matrix_set_color(100, Status_LED_Bright_value, Status_LED_Bright_value, Status_LED_Bright_value);
                isWinLock = false;              //
                isWinLockLedOn = false;         //
            }
            else  {
                rgb_matrix_set_color(99, Status_LED_Bright_value, Status_LED_Bright_value, Status_LED_Bright_value);
            }
        // }
    }

    if (isRecordingLedOn) {                 // Dynamic Macro Recording LED Blink 
        rgb_matrix_set_color(38, Status_LED_Bright_value, Status_LED_Bright_value, Status_LED_Bright_value);            
    }
    if (bootloader_mode) { 
        rgb_matrix_set_color(0, Status_LED_Bright_value, 0x0, 0x0);            
    }

    switch (layer) {
        case 0:
        case 1:
            rgb_matrix_set_color(24,0,0,Status_LED_Bright_value);       // WIN-MAC key
            isMacMode = false;

            if (kvm_pc_sel)  rgb_matrix_set_color(103,0,0,Status_LED_Bright_value);        // Windows Mode & PC2
            else            rgb_matrix_set_color(101,0,0,Status_LED_Bright_value);        // Windows Mode & PC1
            break;   

        case 2:
        case 3:
            rgb_matrix_set_color(24,Status_LED_Bright_value,0,0);       // WIN-MAC key
            isMacMode = true;
            
            if (kvm_pc_sel)  rgb_matrix_set_color(103,Status_LED_Bright_value,0,0);      // MAC Mode & PC2
            else            rgb_matrix_set_color(101,Status_LED_Bright_value,0,0);      // MAC Mode & PC2
            break;          
    }
    return TRUE;
}

#endif
