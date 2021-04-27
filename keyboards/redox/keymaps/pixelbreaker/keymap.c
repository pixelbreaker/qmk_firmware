// make redox:pixelbreaker:avrdude AUDIO_ENABLE=yes

#include QMK_KEYBOARD_H
#ifdef AUDIO_ENABLE
  #include "audio.h"
#endif

extern keymap_config_t keymap_config;

// Layer names
#define _QWERTY 0
#define _SYMB 1
#define _NAV 2
#define _MOUSE 3
#define _TOGGLE 4
#define _GAMING 5
#define _LOCK 6
#define _UNLOCK 7
#define _RESET 8
#define _NUMPAD 9

enum my_keycodes {
  MOUSE = SAFE_RANGE,
  SYMBOL,
  NAV,
  MACSLEEP,
  GAMING,
  ENC_PLAY,
};

#undef COIN_SOUND
#define COIN_SOUND \
    E__NOTE(_A5),      \
    HD_NOTE(_E6),

#define MT_SOUND \
    M__NOTE(_A2,10)

#undef ONE_UP_SOUND
#define ONE_UP_SOUND \
    Q__NOTE(_E6  ),  \
    Q__NOTE(_G6  ),  \
    Q__NOTE(_E7  ),  \
    Q__NOTE(_C7  ),  \
    Q__NOTE(_D7  ),  \
    Q__NOTE(_G7  ),

#undef SONIC_RING
#define SONIC_RING \
    E__NOTE(_E6),  \
    E__NOTE(_G6),  \
    HD_NOTE(_C7),


#ifdef AUDIO_ENABLE
    float startup[][2] = SONG(ONE_UP_SOUND);
    float raise_low[][2] = SONG(MT_SOUND);
    float gaming_on[][2] = SONG(NUM_LOCK_ON_SOUND);
    float gaming_off[][2] = SONG(NUM_LOCK_OFF_SOUND);
    float adjust[][2] = SONG(COIN_SOUND);
    float my_song[][2] = SONG(NO_SOUND);
    float tone_caps_on[][2] = SONG(CAPS_LOCK_ON_SOUND);
    float tone_caps_off[][2] = SONG(CAPS_LOCK_OFF_SOUND);
    float sleep[][2] = SONG(PLOVER_GOODBYE_SOUND);
#endif

//Tap Dance Declarations
enum tap_dance {
  HOME_END,
  SHFT_CAPS,
  THREE_HASH,
};

// Tap dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
  [HOME_END] = ACTION_TAP_DANCE_DOUBLE(KC_HOME, KC_END),
  [SHFT_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_LSFT, KC_CAPS),
  [THREE_HASH] = ACTION_TAP_DANCE_DOUBLE(KC_3, LALT(KC_3)),
};

// Nicer keycode alias for keymap readability
#define x KC_NO // No action
#define _v_ KC_TRNS // Pass through
#define L_SYMB MO(_SYMB)
#define L_NAV LT(_NAV,KC_TAB)
#define L_MOUSE LT(_MOUSE,KC_ESC)
#define L_TOGGLE LT(_TOGGLE,KC_DEL)
#define L_GAMING TG(_GAMING)
#define L_NUMPAD MO(_NUMPAD)
#define L_LOCK TO(_LOCK)
#define L_UNLOCK TO(_QWERTY)
#define HYP_MINS HYPR_T(KC_MINS)
#define MEH_PLUS MEH_T(KC_PPLS)
#define CUT LGUI(KC_X)
#define COPY LGUI(KC_C)
#define PASTE LGUI(KC_V)
#define UNDO LGUI(KC_Z)
#define REDO LSFT(LGUI(KC_Z))

// VSCODE keys
#define CONSOLE LCTL(KC_C)
#define DEL_LINE MEH(KC_K)
#define LINE_DOWN LALT(KC_DOWN)
#define LINE_UP LALT(KC_UP)
#define PANE_1 LGUI(KC_1)
#define PANE_2 LGUI(KC_2)
#define PANE_3 LGUI(KC_3)
#define SORT_LINES KC_F5

// Gaming
#define SCRSHT LSFT(KC_P5)
#define STATS MEH(KC_8)

bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;

void matrix_init_user(void) {
    // frame_timer = timer_read();
    #ifdef AUDIO_ENABLE
        set_voice(default_voice);
        startup_user();
    #endif
}

#ifdef AUDIO_ENABLE
    void startup_user()
    {
        _delay_ms(120); // gets rid of tick
        PLAY_SONG(startup);
    }

    void shutdown_user()
    {
        _delay_ms(150);
        stop_all_notes();
    }

    void led_set_user(uint8_t usb_led)
    {
        static uint8_t old_usb_led = 0;

        _delay_ms(10); // gets rid of tick

        if ((usb_led & (1<<USB_LED_CAPS_LOCK)) && !(old_usb_led & (1<<USB_LED_CAPS_LOCK)))
        {
                // If CAPS LK LED is turning on...
                PLAY_SONG(tone_caps_on);
        }
        else if (!(usb_led & (1<<USB_LED_CAPS_LOCK)) && (old_usb_led & (1<<USB_LED_CAPS_LOCK)))
        {
                // If CAPS LK LED is turning off...
                PLAY_SONG(tone_caps_off);
        }

        old_usb_led = usb_led;
    }
#endif

bool enc_used = false;
bool enc_skip = false;

#ifdef ENCODER_ENABLE
    void encoder_update_user(uint8_t index, bool clockwise) {
        if (index == 1) {
            enc_used = true;
            if(enc_skip) {
                if (clockwise) {
                    tap_code16(KC_MNXT);
                } else {
                    tap_code16(KC_MPRV);
                }
            } else if (IS_LAYER_ON(_TOGGLE)) { // cycle windows
                if (clockwise) {
                    if (!is_alt_tab_active) {
                        is_alt_tab_active = true;
                        register_code(KC_LGUI);
                    }
                    alt_tab_timer = timer_read();
                    unregister_code(KC_LSFT);
                    tap_code(KC_TAB);
                } else {
                    if (!is_alt_tab_active) {
                        is_alt_tab_active = true;
                        register_code(KC_LGUI);
                    }
                    alt_tab_timer = timer_read();
                    register_code(KC_LSFT);
                    tap_code(KC_TAB);
                }
            } else if(IS_LAYER_ON(_SYMB)) {
                if (clockwise) {
                    tap_code16(KC_RGHT);
                } else {
                    tap_code16(KC_LEFT);
                }
            } else if(IS_LAYER_ON(_NAV)) {
                if (clockwise) {
                    tap_code16(KC_DOWN);
                } else {
                    tap_code16(KC_UP);
                }
            } else if(IS_LAYER_ON(_MOUSE)) {
                if (clockwise) {
                    tap_code16(KC_WH_D);
                } else {
                    tap_code16(KC_WH_U);
                }
            } else {
                if (clockwise) {
                    tap_code(KC_VOLU);
                } else {
                    tap_code(KC_VOLD);
                }
            }
        }
    }
#endif

void matrix_scan_user(void) {
  if (is_alt_tab_active) {
    if (timer_elapsed(alt_tab_timer) > 1000) {
      unregister_code(KC_LGUI);
      unregister_code(KC_LSFT);
      is_alt_tab_active = false;
    }
  }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case GAMING:
        if (record->event.pressed) {
            if (IS_LAYER_ON(_GAMING)) {
                layer_off(_GAMING);
                #ifdef AUDIO_ENABLE
                    PLAY_SONG (gaming_off);
                #endif
                #ifdef AUTO_SHIFT_ENABLE
                    autoshift_enable();
                #endif
            } else {
                layer_on(_GAMING);
                #ifdef AUDIO_ENABLE
                    PLAY_SONG (gaming_on);
                #endif
                #ifdef AUTO_SHIFT_ENABLE
                    autoshift_disable();
                #endif
            }
        }
        return false;
    break;
    case SYMBOL:
        if (record->event.pressed) {
            layer_on(_SYMB);
            #ifdef AUDIO_ENABLE
                PLAY_SONG (raise_low);
            #endif
        } else {
            layer_off(_SYMB);
            #ifdef AUDIO_ENABLE
                PLAY_SONG (raise_low);
            #endif
        }
        return false;
    break;
    case MACSLEEP:
        if (record->event.pressed) {
            register_code(KC_RSFT);
            register_code(KC_RCTL);
            register_code(KC_POWER);
            unregister_code(KC_POWER);
            unregister_code(KC_RCTL);
            unregister_code(KC_RSFT);

            PLAY_SONG(sleep);
        }
      return false;
    break;
    case ENC_PLAY:
        if (record->event.pressed) {
            enc_skip = true;
            enc_used = false;
        } else {
            enc_skip = false;
            if (!enc_used) {
                tap_code(KC_MPLY);
            }
        }
        return false;
    break;
  }
  return true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT(
    //┌────────┬────────┬────────┬────────┬────────┬────────┐                                           ┌────────┬────────┬────────┬────────┬────────┬────────┐
      L_MOUSE, KC_1,    KC_2, TD(THREE_HASH),KC_4, KC_5,                                                KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSLS,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                         ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      L_NAV,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_MINS,                           KC_EQL,  KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_GRV,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                         ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      SYMBOL,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_BSLS,                            KC_QUOT, KC_H,   KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      KC_LSPO, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    L_TOGGLE,TD(HOME_END),    KC_PGUP,  ENC_PLAY, KC_N,    KC_M,   KC_COMM, KC_DOT,  KC_SLSH, KC_RSPC,
    //├────────┼────────┼────────┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
      KC_UP,   KC_DOWN, KC_LCTL, KC_LALT,      KC_LGUI,     KC_SPC,  KC_ENT,          TD(SHFT_CAPS),KC_BSPC, MO(_NAV),    KC_RALT, KC_RCTL,   KC_LEFT, KC_RGHT
    //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘       └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
    ),

    [_SYMB] = LAYOUT(
    //┌────────┬────────┬────────┬────────┬────────┬────────┐                                           ┌────────┬────────┬────────┬────────┬────────┬────────┐
      x,       KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                                               KC_F6,   KC_LBRC, KC_RBRC, KC_F9,   KC_F10,  KC_F11,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                         ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      x,       x,       x,       x,       x,       CONSOLE, _v_,                               _v_,     KC_EXLM, KC_LPRN, KC_RPRN, KC_DLR,  KC_PERC, KC_F12,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                         ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      _v_,     x,       PANE_1,  PANE_2,  PANE_3,  x,       _v_,                               KC_UNDS, KC_PIPE, KC_LCBR, KC_RCBR, KC_EQL,  KC_COLN, KC_DQUO,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      _v_,     x,       CUT,     COPY,    PASTE,   x,       _v_,     _v_,             LINE_UP, LINE_DOWN,KC_AMPR,KC_LT,   KC_GT,   KC_MINS, KC_SLSH, KC_GRV,
    //├────────┼────────┼────────┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
      _v_,       x,       _v_,    _v_,         _v_,         _v_,   KC_ENT,          SORT_LINES,DEL_LINE,  _v_,          _v_,     _v_,     _v_,     _v_
    //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘       └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
    ),

    [_NAV] = LAYOUT(
    //┌────────┬────────┬────────┬────────┬────────┬────────┐                                           ┌────────┬────────┬────────┬────────┬────────┬────────┐
      _v_,     _v_,     _v_,     _v_,     _v_,     _v_,                                                 _v_,     KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLU,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                         ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      _v_,     _v_,     _v_,     _v_,     _v_,     _v_,     _v_,                               _v_,     LINE_UP,  _v_,    KC_UP,   _v_,     _v_,     KC_VOLD,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                         ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      KC_CAPS, _v_,     _v_,     _v_,     _v_,     _v_,     _v_,                               _v_,     LINE_DOWN, KC_LEFT, KC_DOWN, KC_RGHT, _v_, KC_MPLY,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      _v_,     _v_,     _v_,     _v_,     _v_,     _v_,     _v_,     _v_,             _v_,     _v_,     _v_,  LALT(KC_LEFT),_v_,  LALT(KC_RGHT),_v_, _v_,
    //├────────┼────────┼────────┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
      _v_,     _v_,     _v_,     _v_,          _v_,         _v_,     _v_,             _v_,     _v_,         _v_,          _v_,     _v_,     _v_,     _v_
    //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘       └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
    ),

    [_NUMPAD] = LAYOUT(
    //┌────────┬────────┬────────┬────────┬────────┬────────┐                                           ┌────────┬────────┬────────┬────────┬────────┬────────┐
      _v_,     _v_,     _v_,     _v_,     _v_,     _v_,                                                 _v_,     KC_7,    KC_8,    KC_9,    KC_0,    _v_,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                         ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      _v_,     _v_,     _v_,     _v_,     _v_,     _v_,     _v_,                               _v_,     _v_,     KC_4,    KC_5,    KC_6,     _v_,    _v_,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                         ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      _v_,     _v_,     _v_,     _v_,     _v_,     _v_,     _v_,                                _v_,    _v_,     KC_1,    KC_2,    KC_3,    _v_,     _v_,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      _v_,     _v_,     _v_,     _v_,     _v_,     _v_,     _v_,     _v_,             _v_,     _v_,     _v_,     _v_,    KC_0,     _v_,     _v_,     _v_,
    //├────────┼────────┼────────┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
      _v_,     _v_,     _v_,     _v_,          _v_,         _v_,     _v_,             _v_,     _v_,         _v_,          _v_,     _v_,     _v_,     _v_
    //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘       └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
    ),


    [_MOUSE] = LAYOUT(
    //┌────────┬────────┬────────┬────────┬────────┬────────┐                                           ┌────────┬────────┬────────┬────────┬────────┬────────┐
      _v_,     x,      x,       x,       x,       x,                                                    x,       KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLU,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                         ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      L_NUMPAD,x,       x,       KC_WH_U, x,       x,       x,                                  x,      KC_WH_U, x,       KC_MS_U,  x,      x,       KC_VOLD,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                         ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      x,       x,       KC_WH_L, KC_WH_D, KC_WH_R, x,       x,                                  x,      KC_WH_D, KC_MS_L, KC_MS_D, KC_MS_R, x,       KC_MPLY,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      x,       x,       x,       x,       x,       x,       x,       x,               x,       KC_BTN1, x,       x,       x,       x,       x,       x,
    //├────────┼────────┼────────┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
      x,       x,       x,       x,            x,           KC_BTN1, KC_BTN2,         x,       x,           x,            x,       x,       x,       x
    //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘       └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
    ),

    [_TOGGLE] = LAYOUT(
    //┌────────┬────────┬────────┬────────┬────────┬────────┐                                           ┌────────┬────────┬────────┬────────┬────────┬────────┐
      x,       KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                                               KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  L_LOCK,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                         ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      x,       AU_ON,   x,       x,       x,       x,       MACSLEEP,                          x,       x,       x,       x,       x,       x,       x,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                         ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      x,       AU_TOG,  x,       x,       x,       GAMING,  x,                                 x,       x,       x,       x,       x,       x,       x,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      x,       AU_OFF,  x,       x,       x,       x,       _v_,     MO(_RESET),      x,       x,       x,       x,       x,       x,       x,       x,
    //├────────┼────────┼────────┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
      x,       x,       x,       x,            x,           x,       x,               x,       x,           x,            x,       x,       x,       x
    //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘       └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
    ),

    [_GAMING] = LAYOUT(
    //┌────────┬────────┬────────┬────────┬────────┬────────┐                                           ┌────────┬────────┬────────┬────────┬────────┬────────┐
      KC_ESC,  _v_,     _v_,     _v_,     _v_,     _v_,                                                 _v_,     _v_,     _v_,     _v_,     _v_,     KC_VOLU,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                         ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      KC_TAB,  _v_,     _v_,     _v_,     _v_,     _v_,     KC_Y,                              _v_,     _v_,     _v_,     _v_,     _v_,     _v_,     KC_VOLD,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                         ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      KC_CAPS, _v_,     _v_,     _v_,     _v_,     _v_,     KC_H,                              _v_,     _v_,     _v_,     _v_,     _v_,     _v_,     _v_,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      KC_LSFT, _v_,     _v_,     _v_,     _v_,     _v_,     _v_,      _v_,            SCRSHT,     STATS,     _v_,     _v_,     _v_,     _v_,     _v_,     _v_,
    //├────────┼────────┼────────┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
      KC_LCTL, KC_LGUI, _v_,     _v_,          KC_LCTL,     _v_,     KC_ENT,          KC_DEL,  _v_,         KC_RCTL,          _v_,     _v_,     _v_,     _v_
    //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘       └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
    ),

    [_LOCK] = LAYOUT(
    //┌────────┬────────┬────────┬────────┬────────┬────────┐                                           ┌────────┬────────┬────────┬────────┬────────┬────────┐
      x,       x,       x,       x,       x,       x,                                                   x,       x,       x,       x,       x,       x,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                         ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      x,       x,       x,       x,       x,       x,       x,                                 x,       x,       x,       x,       x,       x,       x,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                         ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      x,       x,       x,       x,       x,       x,       x,                                 x,       x,       x,       x,       x,       x,       x,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      x,       x,       x,       x,       x,       x,       MO(_UNLOCK),x,            x,       x,       x,       x,       x,       x,       x,       x,
    //├────────┼────────┼────────┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
      x,       x,       x,       x,            x,           x,       x,               x,       x,           x,            x,       x,       x,       x
    //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘       └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
    ),

    [_UNLOCK] = LAYOUT(
    //┌────────┬────────┬────────┬────────┬────────┬────────┐                                           ┌────────┬────────┬────────┬────────┬────────┬────────┐
      x,       x,       x,       x,       x,       x,                                                   x,       x,       x,       x,       x,       L_UNLOCK,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                         ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      x,       x,       x,       x,       x,       x,       x,                                 x,       x,       x,       x,       x,       x,       x,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                         ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      x,       x,       x,       x,       x,       x,       x,                                 x,       x,       x,       x,       x,       x,       x,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      x,       x,       x,       x,       x,       x,       _v_,     x,               x,       x,       x,       x,       x,       x,       x,       x,
    //├────────┼────────┼────────┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
      x,       x,       x,       x,            x,           x,       x,               x,       x,           x,            x,       x,       x,       x
    //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘       └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
    ),

    [_RESET] = LAYOUT(
    //┌────────┬────────┬────────┬────────┬────────┬────────┐                                           ┌────────┬────────┬────────┬────────┬────────┬────────┐
      RESET,   x,       x,       x,       x,       x,                                                   x,       x,       x,       x,       x,       x,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                         ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      x,       x,       x,       x,       x,       x,       x,                                 x,       x,       x,       x,       x,       x,       x,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                         ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      x,       x,       x,       x,       x,       x,       x,                                 x,       x,       x,       x,       x,       x,       x,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      x,       x,       x,       x,       x,       x,       _v_,     _v_,             x,       x,       x,       x,       x,       x,       x,       x,
    //├────────┼────────┼────────┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
      EEPROM_RESET,       x,       x,       x,            x,           x,       x,               x,       x,           x,            x,       x,       x,       x
    //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘       └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
    ),
};


