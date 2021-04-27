/*
Copyright 2018 Mattia Dal Ben <matthewdibi@gmail.com>

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

#pragma once


#define AUTO_SHIFT_TIMEOUT 250
#define TAPPING_TERM 200

#define AUDIO_PIN C6
#undef TEMPO_DEFAULT
#define TEMPO_DEFAULT 100
#define NO_MUSIC_MODE
#define USE_SERIAL

// mousekeys
#define MOUSEKEY_INTERVAL 16
#define MOUSEKEY_MAX_SPEED 5
#define MOUSEKEY_TIME_TO_MAX 25
#define MOUSEKEY_DELAY 150

// encoders
#define ENCODERS_PAD_A { F4 }
#define ENCODERS_PAD_B { D1 }
#define ENCODER_RESOLUTION 4
#define ENCODER_DIRECTION_FLIP

// #define ENCODER_DIRECTION_FLIP

#undef DEBOUNCE
#define DEBOUNCE 2

//#define USE_I2C

/* Select hand configuration */
#define MASTER_LEFT
// #define MASTER_RIGHT
// #define EE_HANDS

#undef RGBLED_NUM
#define RGBLIGHT_ANIMATIONS
#define RGBLED_NUM 14
#define RGBLIGHT_HUE_STEP 8
#define RGBLIGHT_SAT_STEP 8
#define RGBLIGHT_VAL_STEP 8
