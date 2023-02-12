/* Copyright 2022-2023, Martin Petkovski

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

#ifdef RGB_MATRIX_ENABLE

// Custom RGB Colours
#    define RGB_GODSPEED 0x00, 0xE4, 0xFF // colour for matching keycaps
#    define RGB_NAUTILUS 0x00, 0xA4, 0xA9 // Naurilus Font colours

// RGB LED locations
enum led_location_map {
    LED_ESC,   // 0, ESC, k13
    LED_GRV,   // 1, ~, k16
    LEB_TAB,   // 2, Tab, k11
    LED_CAPS,  // 3, Caps, k21
    LED_LSFT,  // 4, Sh_L, k00
    LED_LCTL,  // 5, Ct_L, k06
    LED_F1,    // 6, F1, k26
    LED_1,     // 7, 1, k17
    LED_Q,     // 8, Q, k10
    LED_A,     // 9, A, k12
    LED_Z,     // 10, Z, k14
    LED_LWIN,  // 11, Win_L, k90
    LED_F2,    // 12, F2, k36
    LED_2,     // 13, 2, k27
    LED_W,     // 14, W, k20
    LED_S,     // 15, S, k22
    LED_X,     // 16, X, k24
    LED_LALT,  // 17, Alt_L, k93
    LED_F3,    // 18, F3, k31
    LED_3,     // 19, 3, k37
    LED_E,     // 20, E, k30
    LED_D,     // 21, D, k32
    LED_C,     // 22, C, k34
    LED_F4,    // 23, F4, k33
    LED_4,     // 24, 4, k47
    LED_R,     // 25, R, k40
    LED_F,     // 26, F, k42
    LED_V,     // 27, V, k44
    LED_F5,    // 28, F5, k07
    LED_5,     // 29, 5, k46
    LED_T,     // 30, T, k41
    LED_G,     // 31, G, k43
    LED_B,     // 32, B, k45
    LED_SPC,   // 33, SPACE, k94
    LED_F6,    // 34, F6, k63
    LED_6,     // 35, 6, k56
    LED_Y,     // 36, Y, k51
    LED_H,     // 37, H, k53
    LED_N,     // 38, N, k55
    LED_F7,    // 39, F7, k71
    LED_7,     // 40, 7, k57
    LED_U,     // 41, U, k50
    LED_J,     // 42, J, k52
    LED_M,     // 43, M, k54
    LED_F8,    // 44, F8, k76
    LED_8,     // 45, 8, k67
    LED_I,     // 46, I, k60
    LED_K,     // 47, K, k62
    LED_COMM,  // 48, ,, k64
    LED_RALT,  // 49, Alt_R, k95
    LED_F9,    // 50, F9, ka6
    LED_9,     // 51, 9, k77
    LED_O,     // 52, O, k70
    LED_L,     // 53, L, k72
    LED_DOT,   // 54, ., k74
    LED_FN,    // 55, FN, k92
    LED_F10,   // 56, F10, ka7
    LED_0,     // 57, 0, k87
    LED_P,     // 58, P, k80
    LED_SCLN,  // 59, ;, k82
    LED_SLSH,  // 60, ?, k85
    LED_F11,   // 61, F11, ka3
    LED_MINS,  // 62, -, k86
    LED_LBRC,  // 63, [, k81
    LED_QUOT,  // 64, ", k83
    LED_RCTL,  // 65, Ct_R, k04
    LED_F12,   // 66, F12, ka5
    LED_L1,    // 67, LED, l01
    LED_R1,    // 68, LED, l11
    LED_INS,   // 69, Prt, k97  -- remapped to INS
    LED_L2,    // 70, LED, l02
    LED_R2,    // 71, LED, l12
    LED_DEL,   // 72, Del, k65
    LED_L3,    // 73, LED, l03
    LED_R3,    // 74, LED, l13
    LED_PGUP,  // 75, PgUp, k15
    LED_L4,    // 76, LED, l04
    LED_R4,    // 77, LED, l14
    LED_EQL,   // 78, =, k66
    LED_RIGHT, // 79, Right, k05
    LED_L5,    // 80, LED, l05
    LED_R5,    // 81, LED, l15
    LED_END,   // 82, End, k75
    LED_L6,    // 83, LED, l06
    LED_R6,    // 84, LED, l16
    LED_BSPC,  // 85, BSpc, ka1
    LED_PGDN,  // 86, PgDn, k25
    LED_L7,    // 87, LED, l07
    LED_R7,    // 88, LED, l17
    LED_RBRC,  // 89, ], k61
    LED_RSFT,  // 90, Sh_R, k91
    LED_L8,    // 91, LED, l08
    LED_R8,    // 92, LED, l18
    LED_BSLS,  // 93, \, ka2
    LED_UP,    // 94, Up, k35
    LED_LEFT,  // 95, Left, k03
    LED_ENT,   // 96, Enter, ka4
    LED_DOWN,  // 97, Down, k73
    LED_COUNT
};

const uint8_t LED_LIST_WASD[] = {LED_W, LED_A, LED_S, LED_D};

const uint8_t LED_LIST_ARROWS[] = {LED_LEFT, LED_RIGHT, LED_UP, LED_DOWN};

const uint8_t LED_LIST_FUNCROW[] = {LED_ESC, LED_F1, LED_F2, LED_F3, LED_F4, LED_F5, LED_F6, LED_F7, LED_F8, LED_F9, LED_F10, LED_F11, LED_F12, LED_INS};

const uint8_t LED_LIST_NUMROW[] = {LED_GRV, LED_1, LED_2, LED_3, LED_4, LED_5, LED_6, LED_7, LED_8, LED_9, LED_0, LED_MINS, LED_EQL, LED_BSPC, LED_DEL};

const uint8_t LED_LIST_MACROCOL[] = {LED_DEL, LED_PGUP, LED_PGDN, LED_END};

const uint8_t LED_LIST_NUMPAD[] = {LED_7, LED_8, LED_9, LED_U, LED_I, LED_O, LED_J, LED_K, LED_L, LED_M, LED_COMM, LED_DOT};

const uint8_t LED_SIDE_LEFT[] = {LED_L1, LED_L2, LED_L3, LED_L4, LED_L5, LED_L6, LED_L7, LED_L8};

const uint8_t LED_SIDE_RIGHT[] = {LED_R1, LED_R2, LED_R3, LED_R4, LED_R5, LED_R6, LED_R7, LED_R8};

const uint32_t LED_ALPHA[] = {LED_Q, LED_W, LED_E, LED_R, LED_T, LED_Y, LED_U, LED_I, LED_O, LED_P, LED_A, LED_S, LED_D, LED_F, LED_G, LED_H, LED_J, LED_K, LED_L, LED_Z, LED_X, LED_C, LED_V, LED_B, LED_N, LED_M};

const uint16_t key_led_map[LED_COUNT] = {
    KC_ESC,   // 0, ESC, k13
    KC_GRV,   // 1, ~, k16
    KC_TAB,   // 2, Tab, k11
    KC_CAPS,  // 3, Caps, k21
    KC_LSFT,  // 4, Sh_L, k00
    KC_LCTL,  // 5, Ct_L, k06
    KC_F1,    // 6, F1, k26
    KC_1,     // 7, 1, k17
    KC_Q,     // 8, Q, k10
    KC_A,     // 9, A, k12
    KC_Z,     // 10, Z, k14
    KC_LWIN,  // 11, Win_L, k90
    KC_F2,    // 12, F2, k36
    KC_2,     // 13, 2, k27
    KC_W,     // 14, W, k20
    KC_S,     // 15, S, k22
    KC_X,     // 16, X, k24
    KC_LALT,  // 17, Alt_L, k93
    KC_F3,    // 18, F3, k31
    KC_3,     // 19, 3, k37
    KC_E,     // 20, E, k30
    KC_D,     // 21, D, k32
    KC_C,     // 22, C, k34
    KC_F4,    // 23, F4, k33
    KC_4,     // 24, 4, k47
    KC_R,     // 25, R, k40
    KC_F,     // 26, F, k42
    KC_V,     // 27, V, k44
    KC_F5,    // 28, F5, k07
    KC_5,     // 29, 5, k46
    KC_T,     // 30, T, k41
    KC_G,     // 31, G, k43
    KC_B,     // 32, B, k45
    KC_SPC,   // 33, SPACE, k94
    KC_F6,    // 34, F6, k63
    KC_6,     // 35, 6, k56
    KC_Y,     // 36, Y, k51
    KC_H,     // 37, H, k53
    KC_N,     // 38, N, k55
    KC_F7,    // 39, F7, k71
    KC_7,     // 40, 7, k57
    KC_U,     // 41, U, k50
    KC_J,     // 42, J, k52
    KC_M,     // 43, M, k54
    KC_F8,    // 44, F8, k76
    KC_8,     // 45, 8, k67
    KC_I,     // 46, I, k60
    KC_K,     // 47, K, k62
    KC_COMM,  // 48, ,, k64
    KC_RALT,  // 49, Alt_R, k95
    KC_F9,    // 50, F9, ka6
    KC_9,     // 51, 9, k77
    KC_O,     // 52, O, k70
    KC_L,     // 53, L, k72
    KC_DOT,   // 54, ., k74
    KC_ESC,   // 55, FN, k92
    KC_F10,   // 56, F10, ka7
    KC_0,     // 57, 0, k87
    KC_P,     // 58, P, k80
    KC_SCLN,  // 59, ;, k82
    KC_SLSH,  // 60, ?, k85
    KC_F11,   // 61, F11, ka3
    KC_MINS,  // 62, -, k86
    KC_LBRC,  // 63, [, k81
    KC_QUOT,  // 64, ", k83
    KC_RCTL,  // 65, Ct_R, k04
    KC_F12,   // 66, F12, ka5
    KC_ESC,   // 67, LED, l01
    KC_ESC,   // 68, LED, l11
    KC_INS,   // 69, Prt, k97  -- remapped to INS
    KC_ESC,   // 70, LED, l02
    KC_ESC,   // 71, LED, l12
    KC_DEL,   // 72, Del, k65
    KC_ESC,   // 73, LED, l03
    KC_ESC,   // 74, LED, l13
    KC_PGUP,  // 75, PgUp, k15
    KC_ESC,   // 76, LED, l04
    KC_ESC,   // 77, LED, l14
    KC_EQL,   // 78, =, k66
    KC_RIGHT, // 79, Right, k05
    KC_ESC,   // 80, LED, l05
    KC_ESC,   // 81, LED, l15
    KC_END,   // 82, End, k75
    KC_ESC,   // 83, LED, l06
    KC_ESC,   // 84, LED, l16
    KC_BSPC,  // 85, BSpc, ka1
    KC_PGDN,  // 86, PgDn, k25
    KC_ESC,   // 87, LED, l07
    KC_ESC,   // 88, LED, l17
    KC_RBRC,  // 89, ], k61
    KC_RSFT,  // 90, Sh_R, k91
    KC_ESC,   // 91, LED, l08
    KC_ESC,   // 92, LED, l18
    KC_BSLS,  // 93, \, ka2
    KC_UP,    // 94, Up, k35
    KC_LEFT,  // 95, Left, k03
    KC_ENT,   // 96, Enter, ka4
    KC_DOWN   // 97, Down, k73
};

const uint16_t key_led_ascii_map[LED_COUNT] = {
    '`',   // 0, ESC, k13
    '`',      // 1, ~, k16
    '`',      // 2, Tab, k11
    '`',      // 3, Caps, k21
    '`',      // 4, Sh_L, k00
    '`',      // 5, Ct_L, k06
    '`',      // 6, F1, k26
    '1',     // 7, 1, k17
    'q',     // 8, Q, k10
    'a',     // 9, A, k12
    'z',     // 10, Z, k14
    '`',      // 11, Win_L, k90
    '`',      // 12, F2, k36
    '2',     // 13, 2, k27
    'w',     // 14, W, k20
    's',     // 15, S, k22
    'x',     // 16, X, k24
    '`',      // 17, Alt_L, k93
    '`',      // 18, F3, k31
    '3',     // 19, 3, k37
    'e',     // 20, E, k30
    'd',     // 21, D, k32
    'c',     // 22, C, k34
    '`',      // 23, F4, k33
    '4',     // 24, 4, k47
    'r',     // 25, R, k40
    'f',     // 26, F, k42
    'v',     // 27, V, k44
    '`',      // 28, F5, k07
    '5',     // 29, 5, k46
    't',     // 30, T, k41
    'g',     // 31, G, k43
    'b',     // 32, B, k45
    ' ',   // 33, SPACE, k94
    '`',      // 34, F6, k63
    '6',     // 35, 6, k56
    'y',     // 36, Y, k51
    'h',     // 37, H, k53
    'n',     // 38, N, k55
    '`',      // 39, F7, k71
    '7',     // 40, 7, k57
    'u',     // 41, U, k50
    'j',     // 42, J, k52
    'm',     // 43, M, k54
    '`',      // 44, F8, k76
    '8',     // 45, 8, k67
    'i',     // 46, I, k60
    'k',     // 47, K, k62
    ',',  // 48, ,, k64
    '`',      // 49, Alt_R, k95
    '`',      // 50, F9, ka6
    '9',     // 51, 9, k77
    'o',     // 52, O, k70
    'l',     // 53, L, k72
    '.',   // 54, ., k74
    '`',      // 55, FN, k92
    '`',      // 56, F10, ka7
    '0',     // 57, 0, k87
    'p',     // 58, P, k80
    ';',  // 59, ;, k82
    '?',  // 60, ?, k85
    '`',      // 61, F11, ka3
    '-',  // 62, -, k86
    '[',  // 63, [, k81
    '\"',  // 64, ", k83
    '`',      // 65, Ct_R, k04
    '`',      // 66, F12, ka5
    '`',      // 67, LED, l01
    '`',      // 68, LED, l11
    '`',      // 69, Prt, k97  -- remapped to INS
    '`',      // 70, LED, l02
    '`',      // 71, LED, l12
    '`',      // 72, Del, k65
    '`',      // 73, LED, l03
    '`',      // 74, LED, l13
    '`',      // 75, PgUp, k15
    '`',      // 76, LED, l04
    '`',      // 77, LED, l14
    '=',   // 78, =, k66
    '`',     // 79, Right, k05
    '`',     // 80, LED, l05
    '`',     // 81, LED, l15
    '`',     // 82, End, k75
    '`',     // 83, LED, l06
    '`',     // 84, LED, l16
    '`',     // 85, BSpc, ka1
    '`',     // 86, PgDn, k25
    '`',     // 87, LED, l07
    '`',     // 88, LED, l17
    ']',  // 89, ], k61
    '`',     // 90, Sh_R, k91
    '`',     // 91, LED, l08
    '`',     // 92, LED, l18
    '\\',  // 93, \, ka2
    '`',     // 94, Up, k35
    '`',    // 95, Left, k03
    '\n',   // 96, Enter, ka4
    '`'   // 97, Down, k73
};

#endif
