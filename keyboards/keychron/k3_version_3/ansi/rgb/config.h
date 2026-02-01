/* Copyright 2024 ~ 2025 @ Keychron (https://www.keychron.com)
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

#define MCU_UNUSED_PINS \
    { A13, B2 }
#define MCU_UNSED_PINS_STATE \
    { PAL_MODE_INPUT_PULLDOWN, PAL_MODE_INPUT_PULLDOWN }

#ifdef RGB_MATRIX_ENABLE
/* RGB Matrix driver configuration */
#    define RGB_MATRIX_LED_COUNT 84
#    define DRIVER_CS_PINS \
        { A8, C9 }

/* Scan phase of led driver */
#    define SNLED27351_PHASE_CHANNEL SNLED27351_SCAN_PHASE_9_CHANNEL
/* Set LED driver current */
#    define SNLED27351_CURRENT_TUNE \
        { 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30 }
        // { 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40 }

/* Set to infinit, which is use in USB mode by default */
#    define RGB_MATRIX_TIMEOUT RGB_MATRIX_TIMEOUT_INFINITE
/* Allow shutdown of led driver to save power */
#    define RGB_MATRIX_DRIVER_SHUTDOWN_ENABLE
#    define RGB_MATRIX_DRIVER_LOAD_ENABLE
/* Turn off backlight on low brightness to save power */
#    define RGB_MATRIX_BRIGHTNESS_TURN_OFF_VAL 48
/* Indications */
#    define CAPS_LOCK_INDEX 46
#    define LOW_BAT_IND_INDEX \
        { 77 }

#    define RGB_MATRIX_KEYPRESSES
#    define RGB_MATRIX_FRAMEBUFFER_EFFECTS

    #define RGB_MATRIX_DEFAULT_HUE 25
    #define RGB_MATRIX_DEFAULT_SAT 255
    #define RGB_MATRIX_DEFAULT_VAL 255 //RGB_MATRIX_MAXIMUM_BRIGHTNESS
    #define RGB_MATRIX_DEFAULT_SPD 127

#endif

#define MOUSEKEY_INTERVAL 4 // 20
#define MOUSEKEY_MOVE_DELTA 15 // 20
#define MOUSEKEY_TIME_TO_MAX 30 // 30
#define MOUSEKEY_MAX_SPEED 2 // 10

#define MOUSEKEY_WHEEL_DELAY 100 // 10
#define MOUSEKEY_WHEEL_INTERVAL 100 // 80
#define MOUSEKEY_WHEEL_MAX_SPEED 4
#define MOUSEKEY_WHEEL_TIME_TO_MAX 50

#define LED_HITS_TO_REMEMBER 16 // default 8