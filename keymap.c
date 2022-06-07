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

#define QWERTY 0
#define INTERNET 1
#define VISUAL_STUDIO 2
#define SPECIAL 3
#define FUNCTIONAL 4

#define I_PREV C(S(KC_TAB))
#define I_NEXT C(KC_TAB)
#define I_NEW C(KC_T)
#define I_CLOS C(KC_W)
#define I_RST C(S(KC_T))
#define I_DOWN C(KC_J)

enum custom_keycodes { SK_LYR_U = SAFE_RANGE, SK_LYR_D, SK_RESET, LNK_FB, LNK_TW, LNK_YT, SET_L1, SET_L2, SET_L3, SET_L4, EX_CPYLNK, EX_FINDTXT, EX_CNGLNG, VS_BACK, VS_FWD, VS_FIND, VS_FILE, VS_FORMAT, VS_COM, VS_UNCOM };

int        active_layer = 0;
static int num_layers   = 3;
int        led_tick     = 0;

enum unicode_names { SPECIAL_E };

const uint32_t PROGMEM unicode_map[] = {
    [SPECIAL_E] = 0x1F40D,
};

qk_tap_dance_action_t tap_dance_actions[] = {[0] = ACTION_TAP_DANCE_DOUBLE(KC_E, X(SPECIAL_E))};

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

    [QWERTY] = LAYOUT(
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,   KC_PSCR,          KC_MUTE,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,   KC_BSPC,          KC_DEL,
        KC_TAB,  KC_Q,    KC_W,    KC_E,   KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC,  KC_BSLS,           _______,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,           KC_ENT,           _______,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,           KC_RSFT, KC_UP,   _______,
        KC_LCTL, KC_LGUI, KC_LALT,                  KC_SPC,                             KC_RALT, KC_RCTL, SK_RESET, KC_LEFT, KC_DOWN, KC_RGHT
    ),

    [INTERNET] = LAYOUT(
        _______, I_PREV , I_NEXT , I_NEW  , I_CLOS , _______, _______, I_RST  , I_DOWN , LNK_FB , LNK_TW    , LNK_YT     , _______ , _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______   , _______    , _______ , _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______   , _______    , _______ , _______,          RESET,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______   , _______    ,          _______,          EX_FINDTXT,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______   , _______    ,          _______, _______, EX_CPYLNK,    
        _______, _______, _______,                           _______,                            A(KC_LEFT), A(KC_RIGHT), SK_RESET, _______, _______, _______
    ),

    [VISUAL_STUDIO] = LAYOUT(
        _______, C(KC_S), LALT(KC_O), VS_FIND, VS_FILE, _______, C(S(KC_B)), S(KC_F5), C(S(KC_F5)) , _______, _______, _______,         _______,   _______,          C(KC_V),
        _______, _______, _______,    _______, _______, _______, _______,    _______ , _______  ,    _______, _______, _______,         _______,   _______,          _______,
        _______, _______, _______,    _______, _______, _______, _______,    _______ , _______  ,    _______, _______, _______,         _______,   _______,          VS_UNCOM,
        _______, _______, _______,    _______, _______, _______, _______,    _______ , _______  ,    _______, _______, _______,                _______,              VS_COM,
        _______, _______, _______,    _______, _______, _______, _______,    _______ , _______  ,    _______, _______, LCTL(KC_TAB),                    _______,     VS_FORMAT,
        _______, _______, _______,                               _______,                               VS_BACK, VS_FWD      , SK_RESET, _______, _______, _______
    ),

    [SPECIAL] = LAYOUT(
        _______, KC_F13, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          RESET,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,          _______,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______,
        _______, _______, _______,                            _______,                            _______, _______, SK_RESET, _______, _______, _______
    ),

    [FUNCTIONAL] = LAYOUT(
        _______, SET_L1 , SET_L2 , SET_L3 ,  SET_L4, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,            RESET, 
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,          _______,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______,
        _______, _______, _______,                            _______,                            SK_LYR_D, SK_LYR_U, SK_RESET, _______, _______, _______
    ),

};
// clang-format on

int last_active_layer = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    bool shouldMoveLayer = false;
    bool consumeInput    = false;
    switch (keycode) {
        case SK_LYR_U:
            if (record->event.pressed) {
                last_active_layer++;
                if (last_active_layer > num_layers) last_active_layer = 0;
            }
            break;
        case SK_LYR_D:
            if (record->event.pressed) {
                last_active_layer--;
                if (last_active_layer < 0) last_active_layer = num_layers;
            }
            break;
        case SK_RESET:
            if (record->event.pressed) {
                last_active_layer = active_layer;
                active_layer      = FUNCTIONAL;
                layer_move(FUNCTIONAL);
            } else {
                active_layer    = last_active_layer;
                shouldMoveLayer = true;
            }
            break;
        case SET_L1:
            last_active_layer = QWERTY;
            consumeInput      = true;
            break;
        case SET_L2:
            last_active_layer = INTERNET;
            consumeInput      = true;
            break;
        case SET_L3:
            last_active_layer = VISUAL_STUDIO;
            consumeInput      = true;
            break;
        case SET_L4:
            last_active_layer = SPECIAL;
            consumeInput      = true;
            break;
        case LNK_FB:
            if (record->event.pressed) {
                SEND_STRING(SS_LGUI("r") SS_DELAY(16) "www.facebook.com" SS_TAP(X_ENT));
            }
            break;
        case LNK_TW:
            if (record->event.pressed) {
                SEND_STRING(SS_LGUI("r") SS_DELAY(16) "www.twitter.com" SS_TAP(X_ENT));
            }
            break;
        case LNK_YT:
            if (record->event.pressed) {
                SEND_STRING(SS_LGUI("r") SS_DELAY(16) "www.youtube.com" SS_TAP(X_ENT));
            }
            break;
        case EX_CPYLNK:
            if (record->event.pressed) {
                SEND_STRING(SS_TAP(X_F6) SS_DELAY(16) SS_LCTL("c"));
            }
            break;
        case EX_FINDTXT:
            if (record->event.pressed) {
                SEND_STRING(SS_LCTL("c") SS_DELAY(16) SS_LGUI("r") SS_DELAY(32) "\"www.google.com/search?q=" SS_DELAY(16) SS_LCTL("v") "\"" SS_DELAY(16) SS_TAP(X_ENT));
            }
            break;
        case LALT_T(EX_CNGLNG):
            if (record->tap.count == 2 && record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LALT) SS_DELAY(16) SS_TAP(X_LSFT) SS_DELAY(16) SS_UP(X_LALT));
                consumeInput = true;
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
    }

    if (shouldMoveLayer) {
        active_layer = active_layer > num_layers ? 0 : active_layer;
        layer_move(active_layer);

        if (active_layer == INTERNET || active_layer == VISUAL_STUDIO) {
            autoshift_enable();
        } else {
            autoshift_disable();
        }

        uprintf("active_layer: %d\n", active_layer);
    }

    return !consumeInput;
};

void set_sidelights_color(int r, int g, int b) {
    for (int i = 0; i < 8; i++) {
        rgb_matrix_set_color(LED_SIDE_LEFT[i], r, g, b);
        rgb_matrix_set_color(LED_SIDE_RIGHT[i], r, g, b);
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
    rgb_matrix_set_color(LED_RCTL, 0, 255, 255); // fn
    rgb_matrix_set_color(LED_BSPC, 255, 0, 0);
    rgb_matrix_set_color(LED_DEL, 255, 0, 0);
    rgb_matrix_set_color(LED_ESC, 255, 0, 0);

    if (IS_HOST_LED_ON(USB_LED_CAPS_LOCK)) {
        rgb_matrix_set_color(LED_CAPS, 255, 0, 0);
        set_alpha_color(255, 0, 0);
    }

    if (get_mods() & MOD_MASK_SHIFT) {
        rgb_matrix_set_color(LED_LSFT, 255, 0, 0);
        rgb_matrix_set_color(LED_RSFT, 255, 0, 0);
        set_alpha_color(255, 0, 0);
    }

    if (get_mods() & MOD_MASK_CTRL) {
        rgb_matrix_set_color(LED_LCTL, 0, 255, 0);
        set_alpha_color(0, 255, 0);
    }

    if (get_mods() & MOD_MASK_ALT) {
        rgb_matrix_set_color(LED_LALT, 0, 255, 255);
        set_alpha_color(0, 255, 255);
    }

    switch (active_layer) {
        case QWERTY:
            set_sidelights_color(255, 255, 255);
            break;
        case FUNCTIONAL:
            rgb_matrix_set_color(LED_RCTL, 255, 0, 0);
            rgb_matrix_set_color(LED_RALT, 255, 0, 0);
            rgb_matrix_set_color(LED_FN, 255, 0, 0);
            rgb_matrix_set_color(LED_F1, last_active_layer != 0 ? 255 : 0, last_active_layer == 0 ? 255 : 0, 0);
            rgb_matrix_set_color(LED_F2, last_active_layer != 1 ? 255 : 0, last_active_layer == 1 ? 255 : 0, 0);
            rgb_matrix_set_color(LED_F3, last_active_layer != 2 ? 255 : 0, last_active_layer == 2 ? 255 : 0, 0);
            rgb_matrix_set_color(LED_F4, last_active_layer != 3 ? 255 : 0, last_active_layer == 3 ? 255 : 0, 0);
            rgb_matrix_set_color(LED_ESC, 0, 0, 0);
            rgb_matrix_set_color(LED_DEL, 0, 0, 0);
            rgb_matrix_set_color(LED_BSPC, 0, 0, 0);
            set_sidelights_color(255, 0, 0);
            break;
        case INTERNET:
            set_sidelights_color(0, 255, 0);
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
            break;
        case VISUAL_STUDIO:
            set_sidelights_color(0, 0, 255);
            rgb_matrix_set_color(LED_F1, 0, 255, 0);
            rgb_matrix_set_color(LED_F2, 255, 255, 0);
            rgb_matrix_set_color(LED_F3, 0, 0, 255);
            rgb_matrix_set_color(LED_F4, 0, 255, 255);
            rgb_matrix_set_color(LED_F5, 0, 255, 0);
            rgb_matrix_set_color(LED_F6, 255, 255, 0);
            rgb_matrix_set_color(LED_F7, 255, 0, 0);
            rgb_matrix_set_color(LED_F8, 255, 255, 0);
            rgb_matrix_set_color(LED_F9, 255, 0, 0);
            rgb_matrix_set_color(LED_F10, 255, 100, 100);
            rgb_matrix_set_color(LED_F11, 100, 255, 100);
            rgb_matrix_set_color(LED_F12, 255, 255, 0);
            rgb_matrix_set_color(LED_RCTL, 0, 255, 255);
            rgb_matrix_set_color(LED_RALT, 0, 255, 255);
            rgb_matrix_set_color(LED_FN, 0, 255, 255);
            rgb_matrix_set_color(LED_PGUP, 100, 255, 200);
            rgb_matrix_set_color(LED_PGDN, 0, 255, 0);
            rgb_matrix_set_color(LED_END, 100, 100, 255);
            break;
        case SPECIAL:
            set_sidelights_color(0, 255, 255);
            break;
    }
}

#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
    if (clockwise) {
        if (active_layer == VISUAL_STUDIO) {
            if (IS_HOST_LED_ON(USB_LED_CAPS_LOCK)) {
                SEND_STRING(SS_TAP(X_UP));
            } else {
                SEND_STRING(SS_DOWN(X_LCTL) SS_DOWN(X_RIGHT) SS_DELAY(16) SS_UP(X_LCTL) SS_UP(X_RIGHT));
            }
        } else {
            tap_code(KC_VOLU);
        }
    } else {
        if (active_layer == VISUAL_STUDIO) {
            if (IS_HOST_LED_ON(USB_LED_CAPS_LOCK)) {
                SEND_STRING(SS_TAP(X_DOWN));
            } else {
                SEND_STRING(SS_DOWN(X_LCTL) SS_DOWN(X_LEFT) SS_DELAY(16) SS_UP(X_LCTL) SS_UP(X_LEFT));
            }
        } else {
            tap_code(KC_VOLD);
        }
    }
    return false;
}
#endif // ENCODER_ENABLE
