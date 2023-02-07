#pragma once

#ifdef RGB_MATRIX_ENABLE
    #define RGB_DISABLE_TIMEOUT 600000     // 20 minutes (20 * 60 * 1000ms)
    #define RGB_DISABLE_WHEN_USB_SUSPENDED
#endif

#define MOUSEKEY_INTERVAL 16
#define MOUSEKEY_DELAY 0
#define MOUSEKEY_TIME_TO_MAX 60
#define MOUSEKEY_MAX_SPEED 7
#define MOUSEKEY_WHEEL_DELAY 0
#define USB_MAX_POWER_CONSUMPTION 500
#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 255