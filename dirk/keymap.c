// clang-format off
#include QMK_KEYBOARD_H
#include "print.h"
static uint8_t current_layer = 0;


typedef struct {
    bool is_press_action;
    int  state;
} tap;

typedef struct {
    uint16_t kc;
} tap_dance_single_t;

typedef struct {
    uint16_t kc;
    uint8_t layer;
} tap_dance_key_layer_pair_t;

typedef struct {
    uint16_t kc1;
    uint16_t kc2;
    uint8_t layer;
} tap_dance_keys_layer_pair_t;

enum {
    SINGLE_TAP        = 1,
    SINGLE_HOLD       = 2,
    DOUBLE_TAP        = 3,
    DOUBLE_HOLD       = 4,
    DOUBLE_SINGLE_TAP = 5, // send two single taps
    TRIPLE_TAP        = 6,
    TRIPLE_HOLD       = 7
};

int  cur_dance(tap_dance_state_t *state);
void tap_dance_finished_key_shift_ctrl(tap_dance_state_t *state, void *user_data);
void tap_dance_reset_key_shift_ctrl(tap_dance_state_t *state, void *user_data);
void tap_dance_finished_key_shift_mo(tap_dance_state_t *state, void *user_data);
void tap_dance_reset_key_shift_mo(tap_dance_state_t *state, void *user_data);
void tap_dance_finished_keys_shift_mo(tap_dance_state_t *state, void *user_data);
void tap_dance_reset_keys_shift_mo(tap_dance_state_t *state, void *user_data);


#define ACTION_TAP_DANCE_SHIFT(kc) \
    { .fn = {tap_dance_pair_on_each_tap, tap_dance_pair_finished, tap_dance_pair_reset}, .user_data = (void *)&((tap_dance_pair_t){kc, LSFT(kc)}), }
#define ACTION_TAP_DANCE_SHIFT_CTRL(kc) \
    { .fn = {NULL, tap_dance_finished_key_shift_ctrl, tap_dance_reset_key_shift_ctrl}, .user_data = (void *)&((tap_dance_single_t){kc}), }
#define ACTION_TAP_DANCE_SHIFT_MO(kc, layer) \
    { .fn = {NULL, tap_dance_finished_key_shift_mo, tap_dance_reset_key_shift_mo}, .user_data = (void *)&((tap_dance_key_layer_pair_t){kc, layer}), }
#define ACTION_TAP_DANCE_KC1_KC2_MO(kc1, kc2, layer) \
    { .fn = {NULL, tap_dance_finished_keys_shift_mo, tap_dance_reset_keys_shift_mo}, .user_data = (void *)&((tap_dance_keys_layer_pair_t){kc1, kc2, layer}), }
#define ACTION_TAP_DANCE_CW_LAYER_TOGGLE(layer) \
    { .fn = {NULL, tap_dance_cw_layer_finished, NULL}, .user_data = (void *)&((tap_dance_dual_role_t){KC_NO, layer, layer_invert}), }

// DEFINITIONS: Shorten long "key"-names
// Layer switches
#define FN_BACK TO(DEFAULT)
#define FN_GAME TO(GAMING_LAYER)
#define FN_KCR MO(KEYCHRON_FUNCTION)
#define FN_MOUSE TG(MOUSE_LAYER)

// CC_<KEYNAME>: [C]trl-[C]ode, for keys that have ctrl activated on hold
// LT() is used to switch layers on hold, but used on layer 0 it does not switch a layer, but provides tap/hold logic
// see: https://docs.qmk.fm/#/mod_tap?id=changing-hold-function
#define CC_NUHS LT(0, KC_NUHS)

// TH_<KEYNAME>: [T]ap-[H]old, for keys that have a tap and a hold action
// Tap/hold logic works only on basic keycodes (https://docs.qmk.fm/#/keycodes_basic)
// #define TH_HM_END LT(0, KC_F24)

enum layers{
  DEFAULT,
  SPACE_FUNCTION,
  LEFT_HOLD_FUNCTION,
  RIGHT_HOLD_FUNCTION,
  KEYCHRON_FUNCTION,
  MOUSE_LAYER,
  GAMING_LAYER
};

enum tap_dance {
  TD_LEFT,
  TD_RIGHT,
  TD_HM_END,
  TD_Q,
  TD_W,
  TD_E,
  TD_R,
  TD_T,
  TD_U,
  TD_I,
  TD_O,
  TD_P,
  TD_LBRC, //Ü
  TD_S,
  TD_G,
  TD_J,
  TD_K,
  TD_L,
  TD_QUOT, //Ä
  TD_B,
  TD_N,
  TD_M,
  TD_COMM,
  TD_DOT,
  TD_C,
  TD_D,
  TD_F,
  TD_H,
  TD_X,
  TD_Y,
  TD_Z,
  TD_V,
  TD_A,
  TD_SCLN,
  TD_SPACE,
};

enum custom_keycodes {
  MACRO_0 = SAFE_RANGE, // { curly brace open
  MACRO_1,              // } curly brace close
  MACRO_2,              // ( parenthesis open
  MACRO_3,              // ) parenthesis  close
  MACRO_4,              // [ square bracket open
  MACRO_5,              // ] square bracket close
  MACRO_6,              // ctrl + plus (jump to start)
  MACRO_7,              // ctrl + alt + plus (jump to end)
  MACRO_8,              // ctrl + shift + plus (jump to and select word (target mode))
  MACRO_9,              // ctrl + alt + shift + plus (jump to declaration)
  CK_STUCK,             // Remove all modifiers (shift, alt, win, ctrl) to prevent stuck modifiers
};

// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] ={
  [TD_LEFT]     = ACTION_TAP_DANCE_DOUBLE(KC_LEFT, LCTL(KC_LEFT)),
  [TD_RIGHT]    = ACTION_TAP_DANCE_DOUBLE(KC_RIGHT, LCTL(KC_RIGHT)),
  [TD_HM_END]   = ACTION_TAP_DANCE_DOUBLE(KC_HOME, KC_END),
  [TD_Q]        = ACTION_TAP_DANCE_SHIFT(KC_Q),
  [TD_W]        = ACTION_TAP_DANCE_SHIFT(KC_W),
  [TD_E]        = ACTION_TAP_DANCE_SHIFT(KC_E),
  [TD_R]        = ACTION_TAP_DANCE_SHIFT(KC_R),
  [TD_T]        = ACTION_TAP_DANCE_SHIFT(KC_T),
  [TD_U]        = ACTION_TAP_DANCE_SHIFT(KC_U),
  [TD_I]        = ACTION_TAP_DANCE_SHIFT(KC_I),
  [TD_O]        = ACTION_TAP_DANCE_SHIFT(KC_O),
  [TD_P]        = ACTION_TAP_DANCE_SHIFT(KC_P),
  [TD_LBRC]     = ACTION_TAP_DANCE_SHIFT(KC_LBRC),
  [TD_S]        = ACTION_TAP_DANCE_SHIFT(KC_S),
  [TD_G]        = ACTION_TAP_DANCE_SHIFT(KC_G),
  [TD_J]        = ACTION_TAP_DANCE_SHIFT(KC_J),
  [TD_K]        = ACTION_TAP_DANCE_SHIFT(KC_K),
  [TD_L]        = ACTION_TAP_DANCE_SHIFT(KC_L),
  [TD_QUOT]     = ACTION_TAP_DANCE_SHIFT(KC_QUOT),
  [TD_B]        = ACTION_TAP_DANCE_SHIFT(KC_B),
  [TD_N]        = ACTION_TAP_DANCE_SHIFT(KC_N),
  [TD_M]        = ACTION_TAP_DANCE_SHIFT(KC_M),
  [TD_COMM]     = ACTION_TAP_DANCE_SHIFT(KC_COMM),
  [TD_DOT]      = ACTION_TAP_DANCE_SHIFT(KC_DOT),
  [TD_X]        = ACTION_TAP_DANCE_SHIFT_CTRL(KC_X),
  [TD_Y]        = ACTION_TAP_DANCE_SHIFT_CTRL(KC_Y),
  [TD_Z]        = ACTION_TAP_DANCE_SHIFT_CTRL(KC_Z),
  [TD_D]        = ACTION_TAP_DANCE_SHIFT_CTRL(KC_D),
  [TD_F]        = ACTION_TAP_DANCE_SHIFT_CTRL(KC_F),
  [TD_H]        = ACTION_TAP_DANCE_SHIFT_CTRL(KC_H),
  [TD_C]        = ACTION_TAP_DANCE_SHIFT_CTRL(KC_C),
  [TD_V]        = ACTION_TAP_DANCE_SHIFT_CTRL(KC_V),
  [TD_A]        = ACTION_TAP_DANCE_SHIFT_MO(KC_A, LEFT_HOLD_FUNCTION),
  [TD_SCLN]     = ACTION_TAP_DANCE_SHIFT_MO(KC_SCLN, RIGHT_HOLD_FUNCTION),
  [TD_SPACE]    = ACTION_TAP_DANCE_KC1_KC2_MO(KC_SPACE, KC_DOT, SPACE_FUNCTION),
};

uint32_t change_color_red_callback(uint32_t trigger_time, void *cb_arg) {
    rgb_matrix_sethsv_noeeprom(HSV_RED);
    return 0;
}

uint32_t change_color_green_callback(uint32_t trigger_time, void *cb_arg) {
    rgb_matrix_sethsv_noeeprom(HSV_GREEN);
    return 0;
}

uint32_t change_led_effect_heatmap_callback(uint32_t trigger_time, void *cb_arg) {
    rgb_matrix_mode(RGB_MATRIX_TYPING_HEATMAP);
    return 0;
}

// Used in process_record_user() to provide ctrl+<key> on hold, <key> on tap
// Tap/hold logic works only on basic keycodes (https://docs.qmk.fm/#/keycodes_basic)
bool controlify_on_hold(uint16_t keycode, keyrecord_t *record) {
  if (!record->tap.count && record->event.pressed) {
      tap_code16(C(keycode)); // Intercept hold function to send Ctrl+<KEYCODE>. Other modifiers (Shift, Alt) are applied
    return false;
  }
  return true; // Return true for normal processing of tap keycode
}

// Used in process_record_user() to provide <key1> on tap, <key2> on hold.
// Tap/hold logic works only on basic keycodes (https://docs.qmk.fm/#/keycodes_basic)
bool tap_or_hold(uint16_t keycode_tap, uint16_t keycode_hold, keyrecord_t *record) {
    if (record->tap.count && record->event.pressed) {
        tap_code16(keycode_tap); // Intercept tap function to send [keycode_tap]
    } else if (record->event.pressed) {
        tap_code16(keycode_hold); // Intercept hold function to send [keycode_hold]
    }
    return false;
}

// Handles key presses
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  #ifdef CONSOLE_ENABLE
    uprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
  #endif
  switch (keycode) {
    // --------------------------- CTRL + <KEY> on Hold -----------------------------------------------------

    case CC_NUHS:
      return controlify_on_hold(KC_NUHS, record);
    // --------------------------- Tap / Hold -----------------------------------------------------
    // case LT(0, KC_???):
    //   return tap_or_hold(KC_HOME, KC_END, record);
    // --------------------------- Specials -----------------------------------------------------
    case (CK_STUCK):
      del_mods(MOD_MASK_CSAG); //remove all modifiers to prevent stuck modifiers
      return false;
    // --------------------------- MACROS -----------------------------------------------------
    case MACRO_0:
      if (record->event.pressed) {
        SEND_STRING(SS_LCTL(SS_LALT(SS_TAP(X_7)))); // ==> curly brace open {
      }
      return false;
    case MACRO_1:
      if (record->event.pressed) {
        SEND_STRING(SS_LCTL(SS_LALT(SS_TAP(X_0))));  // ==> curly brace close }
      }
      return false;
    case MACRO_2:
      if (record->event.pressed) {
        SEND_STRING(SS_LSFT(SS_TAP(X_8))); // ==> parenthesis open (
      }
      return false;
    case MACRO_3:
      if (record->event.pressed) {
        SEND_STRING(SS_LSFT(SS_TAP(X_9))); // ==> parenthesis  close )
      }
      return false;
    case MACRO_4:
      if (record->event.pressed) {
        SEND_STRING(SS_LCTL(SS_LALT(SS_TAP(X_8)))); // ==> square bracket open [
      }
      return false;
    case MACRO_5:
      if (record->event.pressed) {
        SEND_STRING(SS_LCTL(SS_LALT(SS_TAP(X_9)))); // ==> square bracket close ]
      }
      return false;
    case MACRO_6:
      if (record->event.pressed) {
        SEND_STRING(SS_LCTL(SS_TAP(X_RBRC))); // ctrl + plus (jump to start)
      }
      return false;
    case MACRO_7:
      if (record->event.pressed) {
        SEND_STRING(SS_LCTL(SS_LALT(SS_TAP(X_RBRC)))); // ctrl + alt + plus (jump to end)
      }
      return false;
    case MACRO_8:
      if (record->event.pressed) {
        SEND_STRING(SS_LCTL(SS_LSFT(SS_TAP(X_RBRC)))); // ctrl + shift + plus (jump to and select word (target mode))
      }
      return false;
    case MACRO_9:
      if (record->event.pressed) {
        SEND_STRING(SS_LCTL(SS_LSFT(SS_LALT(SS_TAP(X_RBRC))))); // ctrl + alt + shift + plus (jump to declaration)
      }
      return false;
  }

  return true; // Process all other keycodes normally
}


layer_state_t layer_state_set_user(layer_state_t state) {
  uint8_t new_layer = get_highest_layer(state);

  if (current_layer == RIGHT_HOLD_FUNCTION) {
    // if layer changed and the previous layer was RIGHT_HOLD_FUNCTION...
    clear_mods(); //...remove all modifiers to prevent stuck modifiers from RIGHT_HOLD_FUNCTION layer
  }
  current_layer = new_layer;

  switch (new_layer) {
    case RIGHT_HOLD_FUNCTION:
      rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
      rgb_matrix_sethsv(HSV_GREEN);
      break;
    case LEFT_HOLD_FUNCTION:
      rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
      rgb_matrix_sethsv(HSV_BLUE);
      break;
    case SPACE_FUNCTION:
      rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
      rgb_matrix_sethsv(HSV_TURQUOISE);
      break;
    case MOUSE_LAYER:
      rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
      rgb_matrix_sethsv(HSV_RED);
      break;
    case KEYCHRON_FUNCTION:
      rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
      rgb_matrix_sethsv(HSV_MAGENTA);
      break;
    case DEFAULT:
      rgb_matrix_mode(RGB_MATRIX_TYPING_HEATMAP);
      break;
    case GAMING_LAYER:
      //rgb_matrix_mode(RGB_MATRIX_TYPING_HEATMAP);
      rgb_matrix_mode(RGB_MATRIX_CUSTOM_custom_gaming);
      break;
    default:
      rgb_matrix_mode(RGB_MATRIX_CYCLE_LEFT_RIGHT);
      break;
  }

  return state;
}


// Feature Caps Words, see https://docs.qmk.fm/#/feature_caps_word
bool caps_word_press_user(uint16_t keycode) {
  switch (keycode) {
    // Keycodes that continue Caps Word, with shift applied.
    case KC_A ... KC_Z:
    case KC_MINS:
    case KC_SLSH: // -_ from german keyboard layout
    case KC_LBRC: // ü from german keyboard layout
    case KC_SCLN: // ö from german keyboard layout
    case KC_QUOT: // ä from german keyboard layout
      add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to next key.
      return true;

    // Keycodes that continue Caps Word, without shifting.
    case KC_1 ... KC_0:
    case KC_BSPC:
    case KC_DEL:
    case KC_UNDS:
      return true;

    default:
      return false;  // Deactivate Caps Word.
  }
}

// set caps-lock LED depending on layers
// https://docs.qmk.fm/#/feature_led_indicators?id=led-indicators
bool led_update_user(led_t led_state) {
  if (current_layer == GAMING_LAYER) {
    writePin(LED_CAPS_LOCK_PIN, LED_PIN_ON_STATE);
  } else {
    writePin(LED_CAPS_LOCK_PIN, !LED_PIN_ON_STATE);
  }
  return false;
}

// change colors after keyboard start
void keyboard_post_init_user(void) {
  // uint16_t ms = 200;
  // rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
  // rgb_matrix_sethsv(HSV_GREEN);

  // defer_exec(ms, change_color_red_callback, NULL);
  // defer_exec(ms * 2, change_color_green_callback, NULL);
  //defer_exec(ms * 3, change_led_effect_heatmap_callback, NULL);

  rgb_matrix_mode(RGB_MATRIX_CYCLE_LEFT_RIGHT);
  defer_exec(5000, change_led_effect_heatmap_callback, NULL);
}

// Set tapping term per key (https://docs.qmk.fm/#/tap_hold?id=tapping-term)
// A key counts as HOLD if held longer than TAPPING_TERM, as TAP/DOUBLE TAP if shorter
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  #ifdef CONSOLE_ENABLE
    uprintf("tapping term kc: 0x%04X, keycode:  %u\n", keycode, keycode);
  #endif

  return TAPPING_TERM - 20;
  // switch (keycode) {
  //   // case FN_LEFT:
  //   // case FN_RIGHT:
  //       // Make time until a press is registered take a bit longer for all but copy (KC_C)
  //       // return TAPPING_TERM + 100;
  //   // Got this numbers with the uprintf() above
  //   case 22272: // TD_LEFT
  //   case 22273: // TD_RIGHT
  //   case 22275 ... 22350: // TD_Q

  //       // Make it faster to prevent accidental use when double/triple... tapping the actual key vs the double tap function
  //       return TAPPING_TERM - 15;
  //   default:
  //       return TAPPING_TERM;
  // }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  You can use _______ in place for KC_TRANS (transparent)   *
 *  Or you can use XXXXXXX for KC_NO (NOOP)                  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[DEFAULT] = LAYOUT_tkl_iso(
    KC_ESC  , KC_F1,    KC_F2   , KC_F3   , KC_F4   , KC_F5   , KC_F6   , KC_F7,    KC_F8   , KC_F9      , KC_F10     , KC_F11     , KC_F12 ,          KC_PSCR, XXXXXXX,  CK_STUCK,
    KC_GRV  , KC_1,     KC_2    , KC_3    , KC_4    , KC_5    , KC_6    , KC_7,     KC_8    , KC_9       , KC_0       , KC_MINS    , KC_EQL , KC_BSPC, XXXXXXX, KC_HOME,  KC_PGUP ,
    KC_TAB  , TD(TD_Q), TD(TD_W), TD(TD_E), TD(TD_R), TD(TD_T), TD(TD_Y), TD(TD_U), TD(TD_I), TD(TD_O)   , TD(TD_P)   , TD(TD_LBRC), KC_RBRC,          KC_DEL , KC_END ,  KC_PGDN ,
    FN_MOUSE, TD(TD_A), TD(TD_S), TD(TD_D), TD(TD_F), TD(TD_G), TD(TD_H), TD(TD_J), TD(TD_K), TD(TD_L)   , TD(TD_SCLN), TD(TD_QUOT), CC_NUHS, KC_ENT,
    KC_LSFT , KC_NUBS,  TD(TD_Z), TD(TD_X), TD(TD_C), TD(TD_V), TD(TD_B), TD(TD_N), TD(TD_M), TD(TD_COMM), TD(TD_DOT) , KC_SLSH    ,          KC_RSFT,          KC_UP   ,
    KC_LCTL , KC_LGUI, KC_LALT,                                TD(TD_SPACE),                               KC_RALT    , KC_RGUI    , FN_KCR , KC_RCTL, KC_LEFT, KC_DOWN ,  KC_RGHT),

[SPACE_FUNCTION] = LAYOUT_tkl_iso(
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            KC_P7  ,  KC_P8  ,    KC_P9,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_P4  ,  KC_P5  ,    KC_P6,
    _______,  _______,  _______,  MACRO_6,  MACRO_7,  _______,  _______,  MACRO_8,  MACRO_9,  _______,  _______,  _______,  _______,            KC_P1  ,  KC_P2  ,    KC_P3,
    _______,  MACRO_0 , MACRO_1,  MACRO_2,  MACRO_3,  _______,  _______,  MACRO_4,  MACRO_5,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,            KC_P0  ,
    _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______,  _______,  _______),

[LEFT_HOLD_FUNCTION] = LAYOUT_tkl_iso(
    KC_ESC ,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,            XXXXXXX,  XXXXXXX,  XXXXXXX,
    XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,
    XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_PAST,  KC_PSLS,  XXXXXXX,  XXXXXXX,  KC_P4  ,  KC_P5  ,  KC_P6  ,  XXXXXXX,  XXXXXXX,  XXXXXXX,            XXXXXXX,  XXXXXXX,  XXXXXXX,
    XXXXXXX,  _______,  XXXXXXX,  KC_PPLS,  KC_PMNS,  XXXXXXX,  XXXXXXX,  KC_P1  ,  KC_P2  ,  KC_P3  ,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,
    _______,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_P7  ,  KC_P8  ,  KC_P9  ,  XXXXXXX,            XXXXXXX,            XXXXXXX,
    _______,  _______,  _______,                                KC_P0  ,                                _______,  _______,  XXXXXXX,  _______,  XXXXXXX,  XXXXXXX,  XXXXXXX),

[RIGHT_HOLD_FUNCTION] = LAYOUT_tkl_iso(
    XXXXXXX,  XXXXXXX      ,  XXXXXXX    ,  XXXXXXX,  XXXXXXX     ,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,            XXXXXXX,  XXXXXXX,  XXXXXXX,
    KC_ESC ,  XXXXXXX      ,  XXXXXXX    ,  XXXXXXX,  XXXXXXX     ,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,
    XXXXXXX,  KC_ESC       ,  KC_TAB     ,  KC_UP  ,  XXXXXXX     ,  KC_PGUP,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,            XXXXXXX,  XXXXXXX,  XXXXXXX,
    XXXXXXX,  TD(TD_HM_END),  TD(TD_LEFT),  KC_DOWN,  TD(TD_RIGHT),  KC_PGDN,  XXXXXXX,  KC_LCTL,  KC_LALT,  KC_LGUI,  _______,  XXXXXXX,  XXXXXXX,  XXXXXXX,
    _______,  XXXXXXX      ,  XXXXXXX    ,  XXXXXXX,  KC_DEL      ,  KC_BSPC,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,            _______,            XXXXXXX,
    _______,  _______      ,  _______    ,                                     KC_LSFT,                                _______,  _______,  XXXXXXX,  _______,  XXXXXXX,  XXXXXXX,  XXXXXXX),

[KEYCHRON_FUNCTION] = LAYOUT_tkl_iso(
    _______,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,            _______,  _______,  FN_GAME,
    _______,  BT_HST1,  BT_HST2,  BT_HST3,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
    RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,  _______,  _______,
    _______,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,  BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,            _______,            _______,
    _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______,  _______,  _______),

[MOUSE_LAYER] = LAYOUT_tkl_iso(
    KC_ESC ,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,            XXXXXXX,  XXXXXXX,  FN_BACK,
    XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,
    XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_MS_U,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_WH_U,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,            XXXXXXX,  XXXXXXX,  XXXXXXX,
    _______,  KC_BTN2,  KC_MS_L,  KC_MS_D,  KC_MS_R,  XXXXXXX,  KC_ACL0,  KC_WH_L,  KC_WH_D,  KC_WH_R,  KC_ACL0,  XXXXXXX,  XXXXXXX,  XXXXXXX,
    XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,            XXXXXXX,            XXXXXXX,
    XXXXXXX,  XXXXXXX,  XXXXXXX,                                KC_BTN1,                                XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX),

[GAMING_LAYER] = LAYOUT_tkl_iso(
    KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,             KC_PSCR,   XXXXXXX,  FN_BACK,
    KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,  KC_INS,    KC_HOME,  KC_PGUP,
    KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,            KC_DEL,    KC_END,   KC_PGDN,
    XXXXXXX,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,  KC_NUHS,  KC_ENT,
    KC_LSFT,  KC_NUBS,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,             KC_UP,
    KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 KC_RALT,  KC_RGUI,  XXXXXXX,  KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT)
};

// Tap Dance code

int cur_dance (tap_dance_state_t *state) {
  if (state->count == 1) {
    if (state->interrupted || !state->pressed)  return SINGLE_TAP;
    //key has not been interrupted, but they key is still held. Means you want to send a 'HOLD'.
    else return SINGLE_HOLD;
  }
  else if (state->count == 2) {
    /*
     * DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
     * action when hitting 'pp'. Suggested use case for this return value is when you want to send two
     * keystrokes of the key, and not the 'double tap' action/macro.
    */
    if (state->interrupted) return DOUBLE_SINGLE_TAP;
    else if (state->pressed) return DOUBLE_HOLD;
    else return DOUBLE_TAP;
  }
  //Assumes no one is trying to type the same letter three times (at least not quickly).
  //If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
  //an exception here to return a 'TRIPLE_SINGLE_TAP', and define that enum just like 'DOUBLE_SINGLE_TAP'
  if (state->count == 3) {
    if (state->interrupted || !state->pressed)  return TRIPLE_TAP;
    else return TRIPLE_HOLD;
  }
  else return 8; //magic number. At some point this method will expand to work for more presses
}

//initialize an instance of 'tap' for the 'x' tap dance.
static tap xtap_state = {
  .is_press_action = true,
  .state = 0
};

void tap_dance_finished_key_shift_ctrl (tap_dance_state_t *state, void *user_data) {
  uint16_t kc = ((tap_dance_single_t *)user_data)->kc;
  xtap_state.state = cur_dance(state);
  switch (xtap_state.state) {
    case SINGLE_TAP: register_code(kc); break;
    case SINGLE_HOLD: register_mods(MOD_BIT(KC_LCTL)); register_code(kc); break;
    case DOUBLE_TAP: register_mods(MOD_BIT(KC_LSFT)); register_code(kc); break;
    case DOUBLE_HOLD: register_mods(MOD_BIT(KC_LCTL)); register_code(kc); break;
    case DOUBLE_SINGLE_TAP: register_code(kc); unregister_code(kc); register_code(kc); break;
    //Last case is for fast typing. Assuming your key is `f`:
    //For example, when typing the word `buffer`, and you want to make sure that you send `ff` and not `Esc`.
    //In order to type `ff` when typing fast, the next character will have to be hit within the `TAPPING_TERM`, which by default is 200ms.
  }
}

void tap_dance_reset_key_shift_ctrl(tap_dance_state_t *state, void *user_data) {
  uint16_t kc = ((tap_dance_single_t *)user_data)->kc;
  switch (xtap_state.state) {
    case SINGLE_TAP: unregister_code(kc); break;
    case SINGLE_HOLD: unregister_mods(MOD_BIT(KC_LCTL)); unregister_code(kc); break;
    case DOUBLE_TAP: unregister_mods(MOD_BIT(KC_LSFT)); unregister_code(kc); break;
    case DOUBLE_HOLD: unregister_mods(MOD_BIT(KC_LCTL)); unregister_code(kc); break;
    case DOUBLE_SINGLE_TAP: unregister_code(kc); break;
  }
  xtap_state.state = 0;
}

void tap_dance_finished_key_shift_mo (tap_dance_state_t *state, void *user_data) {
  tap_dance_key_layer_pair_t *pair = (tap_dance_key_layer_pair_t *)user_data;
  uint16_t kc = pair->kc;
  uint8_t layer = pair->layer;

  xtap_state.state = cur_dance(state);
  switch (xtap_state.state) {
    case SINGLE_TAP: register_code(kc); break;
    case SINGLE_HOLD: layer_on(layer); break;
    case DOUBLE_TAP: register_mods(MOD_BIT(KC_LSFT)); register_code(kc); break;
    case DOUBLE_HOLD: layer_on(layer); break;
    case DOUBLE_SINGLE_TAP: register_code(kc); unregister_code(kc); register_code(kc); break;
    //Last case is for fast typing. Assuming your key is `f`:
    //For example, when typing the word `buffer`, and you want to make sure that you send `ff` and not `Esc`.
    //In order to type `ff` when typing fast, the next character will have to be hit within the `TAPPING_TERM`, which by default is 200ms.
  }
}

void tap_dance_reset_key_shift_mo(tap_dance_state_t *state, void *user_data) {
  tap_dance_key_layer_pair_t *pair = (tap_dance_key_layer_pair_t *)user_data;
  uint16_t kc = pair->kc;
  uint8_t layer = pair->layer;

  switch (xtap_state.state) {
    case SINGLE_TAP: unregister_code(kc); break;
    case SINGLE_HOLD: layer_off(layer); break;
    case DOUBLE_TAP: unregister_mods(MOD_BIT(KC_LSFT)); unregister_code(kc); break;
    case DOUBLE_HOLD: layer_off(layer); break;
    case DOUBLE_SINGLE_TAP: unregister_code(kc); break;
  }
  xtap_state.state = 0;
}

void tap_dance_finished_keys_shift_mo (tap_dance_state_t *state, void *user_data) {
  tap_dance_keys_layer_pair_t *pair = (tap_dance_keys_layer_pair_t *)user_data;
  uint16_t kc1 = pair->kc1;
  uint16_t kc2 = pair->kc2;
  uint8_t layer = pair->layer;

  xtap_state.state = cur_dance(state);
  switch (xtap_state.state) {
    case SINGLE_TAP: register_code(kc1); break;
    case SINGLE_HOLD: layer_on(layer); break;
    case DOUBLE_TAP: register_code(kc2); break;
    case DOUBLE_HOLD: layer_on(layer); break;
    case DOUBLE_SINGLE_TAP: register_code(kc1); unregister_code(kc1); register_code(kc1); break;
    //Last case is for fast typing. Assuming your key is `f`:
    //For example, when typing the word `buffer`, and you want to make sure that you send `ff` and not `Esc`.
    //In order to type `ff` when typing fast, the next character will have to be hit within the `TAPPING_TERM`, which by default is 200ms.
  }
}

void tap_dance_reset_keys_shift_mo(tap_dance_state_t *state, void *user_data) {
  tap_dance_keys_layer_pair_t *pair = (tap_dance_keys_layer_pair_t *)user_data;
  uint16_t kc1 = pair->kc1;
  uint16_t kc2 = pair->kc2;
  uint8_t layer = pair->layer;

  switch (xtap_state.state) {
    case SINGLE_TAP: unregister_code(kc1); break;
    case SINGLE_HOLD: layer_off(layer); break;
    case DOUBLE_TAP: unregister_code(kc2); break;
    case DOUBLE_HOLD: layer_off(layer); break;
    case DOUBLE_SINGLE_TAP: unregister_code(kc1); break;
  }
  xtap_state.state = 0;
}