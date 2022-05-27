/* Copyright 2021 Glorious, LLC <salman@pcgamingrace.com>

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
#define I_NEW  C(KC_T)
#define I_CLOS C(KC_W)
#define I_RST  C(S(KC_T))
#define I_DOWN C(KC_J)

enum custom_keycodes { SK_LAYER = SAFE_RANGE, SK_RESET, LNK_FB, LNK_TW, LNK_YT };

int        active_layer = 0;
static int num_layers   = 3;

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


    // The FN key by default maps to a momentary toggle to layer 1 to provide access to the RESET key (to put the board into bootloader mode). Without
    // this mapping, you have to open the case to hit the button on the bottom of the PCB (near the USB cable attachment) while plugging in the USB
    // cable to get the board into bootloader mode - definitely not fun when you're working on your QMK builds. Remove this and put it back to KC_RGUI
    // if that's your preference.
    //
    // To put the keyboard in bootloader mode, use FN+backslash. If you accidentally put it into bootloader, you can just unplug the USB cable and
    // it'll be back to normal when you plug it back in.
    //
    // This keyboard defaults to 6KRO instead of NKRO for compatibility reasons (some KVMs and BIOSes are incompatible with NKRO).
    // Since this is, among other things, a "gaming" keyboard, a key combination to enable NKRO on the fly is provided for convenience.
    // Press Fn+N to toggle between 6KRO and NKRO. This setting is persisted to the EEPROM and thus persists between restarts.
    [QWERTY] = LAYOUT(
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,   KC_PSCR,          KC_MUTE,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,   KC_BSPC,          KC_DEL,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC,  KC_BSLS,          RESET,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,           KC_ENT,           RGB_TOG,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,           KC_RSFT, KC_UP,   SK_LAYER,
        KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                             KC_RALT, KC_RCTL, SK_RESET, KC_LEFT, KC_DOWN, KC_RGHT
    ),

    [INTERNET] = LAYOUT(
        _______, I_PREV , I_NEXT , I_NEW  , I_CLOS , _______, _______, I_RST  , I_DOWN , LNK_FB , LNK_TW , LNK_YT , _______, _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          RESET,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,          _______,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, SK_LAYER,    
        _______, _______, _______,                            _______,                            _______, _______, SK_RESET, _______, _______, _______
    ),

    [VISUAL_STUDIO] = LAYOUT(
        KC_V   , _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          RESET,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,          _______,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, SK_LAYER,
        _______, _______, _______,                            _______,                            _______, _______, SK_RESET, _______, _______, _______
    ),

    [SPECIAL] = LAYOUT(
        KC_S   , _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          RESET,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,          _______,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, SK_LAYER,
        _______, _______, _______,                            _______,                            _______, _______, SK_RESET, _______, _______, _______
    ),

    [FUNCTIONAL] = LAYOUT(
        _______, KC_MYCM, KC_WHOM, KC_CALC, KC_MSEL, KC_MPRV, KC_MNXT, KC_MPLY, KC_MSTP, KC_MUTE, KC_VOLD, KC_VOLU, _______, _______,          _______,
        _______, RGB_TOG, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, RGB_VAI, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,            RESET,
        _______, _______, RGB_VAD, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,          _______,
        _______,          _______, RGB_HUI, _______, _______, _______, NK_TOGG, _______, _______, _______, _______,          _______, RGB_MOD, SK_LAYER,
        _______, _______, _______,                            _______,                            _______, _______, SK_RESET, RGB_SPD, RGB_RMOD, RGB_SPI
    ),

};
// clang-format on

int last_active_layer = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    bool shouldMoveLayer = false;
    switch (keycode) {
        case SK_LAYER:
            if (record->event.pressed) {
                active_layer++;
                shouldMoveLayer = true;
            }
            break;
        case SK_RESET:
            if (record->event.pressed) {
                last_active_layer = active_layer;
                layer_move(FUNCTIONAL);
            } else {
                active_layer = last_active_layer;
            }
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
    }

    if (shouldMoveLayer) {
        active_layer = active_layer > num_layers ? 0 : active_layer;
        layer_move(active_layer);

        uprintf("active_layer: %d\n", active_layer);
    }

    return true;
};

void set_sidelights_color(int r, int g, int b) {
    for (int i = 0; i < 8; i++) {
        rgb_matrix_set_color(LED_SIDE_LEFT[i], r, g, b);
        rgb_matrix_set_color(LED_SIDE_RIGHT[i], r, g, b);
    }
}

void keyboard_post_init_user(void) {
    // Custoise these values to desired behaviour
    debug_enable   = true;
    debug_matrix   = false;
    debug_keyboard = true;
    // debug_mouse=true;
}

void rgb_matrix_indicators_kb() {
    rgb_matrix_set_color_all(0, 0, 0);

    if (IS_HOST_LED_ON(USB_LED_CAPS_LOCK)) {
        rgb_matrix_set_color(LED_CAPS, RGB_RED);
    } 

    switch (active_layer) {
        case QWERTY:
            rgb_matrix_set_color(LED_F1, 255, 255, 255 );
            set_sidelights_color( 255, 255, 255);
            break;
        case FUNCTIONAL:
            rgb_matrix_set_color(LED_RCTL, 255, 0, 0);
            set_sidelights_color( 255, 0, 0);
            break;
        case INTERNET:
            rgb_matrix_set_color(LED_F2, 0, 255, 0);
            set_sidelights_color( 0, 255, 0);
            break;
        case VISUAL_STUDIO:
            rgb_matrix_set_color(LED_F3, 0, 0, 255);
            set_sidelights_color( 0, 0, 255);
            break;
        case SPECIAL:
            rgb_matrix_set_color(LED_F4, 0, 255, 255);
            set_sidelights_color( 0, 255, 255);
            break;
    }
}

#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
    if (clockwise) {
        tap_code(KC_VOLU);
    } else {
        tap_code(KC_VOLD);
    }
    return false;
}
#endif // ENCODER_ENABLE
