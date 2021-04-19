#include QMK_KEYBOARD_H

extern keymap_config_t keymap_config;

#define _QWERTY 0
#define _SYMB 1
#define _NAV 2
#define _MOUSE 3
#define _TOGGLE 4
#define _GAMING 5
#define _LOCK 6
#define _UNLOCK 7
#define _RESET 8

//Tap Dance Declarations
enum tap_dance {
  TD_PRNSL,
  TD_PRNSR,
  TD_PIPE,
  TD_QMRK
};

// Nicer keycode alias for keymap readability
#define xxxxx KC_NO
#define L_SYMB LT(_SYMB,KC_CAPSLOCK)
#define L_NAV LT(_NAV,KC_TAB)
#define L_MOUSE LT(_MOUSE,KC_ESC)
#define L_TOGGLE LT(_TOGGLE,KC_HOME)
#define L_GAMING TG(_GAMING)
#define L_LOCK TO(_LOCK)
#define L_UNLOCK TO(_QWERTY)
#define HYP_MINS ALL_T(KC_MINS)
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

// Init and switch off the "white" 4th LED
void led_init_user(void) {
	DDRD  |= (1<<0) | (1<<1);
	PORTD |= (1<<0) | (1<<1);
	DDRF  |= (1<<4) | (1<<5);
	PORTF |= (1<<4) | (1<<5);
}

// Turn the white LED on/off with CAPSLOCK state
void led_set_user(uint8_t usb_led) {
    if (usb_led & (1<<USB_LED_CAPS_LOCK)) {
        wht_led_on;
    } else {
        wht_led_off;
    }
}

void matrix_init_user(void) {
    led_init_user();
}

// Change the receiver's LEDs based on the current layer
layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
        case _QWERTY:
            set_led_off;
            break;
        case _SYMB:
            set_led_green;
            break;
        case _NAV:
            set_led_blue;
            break;
        case _MOUSE:
            set_led_red;
            break;
        case _TOGGLE:
            set_led_green;
            break;
        case _GAMING:
            set_led_yellow;
            break;
        case _LOCK:
            set_led_red;
            break;
        default:
            break;
    }
  return state;
}

// Tap dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_PRNSL] = ACTION_TAP_DANCE_DOUBLE(KC_LBRC, KC_LCBR),
  [TD_PRNSR] = ACTION_TAP_DANCE_DOUBLE(KC_RBRC, KC_RCBR),
  [TD_PIPE] = ACTION_TAP_DANCE_DOUBLE(KC_BSLS, KC_PIPE),
  [TD_QMRK] = ACTION_TAP_DANCE_DOUBLE(KC_SLSH, KC_QUES),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT(
    //┌────────┬────────┬────────┬────────┬────────┬────────┐                                           ┌────────┬────────┬────────┬────────┬────────┬────────┐
      L_MOUSE, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                                                KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_GRV,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                         ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      L_NAV,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_MINS,                           KC_EQL,  KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,  TD(TD_PIPE),
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                         ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      L_SYMB,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,  TD(TD_PRNSL),                      TD(TD_PRNSR), KC_H, KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      KC_LSPO, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,  L_TOGGLE,KC_END,            KC_PGUP, KC_PGDN, KC_N,    KC_M,    KC_COMM, KC_DOT,TD(TD_QMRK),KC_RSPC,
    //├────────┼────────┼────────┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
      KC_LCTL, HYP_MINS,MEH_PLUS,KC_LALT,      KC_LGUI,     KC_SPC,  KC_DEL,          KC_BSPC, KC_ENT,      KC_LGUI,      KC_LEFT, KC_UP,   KC_DOWN, KC_RGHT
    //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘       └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
    ),

    [_SYMB] = LAYOUT(
    //┌────────┬────────┬────────┬────────┬────────┬────────┐                                           ┌────────┬────────┬────────┬────────┬────────┬────────┐
      xxxxx,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                                               KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                         ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   _______,                           _______, KC_EXLM, KC_LPRN, KC_RPRN, KC_DLR,  KC_PERC, KC_F12,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                         ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      _______, xxxxx,   PANE_1,  PANE_2,  PANE_3,  xxxxx,   _______,                           KC_UNDS, KC_PIPE, KC_LCBR, KC_RCBR, KC_EQL,  KC_COLN, KC_DQUO,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      _______, xxxxx,   xxxxx,   CONSOLE, xxxxx,   xxxxx,   _______, _______,         LINE_UP, LINE_DOWN,KC_AMPR,KC_LT,KC_GT,KC_SLSH, xxxxx,   KC_GRV,
    //├────────┼────────┼────────┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
      xxxxx,   xxxxx,   REDO,    UNDO,         CUT,         COPY,    PASTE,           DEL_LINE,SORT_LINES,  xxxxx,        _______, _______, _______, _______
    //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘       └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
    ),

    [_NAV] = LAYOUT(
    //┌────────┬────────┬────────┬────────┬────────┬────────┐                                           ┌────────┬────────┬────────┬────────┬────────┬────────┐
      xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,                                               xxxxx,   KC_MRWD, KC_MPLY, KC_MFFD, KC__MUTE,KC_VOLU,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                         ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      _______, xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,                              xxxxx,  xxxxx,   xxxxx,   KC_UP,   xxxxx,   xxxxx,   KC_VOLD,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                         ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,                              xxxxx,  xxxxx,   KC_LEFT, KC_DOWN, KC_RGHT, xxxxx,   KC_MPLY,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,           xxxxx,   xxxxx,   xxxxx,LALT(KC_LEFT),xxxxx,LALT(KC_RGHT),xxxxx,xxxxx,
    //├────────┼────────┼────────┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
      xxxxx,   xxxxx,   xxxxx,   xxxxx,        xxxxx,       xxxxx,   xxxxx,           xxxxx,   xxxxx,       xxxxx,        xxxxx,   xxxxx,   xxxxx,   xxxxx
    //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘       └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
    ),

    [_MOUSE] = LAYOUT(
    //┌────────┬────────┬────────┬────────┬────────┬────────┐                                           ┌────────┬────────┬────────┬────────┬────────┬────────┐
      _______, xxxxx,  xxxxx,   xxxxx,   xxxxx,   xxxxx,                                                xxxxx,   KC_MRWD, KC_MPLY, KC_MFFD, KC__MUTE,KC_VOLU,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                         ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      xxxxx,   xxxxx,   xxxxx,   KC_WH_U, xxxxx,   xxxxx,   xxxxx,                              xxxxx,  xxxxx,   xxxxx,   KC_MS_U,  xxxxx,  xxxxx,   KC_VOLD,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                         ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      xxxxx,   xxxxx,   KC_WH_L, KC_WH_D, KC_WH_R, xxxxx,   xxxxx,                              xxxxx,  xxxxx,   KC_MS_L, KC_MS_D, KC_MS_R, xxxxx,   KC_MPLY,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,           xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,
    //├────────┼────────┼────────┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
      xxxxx,   xxxxx,   xxxxx,   xxxxx,        xxxxx,       KC_BTN1, KC_BTN2,         xxxxx,   xxxxx,       xxxxx,        xxxxx,   xxxxx,   xxxxx,   xxxxx
    //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘       └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
    ),

    [_TOGGLE] = LAYOUT(
    //┌────────┬────────┬────────┬────────┬────────┬────────┐                                           ┌────────┬────────┬────────┬────────┬────────┬────────┐
      xxxxx,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                                               KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  L_LOCK,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                         ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,                             xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                         ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   L_GAMING,xxxxx,                             xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   _______, MO(_RESET),      xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,
    //├────────┼────────┼────────┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
      xxxxx,   xxxxx,   xxxxx,   xxxxx,        xxxxx,       xxxxx,   xxxxx,           xxxxx,   xxxxx,       xxxxx,        xxxxx,   xxxxx,   xxxxx,   xxxxx
    //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘       └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
    ),

    [_GAMING] = LAYOUT(
    //┌────────┬────────┬────────┬────────┬────────┬────────┐                                           ┌────────┬────────┬────────┬────────┬────────┬────────┐
      KC_ESC,  _______, _______, _______, _______, _______,                                             _______, _______, _______, _______, _______, _______,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                         ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      KC_TAB,  _______, _______, _______, _______, _______, KC_Y,                              _______, _______, _______, _______, _______, _______, _______,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                         ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      KC_CAPS, _______, _______, _______, _______, _______, KC_H,                              _______, _______, _______, _______, _______, _______, _______,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      KC_LSFT, _______, _______, _______, _______, _______, _______, _______,         _______, _______, _______, _______, _______, _______, _______, _______,
    //├────────┼────────┼────────┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
      _______, _______, _______, _______,      _______,     _______, _______,         _______, _______,     _______,      _______, _______, _______, _______
    //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘       └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
    ),

    [_LOCK] = LAYOUT(
    //┌────────┬────────┬────────┬────────┬────────┬────────┐                                           ┌────────┬────────┬────────┬────────┬────────┬────────┐
      xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,                                               xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                         ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,                             xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                         ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,                             xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   MO(_UNLOCK),xxxxx,        xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,
    //├────────┼────────┼────────┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
      xxxxx,   xxxxx,   xxxxx,   xxxxx,        xxxxx,       xxxxx,   xxxxx,           xxxxx,   xxxxx,       xxxxx,        xxxxx,   xxxxx,   xxxxx,   xxxxx
    //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘       └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
    ),

    [_UNLOCK] = LAYOUT(
    //┌────────┬────────┬────────┬────────┬────────┬────────┐                                           ┌────────┬────────┬────────┬────────┬────────┬────────┐
      xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,                                               xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   L_UNLOCK,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                         ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,                             xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                         ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,                             xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   _______, xxxxx,           xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,
    //├────────┼────────┼────────┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
      xxxxx,   xxxxx,   xxxxx,   xxxxx,        xxxxx,       xxxxx,   xxxxx,           xxxxx,   xxxxx,       xxxxx,        xxxxx,   xxxxx,   xxxxx,   xxxxx
    //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘       └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
    ),

    [_RESET] = LAYOUT(
    //┌────────┬────────┬────────┬────────┬────────┬────────┐                                           ┌────────┬────────┬────────┬────────┬────────┬────────┐
      RESET,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,                                               xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                         ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      DEBUG,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,                             xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                         ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,                             xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   _______, _______,         xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,   xxxxx,
    //├────────┼────────┼────────┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
      xxxxx,   xxxxx,   xxxxx,   xxxxx,        xxxxx,       xxxxx,   xxxxx,           xxxxx,   xxxxx,       xxxxx,        xxxxx,   xxxxx,   xxxxx,   xxxxx
    //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘       └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
    ),
};


