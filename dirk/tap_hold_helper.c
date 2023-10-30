
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
