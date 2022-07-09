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
enum custom_keycodes { SK_LYR_U = SAFE_RANGE, SK_LYR_D, SK_RESET, LNK_FB, LNK_TW, LNK_YT, LNK_INS, LNK_GOO, EX_CPYLNK, EX_CNGLNG, VS_BACK, VS_FWD, VS_FIND, VS_FILE, VS_FORMAT, VS_COM, VS_UNCOM, VS_BUILD, EX_DUP, EX_NEW, BEGIN_FEATURE_FLAGS, SET_FF1, SET_FF2, SET_FF3, SET_FF4, FEATURE_FLAGS_COUNT, BEGIN_LAYERS, SET_L1, SET_L2, SET_L3, SET_L4, SET_L5, SET_L6, SET_L7, SET_L8, SET_L9, SET_L10, SET_L11, SET_L12, CUSTOM_KEYCODES_COUNT };

int active_layer = 0;

enum custom_feature_flags { CFF_NONE = 0, CFF_EMPTY1 = 1 << 1, CFF_EMPTY2 = 1 << 2, CFF_AUTOSHIFT = 1 << 3, CFF_ARROWS_AS_CURSOR = 1 << 4 };
int custom_feature_flags = CFF_NONE;

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
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,   KC_PSCR,          KC_MUTE,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,   KC_BSPC,          KC_DEL,
        KC_TAB,  KC_Q,    KC_W,    KC_E,   KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC,  KC_BSLS,           RESET,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,           KC_ENT,           KC_HOME,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,           KC_RSFT, KC_UP,   KC_END,
        KC_LCTL, KC_LGUI, KC_LALT,                  KC_SPC,                             KC_RALT, KC_RCTL, SK_RESET, KC_LEFT, KC_DOWN, KC_RGHT
    ),

    [MP_INTERNET] = LAYOUT(
        _______, I_PREV , I_NEXT , I_NEW  , I_CLOS , _______, _______, I_RST  , I_DOWN , LNK_FB , LNK_TW    , LNK_YT     , LNK_INS , LNK_GOO,          KC_PSCR,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______   , _______    , _______ , _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______   , _______    , _______ , _______,          EX_CPYLNK,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______   , _______    ,           _______,          _______,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______   , _______    ,           _______, _______, _______,
        _______, _______, _______,                           _______,                            A(KC_LEFT), A(KC_RIGHT), SK_RESET,  _______, _______, _______
    ),

    [MP_VISUAL_STUDIO] = LAYOUT(
        _______, C(S(KC_S)), LALT(KC_O), VS_FIND, VS_FILE, _______, VS_BUILD, C(KC_BRK), C(S(KC_F5)) , _______, _______, _______,         _______,  LNK_GOO, KC_PSCR,
        _______, _______, _______,    _______, _______, _______, _______,  _______ , _______  ,    _______, _______, _______,         _______,   _______, _______,
        _______, _______, _______,    _______, _______, _______, _______,  _______ , _______  ,    _______, _______, _______,         _______,   _______, VS_FORMAT,
        _______, _______, _______,    _______, _______, _______, _______,  _______ , _______  ,    _______, _______, _______,         _______,            VS_UNCOM,
        _______, _______, _______,    _______, _______, _______, _______,  _______ , _______  ,    _______, _______, _______,             _______,   VS_COM ,
        _______, _______, _______,                               _______,                       C(KC_MINS), C(S(KC_MINS)), SK_RESET,  _______, _______, _______
    ),

    [MP_DISCORD] = LAYOUT(
        _______, C(A(KC_UP)), C(A(KC_DOWN)), A(KC_UP), A(KC_DOWN), A(S(KC_UP)), A(S(KC_DOWN)), C(KC_E), C(KC_G), C(S(KC_U)), C(KC_P), C(KC_F), C(S(A(KC_V))),   C(S(KC_M)),    KC_MUTE,
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
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,             SET_FF1,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,             SET_FF2, 
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,             SET_FF3,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,    _______, SET_FF4,
        _______, _______, _______,                            _______,                            SK_LYR_D, SK_LYR_U, SK_RESET, _______, _______, _______
    ),

};
// clang-format on

int last_active_layer = 0;

void send_hid_byte(uint8_t byte_value)
{
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

    uprintf("active_layer: %d\n", active_layer);
}

bool process_feature_flag_control(uint16_t keycode, keyrecord_t *record) {
    uprintf("feature_flags: %d\n", custom_feature_flags);

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

                uprintf("feature_flags: %d\n", custom_feature_flags);
            }
        }
    }

    execute_feature_flags();

    return false;
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
            }
            break;
        case SK_LYR_D:
            if (record->event.pressed) {
                last_active_layer--;
                if (last_active_layer < 0) {
                    last_active_layer = get_layer_count();
                }
            }
            break;
        case SK_RESET:
            if (record->event.pressed) {
                last_active_layer = active_layer;
                active_layer      = MP_FUNCTIONAL;
                layer_move(MP_FUNCTIONAL);
            } else {
                active_layer = last_active_layer;
                jump_to_active_layer();
            }
            break;
    }

    for (uint32_t i = BEGIN_LAYERS; i < CUSTOM_KEYCODES_COUNT; i++) {
        if (record->event.pressed) {
            if (keycode == i) {
                last_active_layer = i - BEGIN_LAYERS - 1;
            }
        }
    }

    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    bool consume_input = false;

    consume_input = process_layer_control(keycode, record);
    consume_input = process_feature_flag_control(keycode, record) && consume_input;

    switch (keycode) {
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
    }

    return !consume_input;
};

void raw_hid_receive(uint8_t *data, uint8_t length) {
    if (length > 1) {
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

void set_sidelights_colors4(int r1, int g1, int b1, int r2, int g2, int b2,
                            int r3, int g3, int b3, int r4, int g4, int b4) {
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

void keyboard_post_init_user(void) {
    debug_enable   = true;
    debug_matrix   = false;
    debug_keyboard = true;
    // debug_mouse=true;
}

void rgb_matrix_indicators_kb() {
    rgb_matrix_set_color_all(0, 0, 0);

    if (IS_HOST_LED_ON(USB_LED_CAPS_LOCK)) {
        rgb_matrix_set_color(LED_ESC, 255, 0, 0);
    }

    if (get_mods() & MOD_MASK_SHIFT) {
        rgb_matrix_set_color(LED_ESC, 255, 0, 0);
    }

    if (get_mods() & MOD_MASK_CTRL) {
        rgb_matrix_set_color(LED_ESC, 0, 255, 0);
    }

    if (get_mods() & MOD_MASK_ALT) {
        rgb_matrix_set_color(LED_ESC, 0, 255, 255);
    }

    rgb_matrix_set_color(LED_DEL, 255, 0, 0);

    switch (active_layer) {
        case MP_QWERTY:
            set_sidelights_color(255, 255, 255);
            rgb_matrix_set_color(LED_DEL, 0, 0, 0);
            rgb_matrix_set_color(LED_PGUP, 255, 0, 0);
            break;
        case MP_FUNCTIONAL:
            rgb_matrix_set_color(LED_RCTL, 255, 0, 0);
            rgb_matrix_set_color(LED_RALT, 0, 255, 100);
            rgb_matrix_set_color(LED_FN, 0, 100, 255);
            for (uint32_t i = MP_QWERTY; i < MP_COUNT; i++) {
                rgb_matrix_set_color(LED_LIST_FUNCROW[i + 1], last_active_layer != i ? 255 : 0, last_active_layer == i ? 255 : 0, 0);
            }
            rgb_matrix_set_color(LED_ESC, 0, 0, 0);
            rgb_matrix_set_color(LED_DEL, 0, 0, 0);
            rgb_matrix_set_color(LED_BSPC, 0, 0, 0);
            set_sidelights_color(255, 0, 0);

            for (uint32_t i = 0; i < (FEATURE_FLAGS_COUNT - BEGIN_FEATURE_FLAGS)-1; i++) {
                bool isFlagSet = custom_feature_flags & (1 << (i + 1));
                rgb_matrix_set_color(LED_LIST_MACROCOL[i], isFlagSet ? 0 : 255, isFlagSet ? 255 : 0, 0);
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
        rgb_matrix_set_color(LED_PGDN, 255, 0, 0);
        rgb_matrix_set_color(LED_END, 255, 0, 0);
        set_arrows_color(255, 0, 0);
        rgb_matrix_set_color(LED_RSFT, 255, 0, 0);
    }
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (clockwise) {
        if (custom_feature_flags & CFF_ARROWS_AS_CURSOR && active_layer != MP_FUNCTIONAL) {
            SEND_STRING(SS_TAP(X_MS_WH_DOWN) SS_TAP(X_MS_WH_DOWN) SS_TAP(X_MS_WH_DOWN) SS_TAP(X_MS_WH_DOWN));
        } else {
            tap_code(KC_VOLU);
            tap_code(KC_VOLU);
        }
    } else {
        if (custom_feature_flags & CFF_ARROWS_AS_CURSOR && active_layer != MP_FUNCTIONAL) {
            SEND_STRING(SS_TAP(X_MS_WH_UP) SS_TAP(X_MS_WH_UP) SS_TAP(X_MS_WH_UP) SS_TAP(X_MS_WH_UP));
        } else {
            tap_code(KC_VOLD);
            tap_code(KC_VOLD);
        }
    }
    return false;
}
