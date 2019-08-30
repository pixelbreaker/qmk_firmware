#include QMK_KEYBOARD_H

extern keymap_config_t keymap_config;

// Layer namesg
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
};

// Nicer keycode alias for keymap readability
#define x KC_NO // No action
#define _v_ KC_TRNS // Pass through
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

// Gaming
#define SCRSHT MEH(KC_PSCR)
#define STATS MEH(8)

uint16_t frame_timer;
uint16_t anim_timer;
uint16_t anim_frame = 0;
int anim_dir = 1;
bool anim_run = true;
bool locked = false;
bool locked_light = true;

// Init and switch off the "white" 4th LED
void led_init_user(void) {
	DDRD  |= (1<<0) | (1<<1);
	PORTD |= (1<<0) | (1<<1);
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

    frame_timer = timer_read();
    anim_timer = timer_read();
    anim_frame = 0;
}

void matrix_scan_user(void) {
    if (anim_run && timer_elapsed(anim_timer) < 1440) {
        if (timer_elapsed(frame_timer) > 180) {
            switch (anim_frame) {
                case 0:
                    red_led_on; blu_led_off; grn_led_off; wht_led_off;
                    break;
                case 1:
                    red_led_on; blu_led_on; grn_led_off; wht_led_off;
                    break;
                case 2:
                    red_led_on; blu_led_on; grn_led_on; wht_led_off;
                    break;
                case 3:
                    red_led_on; blu_led_on; grn_led_on; wht_led_on;
                    break;
            }

            anim_frame = anim_frame + anim_dir;
            if (anim_dir > 0 && anim_frame == 3) anim_dir = -anim_dir;
            else if(anim_dir < 0 && anim_frame == 0) anim_dir = -anim_dir;
            frame_timer = timer_read();
        }
    } else if (anim_run) {
        anim_run = false;
        red_led_off; blu_led_off; grn_led_off; wht_led_off;
    } else if (locked && timer_elapsed(frame_timer) > 500) {
      if (locked_light) {
        set_led_red;
      } else {
        set_led_off;
      }
      locked_light = !locked_light;
      frame_timer = timer_read();
    }
}

// Change the receiver's LEDs based on the current layer
layer_state_t layer_state_set_user(layer_state_t state) {
    anim_run = false;

    switch (get_highest_layer(state)) {
        case _QWERTY:
            locked = false;
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
            locked = true;
            frame_timer = timer_read();
            break;
        default:
            break;
    }
  return state;
}

// Tap dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_PRNSL] = ACTION_TAP_DANCE_DOUBLE(KC_LBRC, KC_LCBR), // [[ -> {
  [TD_PRNSR] = ACTION_TAP_DANCE_DOUBLE(KC_RBRC, KC_RCBR), // ]] -> }
  [TD_PIPE] = ACTION_TAP_DANCE_DOUBLE(KC_BSLS, KC_PIPE), // \\ -> |
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
      KC_LSPO, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    L_TOGGLE,KC_END,          KC_PGUP,  KC_PGDN, KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSPC,
    //├────────┼────────┼────────┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
      KC_LCTL, HYP_MINS,MEH_PLUS,KC_LALT,      KC_LGUI,     KC_SPC,  KC_DEL,         KC_BSPC, KC_ENT,      KC_LGUI,      KC_LEFT, KC_UP,   KC_DOWN, KC_RGHT
    //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘       └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
    ),

    [_SYMB] = LAYOUT(
    //┌────────┬────────┬────────┬────────┬────────┬────────┐                                           ┌────────┬────────┬────────┬────────┬────────┬────────┐
      x,       KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                                               KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                         ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      x,       x,       x,       x,       x,       x,       _v_,                               _v_,     KC_EXLM, KC_LPRN, KC_RPRN, KC_DLR,  KC_PERC, KC_F12,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                         ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      _v_,     x,       PANE_1,  PANE_2,  PANE_3,  x,       _v_,                               KC_UNDS, KC_PIPE, KC_LCBR, KC_RCBR, KC_EQL,  KC_COLN, KC_DQUO,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      _v_,     x,       x,       CONSOLE, x,       x,       _v_,     _v_,             LINE_UP, LINE_DOWN,KC_AMPR,KC_LT,   KC_GT,   KC_MINS, KC_SLSH, KC_GRV,
    //├────────┼────────┼────────┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
      x,       x,       REDO,    UNDO,         CUT,         COPY,    PASTE,           DEL_LINE,SORT_LINES,  x,            _v_,     _v_,     _v_,     _v_
    //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘       └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
    ),

    [_NAV] = LAYOUT(
    //┌────────┬────────┬────────┬────────┬────────┬────────┐                                           ┌────────┬────────┬────────┬────────┬────────┬────────┐
      x,       x,       x,       x,       x,       x,                                                   x,       KC_MRWD, KC_MPLY, KC_MFFD, KC__MUTE,KC_VOLU,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                         ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      _v_,     x,       x,       x,       x,       x,       x,                                 x,       x,       x,       KC_UP,   x,       x,       KC_VOLD,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                         ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      x,       x,       x,       x,       x,       x,       x,                                  x,      x,       KC_LEFT, KC_DOWN, KC_RGHT, x,       KC_MPLY,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      x,       x,       x,       x,       x,       x,       x,       x,               x,       x,       x,    LALT(KC_LEFT),x,    LALT(KC_RGHT),x,   x,
    //├────────┼────────┼────────┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
      x,       x,       x,       x,            x,           x,       x,               x,       x,           x,            x,       x,       x,       x
    //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘       └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
    ),

    [_MOUSE] = LAYOUT(
    //┌────────┬────────┬────────┬────────┬────────┬────────┐                                           ┌────────┬────────┬────────┬────────┬────────┬────────┐
      _v_,     x,      x,       x,       x,       x,                                                    x,       KC_MRWD, KC_MPLY, KC_MFFD, KC__MUTE,KC_VOLU,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                         ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      x,       x,       x,       KC_WH_U, x,       x,       x,                                  x,      x,       x,       KC_MS_U,  x,      x,       KC_VOLD,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                         ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      x,       x,       KC_WH_L, KC_WH_D, KC_WH_R, x,       x,                                  x,      x,       KC_MS_L, KC_MS_D, KC_MS_R, x,       KC_MPLY,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      x,       x,       x,       x,       x,       x,       x,       x,               x,       x,       x,       x,       x,       x,       x,       x,
    //├────────┼────────┼────────┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
      x,       x,       x,       x,            x,           KC_BTN1, KC_BTN2,         x,       x,           x,            x,       x,       x,       x
    //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘       └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
    ),

    [_TOGGLE] = LAYOUT(
    //┌────────┬────────┬────────┬────────┬────────┬────────┐                                           ┌────────┬────────┬────────┬────────┬────────┬────────┐
      x,       KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                                               KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  L_LOCK,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                         ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      x,       x,       x,       x,       x,       x,       x,                                 x,       x,       x,       x,       x,       x,       x,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                         ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      x,       x,       x,       x,       x,       L_GAMING,x,                                 x,       x,       x,       x,       x,       x,       x,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      x,       x,       x,       x,       x,       x,       _v_,     MO(_RESET),      x,       x,       x,       x,       x,       x,       x,       x,
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
      KC_CAPS, _v_,     _v_,     _v_,     _v_,     _v_,     KC_H,                              _v_,     _v_,     _v_,     _v_,     _v_,     _v_,     SCRSHT,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      KC_LSFT, _v_,     _v_,     _v_,     _v_,     _v_,     _v_,      _v_,            _v_,     _v_,     _v_,     _v_,     _v_,     _v_,     _v_,     STATS,
    //├────────┼────────┼────────┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
      KC_LCTL, KC_LGUI, _v_,     _v_,          KC_LCTL,     _v_,     _v_,             _v_,     _v_,         _v_,          _v_,     _v_,     _v_,     _v_
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
      DEBUG,   x,       x,       x,       x,       x,       x,                                 x,       x,       x,       x,       x,       x,       x,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                         ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      x,       x,       x,       x,       x,       x,       x,                                 x,       x,       x,       x,       x,       x,       x,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
      x,       x,       x,       x,       x,       x,       _v_,     _v_,             x,       x,       x,       x,       x,       x,       x,       x,
    //├────────┼────────┼────────┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
      x,       x,       x,       x,            x,           x,       x,               x,       x,           x,            x,       x,       x,       x
    //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘       └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
    ),
};


