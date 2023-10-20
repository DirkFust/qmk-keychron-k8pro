/* Copyright 2021 @ Keychron (https://www.keychron.com)
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

#include QMK_KEYBOARD_H
static uint8_t current_layer = 0;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  You can use _______ in place for KC_TRANS (transparent)   *
 *  Or you can use XXXXXXX for KC_NO (NOOP)                  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// DEFINITIONS: Shorten long "key"-names
// Layer switches
#define FN_BACK TO(DEFAULT)
#define FN_GAME TO(GAMING_LAYER)
#define FN_KCR MO(KEYCHRON_FUNCTION)
#define FN_LEFT LT(LEFT_HOLD_FUNCTION, KC_A)
#define FN_RIGHT LT(RIGHT_HOLD_FUNCTION, KC_SCLN)
#define FN_SPACE LT(SPACE_FUNCTION, KC_SPC)
#define FN_MOUSE TG(MOUSE)

// CC_<KEYNAME>: [C]trl-[C]ode, for keys that have ctrl activated on hold
// LT() is used to switch layers on hold, but used on layer 0 it does not switch a layer, but provides tap/hold logic
// see: https://docs.qmk.fm/#/mod_tap?id=changing-hold-function
#define CC_C LT(0, KC_C)
#define CC_D LT(0, KC_D)
#define CC_F LT(0, KC_F)
#define CC_H LT(0, KC_H)
#define CC_V LT(0, KC_V)
#define CC_X LT(0, KC_X)
#define CC_Y LT(0, KC_Y)
#define CC_Z LT(0, KC_Z)
#define CC_NUHS LT(0, KC_NUHS)

// TH_<KEYNAME>: [T]ap-[H]old, for keys that have a tap and a hold action
// Tap/hold logic works only on basic keycodes (https://docs.qmk.fm/#/keycodes_basic)
#define TH_HM_END LT(0, KC_F24)

// clang-format off
enum layers{
  DEFAULT,
  SPACE_FUNCTION,
  LEFT_HOLD_FUNCTION,
  RIGHT_HOLD_FUNCTION,
  KEYCHRON_FUNCTION,
  MOUSE,
  GAMING_LAYER
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
  switch (keycode) {
    // --------------------------- CTRL + <KEY> on Hold -----------------------------------------------------
    case CC_C:
      return controlify_on_hold(KC_C, record);
    case CC_D:
      return controlify_on_hold(KC_D, record);
    case CC_F:
      return controlify_on_hold(KC_F, record);
    case CC_H:
      return controlify_on_hold(KC_H, record);
    case CC_V:
      return controlify_on_hold(KC_V, record);
    case CC_X:
      return controlify_on_hold(KC_X, record);
    case CC_Y:
      return controlify_on_hold(KC_Y, record);
    case CC_Z:
      return controlify_on_hold(KC_Z, record);
    case CC_NUHS:
      return controlify_on_hold(KC_NUHS, record);
    // --------------------------- Tap / Hold -----------------------------------------------------
    case LT(0, KC_F24):
      return tap_or_hold(KC_HOME, KC_END, record);
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
    case MOUSE:
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
  defer_exec(1000, change_led_effect_heatmap_callback, NULL);
}

// Set tapping term per key (https://docs.qmk.fm/#/tap_hold?id=tapping-term)
// A key counts as HOLD if held longer than TAPPING_TERM, as TAP if shorter
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case FN_LEFT:
        return TAPPING_TERM + 100;
    default:
        return TAPPING_TERM;
  }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[DEFAULT] = LAYOUT_tkl_iso(
    KC_ESC,   KC_F1,   KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,             KC_PSCR,   XXXXXXX,  CK_STUCK,
    KC_GRV,   KC_1,    KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,  XXXXXXX,   KC_HOME,  KC_PGUP,
    KC_TAB,   KC_Q,    KC_W,     KC_E,     KC_R,     KC_T,     CC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,            KC_DEL,    KC_END,   KC_PGDN,
    FN_MOUSE, FN_LEFT, KC_S,     CC_D,     CC_F,     KC_G,     CC_H,     KC_J,     KC_K,     KC_L,     FN_RIGHT, KC_QUOT,  CC_NUHS,  KC_ENT,
    KC_LSFT,  KC_NUBS, CC_Z,     CC_X,     CC_C,     CC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,             KC_UP,
    KC_LCTL,  KC_LGUI, KC_LALT,                                FN_SPACE,                               KC_RALT,  KC_RGUI,  FN_KCR ,   KC_RCTL,  KC_LEFT,   KC_DOWN,  KC_RGHT),

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
    KC_ESC ,  XXXXXXX  ,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,
    XXXXXXX,  XXXXXXX  ,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,            XXXXXXX,  XXXXXXX,  XXXXXXX,
    XXXXXXX,  KC_ESC   ,  KC_TAB ,  KC_UP  ,  XXXXXXX,  KC_PGUP,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,            XXXXXXX,  XXXXXXX,  XXXXXXX,
    XXXXXXX,  TH_HM_END,  KC_LEFT,  KC_DOWN,  KC_RIGHT, KC_PGDN,  XXXXXXX,  KC_LCTL,  KC_LALT,  KC_LGUI,  _______,  XXXXXXX,  XXXXXXX,  XXXXXXX,
    _______,  XXXXXXX  ,  XXXXXXX,  XXXXXXX,  KC_DEL ,  KC_BSPC,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,            _______,            XXXXXXX,
    _______,  _______  ,  _______,                                KC_LSFT,                                _______,  _______,  XXXXXXX,  _______,  XXXXXXX,  XXXXXXX,  XXXXXXX),

[KEYCHRON_FUNCTION] = LAYOUT_tkl_iso(
    _______,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,            _______,  _______,  FN_GAME,
    _______,  BT_HST1,  BT_HST2,  BT_HST3,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
    RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,  _______,  _______,
    _______,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,  BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,            _______,            _______,
    _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______,  _______,  _______),

[MOUSE] = LAYOUT_tkl_iso(
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
