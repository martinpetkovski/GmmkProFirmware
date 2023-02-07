/* Copyright 2022, Martin Petkovski

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include <time.h>
#include <math.h>
#include "print.h"
#include "rgb_matrix_map.h"
#include "raw_hid.h"

#define I_PREV C(S(KC_TAB))
#define I_NEXT C(KC_TAB)
#define I_NEW C(KC_T)
#define I_CLOS C(KC_W)
#define I_RST C(S(KC_T))
#define I_DOWN C(KC_J)

enum mp_profiles { MP_QWERTY, MP_INTERNET, MP_VISUAL_STUDIO, MP_DISCORD, MP_EXPLORER, MP_EMPTY2, MP_EMPTY3, MP_EMPTY4, MP_EMPTY5, MP_EMPTY6, MP_EMPTY7, MP_EMPTY8, MP_COUNT, MP_ARROWS_MOUSE, MP_FUNCTIONAL };
enum custom_keycodes { SK_LYR_U = SAFE_RANGE, SK_LYR_D, SK_RESET, LNK_FB, LNK_TW, LNK_YT, LNK_INS, LNK_GOO, EX_CPYLNK, EX_CNGLNG, VS_BACK, VS_FWD, VS_FIND, VS_FILE, VS_FORMAT, VS_COM, VS_UNCOM, VS_BUILD, EX_DUP, EX_NEW, KC_ROT, BEGIN_FEATURE_FLAGS, SET_FF1, SET_FF2, SET_FF3, SET_FF4, FEATURE_FLAGS_COUNT, SET_BL1, SET_BL2, SET_BL3, SET_BL4, SET_BL5, SET_BL6, SET_BL7, SET_BL8, SET_BL9, SET_BL0, BEGIN_LAYERS, SET_L1, SET_L2, SET_L3, SET_L4, SET_L5, SET_L6, SET_L7, SET_L8, SET_L9, SET_L10, SET_L11, SET_L12, CUSTOM_KEYCODES_COUNT };

int      active_layer = 0;
uint64_t frame_number = 0;

const uint32_t default_single_key_string_timer_time = 128;
uint32_t       single_key_string_timer_time         = 128;

enum custom_feature_flags { CFF_NONE = 0, CFF_RANDOM_LIGHTS = 1 << 1, CFF_BACKLIGHT = 1 << 2, CFF_AUTOSHIFT = 1 << 3, CFF_ARROWS_AS_CURSOR = 1 << 4 };
int custom_feature_flags = CFF_NONE;

enum effect_types { LIGHT_UP, FADE_IN, FADE_OUT, FADE_IN_OUT };

typedef struct key_color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} key_color;

typedef struct backlight_color {
    key_color color;
    char *    name;
} backlight_color;

key_color       current_backlight_color;
uint8_t         backlight_color_index = 0;
backlight_color backlight_colors[10];

typedef struct key_timer {
    uint64_t last_activation_time;
    uint64_t duration;
    uint8_t  effect_type;
    uint8_t  r;
    uint8_t  g;
    uint8_t  b;
} key_timer;

key_timer *timers;

typedef struct string_timer {
    char *   text;
    uint64_t last_activation_time;
} string_timer;

string_timer str_timer;

uint64_t get_total_str_timer_duration(char *str) {
    return strlen(str) * single_key_string_timer_time;
}

bool is_string_timer_running(void) {
    return frame_number < (str_timer.last_activation_time + (strlen(str_timer.text) * single_key_string_timer_time));
}

void start_string_timer(char *text, uint32_t single_key_duration) {
    str_timer.text                 = text;
    str_timer.last_activation_time = frame_number;
    single_key_string_timer_time   = single_key_duration;
}

void stop_string_timer(void) {
    str_timer.last_activation_time = 0;
}

void start_string_timer_default(char *text) {
    start_string_timer(text, default_single_key_string_timer_time);
}

void set_backlight_color(uint8_t r, uint8_t g, uint8_t b) {
    current_backlight_color.r = r;
    current_backlight_color.g = g;
    current_backlight_color.b = b;
    rgb_matrix_set_color_all(current_backlight_color.r, current_backlight_color.g, current_backlight_color.b);
}

void select_backlight_color(void) {
    if (backlight_color_index < 10) {
        set_backlight_color(backlight_colors[backlight_color_index].color.r, backlight_colors[backlight_color_index].color.g, backlight_colors[backlight_color_index].color.b);
    }
}

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

//      ESC      F1       F2       F3       F4       F5       F6       F7       F8       F9       F10      F11      F12	     Prt           Rotary(Mute)
//      ~        1        2        3        4        5        6        7        8        9        0         -       (=)	     BackSpc           Del
//      Tab      Q        W        E        R        T        Y        U        I        O        P        [        ]        \                 PgUp
//      Caps     A        S        D        F        G        H        J        K        L        ;        "                 Enter             PgDn
//      Sh_L              Z        X        C        V        B        N        M        ,        .        ?                 Sh_R     Up       End  
//      Ct_L     Win_L    Alt_L                               SPACE                               Alt_R    FN       Ct_R     Left     Down     Right

/*[EMPTY] = LAYOUT(
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,          _______,
    _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______,
    _______, _______, _______,                            _______,                            _______, _______, _______, _______, _______, _______
),*/

    [MP_QWERTY] = LAYOUT(
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,   KC_PSCR,          MT(KC_ROT, KC_MUTE),
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,   KC_BSPC,          KC_DEL,
        KC_TAB,  KC_Q,    KC_W,    KC_E,   KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC,  KC_BSLS,           RESET,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,           KC_ENT,           KC_HOME,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,           KC_RSFT, KC_UP,   KC_END,
        KC_LCTL, KC_LGUI, KC_LALT,                  KC_SPC,                             KC_RALT, KC_RCTL, SK_RESET, KC_LEFT, KC_DOWN, KC_RGHT
    ),

    [MP_INTERNET] = LAYOUT(
        _______, I_PREV , I_NEXT , I_NEW  , I_CLOS , _______, _______, I_RST  , I_DOWN , LNK_FB , LNK_TW    , LNK_YT     , LNK_INS , LNK_GOO,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______   , _______    , _______ , _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______   , _______    , _______ , _______,          EX_CPYLNK,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______   , _______    ,           _______,          _______,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______   , _______    ,           _______, _______, _______,
        _______, _______, _______,                           _______,                            A(KC_LEFT), A(KC_RIGHT), SK_RESET,  _______, _______, _______
    ),

    [MP_VISUAL_STUDIO] = LAYOUT(
        _______, C(S(KC_S)), LALT(KC_O), VS_FIND, VS_FILE, _______, VS_BUILD, C(KC_BRK), C(S(KC_F5)) , _______, _______, _______,         _______,  LNK_GOO, _______,
        _______, _______, _______,    _______, _______, _______, _______,  _______ , _______  ,    _______, _______, _______,         _______,   _______, _______,
        _______, _______, _______,    _______, _______, _______, _______,  _______ , _______  ,    _______, _______, _______,         _______,   _______, VS_FORMAT,
        _______, _______, _______,    _______, _______, _______, _______,  _______ , _______  ,    _______, _______, _______,         _______,            VS_UNCOM,
        _______, _______, _______,    _______, _______, _______, _______,  _______ , _______  ,    _______, _______, _______,             _______,   VS_COM ,
        _______, _______, _______,                               _______,                       C(KC_MINS), C(S(KC_MINS)), SK_RESET,  _______, _______, _______
    ),

    [MP_DISCORD] = LAYOUT(
        _______, C(A(KC_UP)), C(A(KC_DOWN)), A(KC_UP), A(KC_DOWN), A(S(KC_UP)), A(S(KC_DOWN)), C(KC_E), C(KC_G), C(S(KC_U)), C(KC_P), C(KC_F), C(S(A(KC_V))),   C(S(KC_M)),    _______,
        _______, _______,           _______,  _______,    _______,     _______,       _______, _______, _______,    _______, _______, _______, _______,       _______,          _______,
        _______, _______,           _______,  _______,    _______,     _______,       _______, _______, _______,    _______, _______, _______, _______,       _______,          S(KC_ESC),
        _______, _______,           _______,  _______,    _______,     _______,       _______, _______, _______,    _______, _______, _______,                _______,          _______,
        _______,                    _______,  _______,    _______,     _______,       _______, _______, _______,    _______, _______, _______,                _______, _______, _______,
        _______, _______,           _______,                                          _______,                               A(KC_LEFT), A(KC_RIGHT), SK_RESET, _______, _______, _______
    ),

    [MP_EXPLORER] = LAYOUT(
        _______, A(KC_ENT), _______, _______, C(KC_W), A(KC_UP), A(KC_D), _______, _______, EX_DUP, C(KC_C), C(KC_V), C(S(KC_N)), EX_NEW,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          RESET,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,          _______,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______,
        _______, _______, _______,                            _______,                            A(KC_LEFT), A(KC_RIGHT), SK_RESET, _______, _______, _______
    ),

    [MP_EMPTY2] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          RESET,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,          _______,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______,
        _______, _______, _______,                            _______,                            _______, _______, SK_RESET, _______, _______, _______
    ),

    [MP_EMPTY3] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          RESET,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,          _______,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______,
        _______, _______, _______,                            _______,                            _______, _______, SK_RESET, _______, _______, _______
    ),

    [MP_EMPTY4] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          RESET,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,          _______,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______,
        _______, _______, _______,                            _______,                            _______, _______, SK_RESET, _______, _______, _______
    ),

    [MP_EMPTY5] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          RESET,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,          _______,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______,
        _______, _______, _______,                            _______,                            _______, _______, SK_RESET, _______, _______, _______
    ),

    [MP_EMPTY6] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          RESET,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,          _______,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______,
        _______, _______, _______,                            _______,                            _______, _______, SK_RESET, _______, _______, _______
    ),

    [MP_EMPTY7] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          RESET,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,          _______,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______,
        _______, _______, _______,                            _______,                            _______, _______, SK_RESET, _______, _______, _______
    ),

    [MP_EMPTY8] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          RESET,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,          _______,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______,
        _______, _______, _______,                            _______,                            _______, _______, SK_RESET, _______, _______, _______
    ),

    [MP_ARROWS_MOUSE] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,                 _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,                 _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,                 RESET,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,                 KC_MS_BTN3,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          KC_MS_BTN1,    KC_MS_UP, KC_MS_BTN2,
        _______, _______, _______,                            _______,                            _______, _______, SK_RESET, KC_MS_LEFT, KC_MS_DOWN, KC_MS_RIGHT
    ),

    [MP_FUNCTIONAL] = LAYOUT(
        _______, SET_L1 , SET_L2 , SET_L3 ,  SET_L4,  SET_L5,  SET_L6,  SET_L7,  SET_L8,  SET_L9, SET_L10, SET_L11, SET_L12, _______,             KC_STOP,
        _______, SET_BL1, SET_BL2, SET_BL3, SET_BL4, SET_BL5, SET_BL6, SET_BL7, SET_BL8, SET_BL9, SET_BL0, _______, _______, _______,             SET_FF1,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,             SET_FF2, 
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,             SET_FF3,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,    _______, SET_FF4,
        _______, _______, _______,                            _______,                            SK_LYR_D, SK_LYR_U, SK_RESET, _______, _______, _______
    ),

};
// clang-format on

int last_active_layer = 0;

void send_hid_byte(uint8_t byte_value) {
    uint8_t data[32];
    data[2] = byte_value;
    uprintf("buffer value: %d \n", data[2]);
    raw_hid_send(data, 32);
}

int get_layer_count(void) {
    return MP_COUNT - 1;
}

void execute_feature_flags(void) {
    if (custom_feature_flags & CFF_AUTOSHIFT) {
        autoshift_enable();
    } else {
        autoshift_disable();
    }
}

void jump_to_active_layer(void) {
    active_layer = active_layer > get_layer_count() ? 0 : active_layer;
    active_layer = active_layer < 0 ? get_layer_count() : active_layer;
    layer_move(active_layer);

    if (custom_feature_flags & CFF_ARROWS_AS_CURSOR) {
        layer_on(MP_ARROWS_MOUSE);
    }
}

bool process_feature_flag_control(uint16_t keycode, keyrecord_t *record) {
    for (uint32_t i = BEGIN_FEATURE_FLAGS; i < FEATURE_FLAGS_COUNT; i++) {
        if (record->event.pressed) {
            if (keycode == i) {
                int  feature_index       = (i - BEGIN_FEATURE_FLAGS);
                bool is_feature_flag_set = custom_feature_flags & (1 << feature_index);
                if (is_feature_flag_set) {
                    custom_feature_flags &= ~(1 << feature_index);
                } else {
                    custom_feature_flags |= (1 << feature_index);
                }

                if (1 << feature_index == CFF_RANDOM_LIGHTS) {
                    start_string_timer("stranger things", 128);
                } else if (1 << feature_index == CFF_BACKLIGHT) {
                    start_string_timer("backlight", 128);
                } else if (1 << feature_index == CFF_AUTOSHIFT) {
                    start_string_timer("autoshift", 128);
                } else if (1 << feature_index == CFF_ARROWS_AS_CURSOR) {
                    start_string_timer("arrows as cursor", 128);
                }

                execute_feature_flags();
                uprintf("feature_flags: %d\n", custom_feature_flags);
            }
        }
    }

    return false;
}

void set_key_timer(uint16_t index, key_timer value) {
    memcpy(timers + index, &value, sizeof(value));
}

key_timer *get_key_timer(uint16_t index) {
    return (key_timer *)(timers + index);
}

void send_led_signal(uint16_t ledcode) {
    key_timer current_timer;
    current_timer.last_activation_time = frame_number;
    current_timer.duration             = 384;
    current_timer.effect_type          = FADE_OUT;
    if (custom_feature_flags & CFF_BACKLIGHT) {
        current_timer.r = 0xFF - current_backlight_color.r;
        current_timer.g = 0xFF - current_backlight_color.g;
        current_timer.b = 0xFF - current_backlight_color.b;
    } else {
        current_timer.r = 0xFF;
        current_timer.g = 0;
        current_timer.b = 0;
    }

    set_key_timer(ledcode, current_timer);
}

void send_led_signal_custom(uint16_t ledcode, uint32_t duration, uint8_t effect_type, uint8_t r, uint8_t g, uint8_t b) {
    key_timer current_timer;
    current_timer.last_activation_time = frame_number;
    current_timer.duration             = duration;
    current_timer.effect_type          = effect_type;
    current_timer.r                    = r;
    current_timer.g                    = g;
    current_timer.b                    = b;
    set_key_timer(ledcode, current_timer);
}

void light_up_everything(void) {
    stop_string_timer();
    for (uint8_t i = 0; i < LED_DOWN; i++) {
        bool should_light_up = true;
        for (uint8_t j = 0; j < 14; j++) {
            if (LED_LIST_FUNCROW[j] == i) {
                should_light_up = false;
                break;
            }
        }

        for (uint8_t j = 0; j < 4; j++) {
            if (LED_LIST_MACROCOL[j] == i) {
                should_light_up = false;
                break;
            }
        }

        for (uint8_t j = 0; j < 8; j++) {
            if (LED_SIDE_LEFT[j] == i || LED_SIDE_RIGHT[j] == i) {
                should_light_up = false;
                break;
            }
        }

        if (i == LED_RCTL || i == LED_RALT || i == LED_FN) {
            should_light_up = false;
        }

        if (should_light_up) send_led_signal_custom(i, 128, FADE_OUT, 255, 0, 0);
    }
}

void print_last_active_layer(void) {
    if (last_active_layer == MP_INTERNET)
        start_string_timer_default("internet");
    else if (last_active_layer == MP_VISUAL_STUDIO)
        start_string_timer_default("visual studio");
    else if (last_active_layer == MP_DISCORD)
        start_string_timer_default("discord");
    else if (last_active_layer == MP_EXPLORER)
        start_string_timer_default("explorer");
    else
        start_string_timer_default("qwerty");
}

// if true input should be consumed
bool process_layer_control(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case SK_LYR_U:
            if (record->event.pressed) {
                last_active_layer++;
                if (last_active_layer > get_layer_count()) {
                    last_active_layer = 0;
                }
                print_last_active_layer();
            }
            break;
        case SK_LYR_D:
            if (record->event.pressed) {
                last_active_layer--;
                if (last_active_layer < 0) {
                    last_active_layer = get_layer_count();
                }
                print_last_active_layer();
            }
            break;
        case SK_RESET:
            if (record->event.pressed) {
                last_active_layer = active_layer;
                active_layer      = MP_FUNCTIONAL;
                layer_move(MP_FUNCTIONAL);
                start_string_timer("select modes", 96);
            } else {
                light_up_everything();
                active_layer = last_active_layer;
                jump_to_active_layer();
            }
            break;
    }

    for (uint32_t i = BEGIN_LAYERS; i < CUSTOM_KEYCODES_COUNT; i++) {
        if (record->event.pressed) {
            if (keycode == i) {
                last_active_layer = i - BEGIN_LAYERS - 1;
                print_last_active_layer();
            }
        }
    }

    for (uint32_t i = SET_BL1; i <= SET_BL0; i++) {
        if (record->event.pressed) {
            if (keycode == i) {
                backlight_color_index = i - SET_BL1;
                start_string_timer_default(backlight_colors[backlight_color_index].name);
            }
        }
    }

    return false;
}

uint16_t get_led_code(uint16_t keycode) {
    for (uint16_t i = 0; i < LED_DOWN; i++) {
        if (key_led_map[i] == keycode) return i;
    }
    return LED_DOWN;
}

uint16_t get_led_code_ascii(char chr) {
    for (uint16_t i = 0; i < LED_DOWN; i++) {
        if (key_led_ascii_map[i] == chr) return i;
    }
    return LED_DOWN;
}

bool isFunctionalLanguageChange = false;
bool isLanguageUS               = true;
bool isRotaryModDown            = false;

void handle_ctrl_nonus_language(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_LSFT:
            if (record->event.pressed) {
                if (get_mods() & MOD_MASK_ALT) {
                    isLanguageUS = !isLanguageUS;
                }
            }
            break;
        case KC_LALT:
            if (record->event.pressed) {
                if (get_mods() & MOD_MASK_SHIFT) {
                    isLanguageUS = !isLanguageUS;
                }
            }
            break;
        case KC_LCTL:
            if (record->event.pressed) {
                if (!isLanguageUS) {
                    isFunctionalLanguageChange = true;
                    SEND_STRING(SS_DOWN(X_LALT) SS_DELAY(4) SS_DOWN(X_LSFT) SS_DELAY(4) SS_UP(X_LSFT) SS_DELAY(4) SS_UP(X_LALT));
                }
            } else {
                if (isFunctionalLanguageChange) {
                    isFunctionalLanguageChange = false;
                    SEND_STRING(SS_UP(X_LCTL) SS_DELAY(4) SS_DOWN(X_LALT) SS_DELAY(16) SS_DOWN(X_LSFT) SS_DELAY(16) SS_UP(X_LSFT) SS_DELAY(16) SS_UP(X_LALT));
                }
            }
            break;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    handle_ctrl_nonus_language(keycode, record);

    bool consume_input = false;

    uint16_t led_code = get_led_code(keycode);
    send_led_signal(led_code);

    consume_input = process_layer_control(keycode, record);
    consume_input = process_feature_flag_control(keycode, record) && consume_input;

    switch (keycode) {
        case KC_ROT:
            if (record->event.pressed) {
                isRotaryModDown = true;
            } else {
                isRotaryModDown = false;
            }
            break;
        case LNK_FB:
            if (record->event.pressed) {
                SEND_STRING(SS_LCTL("c"));
                send_hid_byte(1);
            }
            break;
        case LNK_TW:
            if (record->event.pressed) {
                SEND_STRING(SS_LCTL("c"));
                send_hid_byte(2);
            }
            break;
        case LNK_YT:
            if (record->event.pressed) {
                SEND_STRING(SS_LCTL("c"));
                send_hid_byte(3);
            }
            break;
        case LNK_INS:
            if (record->event.pressed) {
                SEND_STRING(SS_LCTL("c"));
                send_hid_byte(4);
            }
            break;
        case LNK_GOO:
            if (record->event.pressed) {
                SEND_STRING(SS_LCTL("c"));
                send_hid_byte(5);
            }
            break;
        case EX_CPYLNK:
            if (record->event.pressed) {
                SEND_STRING(SS_TAP(X_F6) SS_DELAY(16) SS_LCTL("c"));
            }
            break;
        case EX_DUP:
            if (record->event.pressed) {
                SEND_STRING(SS_LALT("f") SS_DELAY(32) SS_LALT("n"));
            }
            break;
        case EX_NEW:
            if (record->event.pressed) {
                SEND_STRING(SS_LALT("h") SS_DELAY(32) SS_TAP(X_W) SS_DELAY(16) SS_TAP(X_UP) SS_DELAY(16) SS_TAP(X_UP) SS_DELAY(16) SS_TAP(X_UP) SS_DELAY(16) SS_TAP(X_ENT));
            }
            break;
        case VS_BACK:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LCTL) SS_DELAY(16) SS_DOWN(X_LSFT) SS_DELAY(16) SS_DOWN(X_BSPC) SS_DELAY(16) SS_UP(X_LCTL) SS_DELAY(16) SS_UP(X_LSFT) SS_DELAY(16) SS_UP(X_BSPC));
            }
            break;
        case VS_FWD:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LCTL) SS_DELAY(16) SS_DOWN(X_LSFT) SS_DELAY(16) SS_DOWN(X_LALT) SS_DELAY(16) SS_DOWN(X_BSPC) SS_DELAY(16) SS_UP(X_LCTL) SS_DELAY(16) SS_UP(X_LSFT) SS_DELAY(16) SS_UP(X_LALT) SS_DELAY(16) SS_UP(X_BSPC));
            }
            break;
        case VS_FIND:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LCTL) SS_DELAY(16) SS_DOWN(X_LSFT) SS_DELAY(16) SS_DOWN(X_F) SS_DELAY(16) SS_UP(X_LCTL) SS_DELAY(16) SS_UP(X_LSFT) SS_DELAY(16) SS_UP(X_F) SS_DELAY(60) SS_TAP(X_ENT));
            }
            break;
        case VS_FILE:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LCTL) SS_DELAY(16) SS_DOWN(X_1) SS_DELAY(16) SS_DOWN(X_F) SS_DELAY(16) SS_UP(X_LCTL) SS_DELAY(16) SS_UP(X_1) SS_DELAY(16) SS_UP(X_F));
            }
            break;
        case VS_FORMAT:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LCTL) SS_DELAY(16) SS_DOWN(X_K) SS_DELAY(16) SS_DOWN(X_D) SS_DELAY(16) SS_UP(X_LCTL) SS_DELAY(16) SS_UP(X_K) SS_DELAY(16) SS_UP(X_D));
            }
            break;
        case VS_COM:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LCTL) SS_DELAY(16) SS_DOWN(X_K) SS_DELAY(16) SS_DOWN(X_C) SS_DELAY(16) SS_UP(X_LCTL) SS_DELAY(16) SS_UP(X_K) SS_DELAY(16) SS_UP(X_C));
            }
            break;
        case VS_UNCOM:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LCTL) SS_DELAY(16) SS_DOWN(X_K) SS_DELAY(16) SS_DOWN(X_U) SS_DELAY(16) SS_UP(X_LCTL) SS_DELAY(16) SS_UP(X_K) SS_DELAY(16) SS_UP(X_U));
            }
            break;
        case VS_BUILD:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LCTL) SS_DELAY(16) SS_DOWN(X_LSFT) SS_DELAY(16) SS_DOWN(X_S) SS_DELAY(16) SS_UP(X_LCTL) SS_DELAY(16) SS_UP(X_LSFT) SS_DELAY(16) SS_UP(X_S) SS_DELAY(16) SS_DOWN(X_LCTL) SS_DELAY(16) SS_DOWN(X_LSFT) SS_DELAY(16) SS_DOWN(X_B) SS_DELAY(16) SS_UP(X_LCTL) SS_DELAY(16) SS_UP(X_LSFT) SS_DELAY(16) SS_UP(X_B));
            }
            break;
    }

    return !consume_input;
};

void raw_hid_receive(uint8_t *data, uint8_t length) {
    if (length > 2) {
        active_layer = data[1];
        jump_to_active_layer();
    }
    raw_hid_send(data, length);
}

void set_sidelights_color(int r, int g, int b) {
    for (int i = 0; i < 8; i++) {
        rgb_matrix_set_color(LED_SIDE_LEFT[i], r, g, b);
        rgb_matrix_set_color(LED_SIDE_RIGHT[i], r, g, b);
    }
}

void set_sidelights_colors(int r1, int g1, int b1, int r2, int g2, int b2) {
    for (int i = 0; i < 4; i++) {
        rgb_matrix_set_color(LED_SIDE_LEFT[i], r1, g1, b1);
        rgb_matrix_set_color(LED_SIDE_RIGHT[i], r1, g1, b1);
    }
    for (int i = 4; i < 8; i++) {
        rgb_matrix_set_color(LED_SIDE_LEFT[i], r2, g2, b2);
        rgb_matrix_set_color(LED_SIDE_RIGHT[i], r2, g2, b2);
    }
}

void set_sidelights_colors4(int r1, int g1, int b1, int r2, int g2, int b2, int r3, int g3, int b3, int r4, int g4, int b4) {
    for (int i = 0; i < 2; i++) {
        rgb_matrix_set_color(LED_SIDE_LEFT[i], r1, g1, b1);
        rgb_matrix_set_color(LED_SIDE_RIGHT[i], r1, g1, b1);
    }
    for (int i = 2; i < 4; i++) {
        rgb_matrix_set_color(LED_SIDE_LEFT[i], r2, g2, b2);
        rgb_matrix_set_color(LED_SIDE_RIGHT[i], r2, g2, b2);
    }
    for (int i = 4; i < 6; i++) {
        rgb_matrix_set_color(LED_SIDE_LEFT[i], r3, g3, b3);
        rgb_matrix_set_color(LED_SIDE_RIGHT[i], r3, g3, b3);
    }
    for (int i = 6; i < 8; i++) {
        rgb_matrix_set_color(LED_SIDE_LEFT[i], r4, g4, b4);
        rgb_matrix_set_color(LED_SIDE_RIGHT[i], r4, g4, b4);
    }
}

void set_arrows_color(int r, int g, int b) {
    for (int i = 0; i < 4; i++) {
        rgb_matrix_set_color(LED_LIST_ARROWS[i], r, g, b);
    }
}

void set_alpha_color(int r, int g, int b) {
    for (int i = 0; i < 26; i++) {
        rgb_matrix_set_color(LED_ALPHA[i], r, g, b);
    }
}

void add_backlight_color(uint8_t index, uint8_t r, uint8_t g, uint8_t b, char *name) {
    if (index >= 10) return;
    backlight_colors[index].color.r = r;
    backlight_colors[index].color.g = g;
    backlight_colors[index].color.b = b;
    backlight_colors[index].name    = name;
}

void keyboard_post_init_user(void) {
    debug_enable   = true;
    debug_matrix   = false;
    debug_keyboard = true;
    // debug_mouse=true;

    timers = malloc(LED_DOWN * sizeof(key_timer));
    memset(timers, 0, LED_DOWN * sizeof(key_timer));

    srand(time(NULL));

    start_string_timer("`1234567890-= welcome", 32);
    add_backlight_color(0, 0xCF, 0xAB, 0x4D, "byers telephone");
    add_backlight_color(1, 0x1A, 0x48, 0x46, "mike\'s shirt");
    add_backlight_color(2, 0xFD, 0xD4, 0x06, "l\'eggo my eggo");
    add_backlight_color(3, 0xE5, 0x63, 0x25, "rotted pumpkin patch");
    add_backlight_color(4, 0x4F, 0xB4, 0xE8, "snowball");
    add_backlight_color(5, 0xFF, 0, 0, "true red");
    add_backlight_color(6, 0, 0xFF, 0, "true green");
    add_backlight_color(7, 0, 0, 0xFF, "true blue");
    add_backlight_color(8, 0x36, 0xC6, 0xE9, "starcourt mall");
    add_backlight_color(9, 0xA9, 0x5B, 0x35, "max");

    custom_feature_flags = CFF_AUTOSHIFT;
}

int32_t remap_value(int32_t low1, int32_t high1, int32_t low2, int32_t high2, int32_t value) {
    return low2 + (value - low1) * (high2 - low2) / (high1 - low1);
}

float lerp(float a, float b, float f) {
    return a + f * (b - a);
}

float clamp(float n, float lower, float upper) {
    return fmax(lower, fmin(n, upper));
}

float circular_ease_out(float p) {
    p = p * p;
    clamp(p, 0.0f, 1.0f);
    return -(p * (p - 2));
}

void rgb_matrix_indicators_kb() {
    if (custom_feature_flags & CFF_BACKLIGHT) {
        select_backlight_color();
    } else {
        set_backlight_color(0, 0, 0);
    }

    if (custom_feature_flags & CFF_RANDOM_LIGHTS) {
        if (frame_number % 2 == 0) {
            uint16_t r = rand() % LED_DOWN;
            send_led_signal_custom(r, 32, FADE_OUT, 255, 0, 0);
        }
    }

    if (IS_HOST_LED_ON(USB_LED_CAPS_LOCK)) {
        rgb_matrix_set_color(LED_CAPS, 0xFF, 0, 0);
    }

    if (get_mods() & MOD_MASK_SHIFT) {
        rgb_matrix_set_color(LED_LSFT, 0xFF, 0, 0);
    }

    if (get_mods() & MOD_MASK_CTRL) {
        rgb_matrix_set_color(LED_LCTL, 0, 0xFF, 0);
    }

    if (get_mods() & MOD_MASK_ALT) {
        rgb_matrix_set_color(LED_LALT, 0, 0xFF, 0xFF);
    }

    rgb_matrix_set_color(LED_DEL, 0xFF, 0, 0);
    rgb_matrix_set_color(LED_ESC, 0xFF, 0, 0);

    switch (active_layer) {
        case MP_QWERTY:
            set_sidelights_color(0xFF, 0xFF, 0xFF);
            rgb_matrix_set_color(LED_DEL, 0, 0, 0);
            rgb_matrix_set_color(LED_PGUP, 0xFF, 0, 0);
            break;
        case MP_FUNCTIONAL:
            rgb_matrix_set_color(LED_RCTL, 0xFF, 0, 0);
            rgb_matrix_set_color(LED_RALT, 0, 0xFF, 100);
            rgb_matrix_set_color(LED_FN, 0, 100, 0xFF);
            for (uint32_t i = MP_QWERTY; i < MP_COUNT; i++) {
                rgb_matrix_set_color(LED_LIST_FUNCROW[i + 1], last_active_layer != i ? 0xFF : 0, last_active_layer == i ? 0xFF : 0, 0);
            }
            rgb_matrix_set_color(LED_ESC, 0, 0, 0);
            rgb_matrix_set_color(LED_DEL, 0, 0, 0);
            rgb_matrix_set_color(LED_BSPC, 0, 0, 0);
            set_sidelights_color(0xFF, 0, 0);

            for (uint32_t i = 0; i < (FEATURE_FLAGS_COUNT - BEGIN_FEATURE_FLAGS) - 1; i++) {
                bool isFlagSet = custom_feature_flags & (1 << (i + 1));
                rgb_matrix_set_color(LED_LIST_MACROCOL[i], isFlagSet ? 0 : 0xFF, isFlagSet ? 0xFF : 0, 0);
            }

            if (custom_feature_flags & CFF_BACKLIGHT) {
                for (uint32_t i = 0; i < 10; i++) {
                    rgb_matrix_set_color(LED_LIST_NUMROW[i + 1], backlight_colors[i].color.r, backlight_colors[i].color.g, backlight_colors[i].color.b);
                }
            }

            break;
        case MP_INTERNET:
            set_sidelights_colors(247, 170, 1, 0, 150, 239);
            rgb_matrix_set_color(LED_F1, 0, 255, 255);
            rgb_matrix_set_color(LED_F2, 0, 255, 255);
            rgb_matrix_set_color(LED_F3, 0, 255, 0);
            rgb_matrix_set_color(LED_F4, 255, 0, 0);
            rgb_matrix_set_color(LED_F5, 0, 255, 0);
            rgb_matrix_set_color(LED_F6, 138, 43, 226);
            rgb_matrix_set_color(LED_F7, 184, 245, 0);
            rgb_matrix_set_color(LED_F8, 255, 255, 255);
            rgb_matrix_set_color(LED_F9, 0, 0, 255);
            rgb_matrix_set_color(LED_F10, 0, 255, 255);
            rgb_matrix_set_color(LED_F11, 255, 0, 0);
            rgb_matrix_set_color(LED_F12, 255, 150, 0);
            rgb_matrix_set_color(LED_RALT, 0, 100, 255);
            rgb_matrix_set_color(LED_FN, 0, 100, 255);
            rgb_matrix_set_color(LED_PGUP, 0, 100, 255);
            rgb_matrix_set_color(LED_PGDN, 0, 100, 255);
            rgb_matrix_set_color(LED_END, 0, 100, 255);
            rgb_matrix_set_color(LED_INS, 255, 255, 255);
            break;
        case MP_VISUAL_STUDIO:
            set_sidelights_colors(69, 14, 134, 114, 191, 217);
            rgb_matrix_set_color(LED_F1, 0, 255, 0);
            rgb_matrix_set_color(LED_F2, 255, 100, 0);
            rgb_matrix_set_color(LED_F3, 0, 0, 255);
            rgb_matrix_set_color(LED_F4, 0, 255, 255);
            rgb_matrix_set_color(LED_F5, 0, 255, 0);
            rgb_matrix_set_color(LED_F6, 255, 100, 0);
            rgb_matrix_set_color(LED_F7, 255, 0, 0);
            rgb_matrix_set_color(LED_F8, 255, 100, 0);
            rgb_matrix_set_color(LED_F9, 255, 0, 0);
            rgb_matrix_set_color(LED_F10, 255, 100, 100);
            rgb_matrix_set_color(LED_F11, 100, 255, 100);
            rgb_matrix_set_color(LED_F12, 255, 100, 0);
            rgb_matrix_set_color(LED_RALT, 0, 255, 255);
            rgb_matrix_set_color(LED_FN, 0, 255, 255);
            rgb_matrix_set_color(LED_PGUP, 100, 255, 200);
            rgb_matrix_set_color(LED_PGDN, 0, 255, 0);
            rgb_matrix_set_color(LED_END, 100, 100, 255);
            rgb_matrix_set_color(LED_INS, 255, 255, 255); // prt scr
            break;
        case MP_DISCORD:
            set_sidelights_color(64, 78, 237);
            rgb_matrix_set_color(LED_F1, 255, 100, 0);
            rgb_matrix_set_color(LED_F2, 255, 100, 0);
            rgb_matrix_set_color(LED_F3, 0, 100, 255);
            rgb_matrix_set_color(LED_F4, 0, 100, 255);
            rgb_matrix_set_color(LED_F5, 160, 0, 255);
            rgb_matrix_set_color(LED_F6, 160, 0, 255);
            rgb_matrix_set_color(LED_F7, 255, 100, 0);
            rgb_matrix_set_color(LED_F8, 255, 100, 200);
            rgb_matrix_set_color(LED_F9, 200, 255, 80);
            rgb_matrix_set_color(LED_F10, 255, 0, 255);
            rgb_matrix_set_color(LED_F11, 255, 80, 200);
            rgb_matrix_set_color(LED_F12, 255, 150, 0);
            rgb_matrix_set_color(LED_INS, 255, 0, 0);
            rgb_matrix_set_color(LED_RALT, 0, 255, 255);
            rgb_matrix_set_color(LED_FN, 0, 255, 255);
            rgb_matrix_set_color(LED_PGUP, 0, 255, 0);
            break;
        case MP_EXPLORER:
            set_sidelights_color(247, 211, 97);
            rgb_matrix_set_color(LED_F1, 0, 255, 255);
            rgb_matrix_set_color(LED_F2, 255, 0, 255);
            rgb_matrix_set_color(LED_F3, 0, 0, 255);
            rgb_matrix_set_color(LED_F4, 255, 0, 0);
            rgb_matrix_set_color(LED_F5, 247, 211, 97);
            rgb_matrix_set_color(LED_F6, 180, 0, 255);
            rgb_matrix_set_color(LED_F9, 0, 255, 0);
            rgb_matrix_set_color(LED_F10, 255, 150, 255);
            rgb_matrix_set_color(LED_F11, 255, 150, 255);
            rgb_matrix_set_color(LED_F12, 0, 255, 0);
            rgb_matrix_set_color(LED_INS, 0, 255, 0);
            rgb_matrix_set_color(LED_RALT, 0, 0, 0);
            rgb_matrix_set_color(LED_RALT, 0, 255, 255);
            rgb_matrix_set_color(LED_FN, 0, 255, 255);
            rgb_matrix_set_color(LED_PGUP, 0, 0, 0);
            break;
    }

    if (custom_feature_flags & CFF_ARROWS_AS_CURSOR && active_layer != MP_FUNCTIONAL) {
        rgb_matrix_set_color(LED_PGDN, 0, 255, 255);
        rgb_matrix_set_color(LED_END, 0, 255, 255);
        set_arrows_color(0, 255, 255);
        rgb_matrix_set_color(LED_RSFT, 0, 255, 255);
    }

    if (!isLanguageUS) {
        rgb_matrix_set_color(LED_LSFT, 255, 0, 255);
        rgb_matrix_set_color(LED_LALT, 255, 0, 255);
    }

    for (uint16_t i = 0; i < LED_DOWN; i++) {
        if (i == LED_CAPS || i == LED_LSFT || i == LED_LALT || i == LED_LCTL) continue;

        key_timer *current_timer = get_key_timer(i);
        if (current_timer) {
            int64_t end_time = current_timer->last_activation_time + (current_timer->duration);
            if (end_time > frame_number) {
                int64_t mapped_value = 0;
                if (current_timer->effect_type == FADE_OUT) {
                    mapped_value = remap_value(current_timer->last_activation_time, end_time, 0, 256, frame_number);
                } else if (current_timer->effect_type == FADE_IN) {
                    mapped_value = remap_value(current_timer->last_activation_time, end_time, 256, 0, frame_number);
                }

                float normalized_mapped_value = (float)((float)mapped_value / 255.0f);
                normalized_mapped_value       = circular_ease_out(normalized_mapped_value);

                rgb_matrix_set_color(i, lerp(current_timer->r, current_backlight_color.r, normalized_mapped_value), lerp(current_timer->g, current_backlight_color.g, normalized_mapped_value), lerp(current_timer->b, current_backlight_color.b, normalized_mapped_value));
            }
        }
    }

    uint32_t letter_index = (frame_number - str_timer.last_activation_time) / single_key_string_timer_time;
    if (letter_index < strlen(str_timer.text)) {
        char     letter  = str_timer.text[letter_index];
        uint16_t ledcode = get_led_code_ascii(letter);
        send_led_signal_custom(ledcode, single_key_string_timer_time, FADE_OUT, 255, 0, 0);
    }

    frame_number++;
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (clockwise) {
        if (custom_feature_flags & CFF_ARROWS_AS_CURSOR && active_layer != MP_FUNCTIONAL) {
            if (isRotaryModDown) {
                SEND_STRING(SS_DOWN(X_LSFT) SS_TAP(X_MS_WH_UP) SS_TAP(X_MS_WH_UP) SS_UP(X_LSFT));
            } else {
                SEND_STRING(SS_TAP(X_MS_WH_DOWN) SS_TAP(X_MS_WH_DOWN) SS_TAP(X_MS_WH_DOWN) SS_TAP(X_MS_WH_DOWN));
            }
        } else {
            tap_code(KC_VOLU);
            tap_code(KC_VOLU);
        }
    } else {
        if (custom_feature_flags & CFF_ARROWS_AS_CURSOR && active_layer != MP_FUNCTIONAL) {
            if (isRotaryModDown) {
                SEND_STRING(SS_DOWN(X_LSFT) SS_TAP(X_MS_WH_DOWN) SS_TAP(X_MS_WH_DOWN) SS_UP(X_LSFT));
            } else {
                SEND_STRING(SS_TAP(X_MS_WH_UP) SS_TAP(X_MS_WH_UP) SS_TAP(X_MS_WH_UP) SS_TAP(X_MS_WH_UP));
            }
        } else {
            tap_code(KC_VOLD);
            tap_code(KC_VOLD);
        }
    }
    return false;
}
