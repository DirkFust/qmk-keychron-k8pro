typedef struct {
    bool is_press_action;
    int  state;
} tap;

typedef struct {
    uint16_t kc;
} tap_dance_single_key_t;

typedef struct {
    uint16_t kc;
    uint8_t  layer;
} tap_dance_key_layer_t;

typedef struct {
    uint16_t kc1;
    uint16_t kc2;
    uint8_t  layer;
} tap_dance_two_keys_layer_t;

enum td_type {
    SINGLE_TAP        = 1,
    SINGLE_HOLD       = 2,
    DOUBLE_TAP        = 3,
    DOUBLE_HOLD       = 4,
    DOUBLE_SINGLE_TAP = 5, // send two single taps
    TRIPLE_TAP        = 6,
    TRIPLE_HOLD       = 7,
    TRIPLE_SINGLE_TAP = 8, // send three single taps
    OTHER             = 255
};

int  cur_dance(tap_dance_state_t *state);
void tap_dance_finished_key_shift_ctrl(tap_dance_state_t *state, void *user_data);
void tap_dance_reset_key_shift_ctrl(tap_dance_state_t *state, void *user_data);
void tap_dance_finished_key_shift_mo(tap_dance_state_t *state, void *user_data);
void tap_dance_reset_key_shift_mo(tap_dance_state_t *state, void *user_data);
void tap_dance_finished_keys_shift_mo(tap_dance_state_t *state, void *user_data);
void tap_dance_reset_keys_shift_mo(tap_dance_state_t *state, void *user_data);
void tap_dance_finished_key_shift_altgr(tap_dance_state_t *state, void *user_data);
void tap_dance_reset_key_shift_altgr(tap_dance_state_t *state, void *user_data);
void tap_dance_finished_dual(tap_dance_state_t *state, void *user_data);
void tap_dance_reset_dual(tap_dance_state_t *state, void *user_data);

// <kc> on tap, shift+<kc> on double tap
#define ACTION_TAP_DANCE_SHIFT(kc) \
    { .fn = {NULL, tap_dance_finished_dual, tap_dance_reset_dual}, .user_data = (void *)&((tap_dance_single_key_t){kc}), }
// <kc> on tap, shift+<kc> on double tap, ctrl+<kc> on hold
#define ACTION_TAP_DANCE_SHIFT_CTRL(kc) \
    { .fn = {NULL, tap_dance_finished_key_shift_ctrl, tap_dance_reset_key_shift_ctrl}, .user_data = (void *)&((tap_dance_single_key_t){kc}), }
// <kc> on tap, shift+<kc> on double tap, move to <layer> on hold, as long as key is held
#define ACTION_TAP_DANCE_SHIFT_MO(kc, layer) \
    { .fn = {NULL, tap_dance_finished_key_shift_mo, tap_dance_reset_key_shift_mo}, .user_data = (void *)&((tap_dance_key_layer_t){kc, layer}), }
// <kc1> on tap, <kc2> on double tap, move to <layer> on hold, as long as key is held
#define ACTION_TAP_DANCE_KC1_KC2_MO(kc1, kc2, layer) \
    { .fn = {NULL, tap_dance_finished_keys_shift_mo, tap_dance_reset_keys_shift_mo}, .user_data = (void *)&((tap_dance_two_keys_layer_t){kc1, kc2, layer}), }
// <kc> on tap, shift+<kc> on double tap, altgr+<kc> on triple tap
#define ACTION_TAP_DANCE_SHIFT_ALTGR(kc) \
    { .fn = {NULL, tap_dance_finished_key_shift_altgr, tap_dance_reset_key_shift_altgr}, .user_data = (void *)&((tap_dance_single_key_t){kc}), }

int cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return SINGLE_TAP;
        // key has not been interrupted, but they key is still held. Means you want to send a 'HOLD'.
        else
            return SINGLE_HOLD;
    } else if (state->count == 2) {
        /*
         * DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
         * action when hitting 'pp'. Suggested use case for this return value is when you want to send two
         * keystrokes of the key, and not the 'double tap' action/macro.
         */
        if (state->interrupted)
            return DOUBLE_SINGLE_TAP;
        else if (state->pressed)
            return DOUBLE_HOLD;
        else
            return DOUBLE_TAP;
    }
    // Assumes no one is trying to type the same letter three times (at least not quickly).
    // If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
    // an exception here to return a 'TRIPLE_SINGLE_TAP', and define that enum just like 'DOUBLE_SINGLE_TAP'
    if (state->count == 3) {
        if (state->interrupted)
            return TRIPLE_SINGLE_TAP;
        else if (!state->pressed)
            return TRIPLE_TAP;
        else
            return TRIPLE_HOLD;
    } else
        return OTHER;
}

// initialize an instance of 'tap' for the 'x' tap dance.
static tap xtap_state = {.is_press_action = true, .state = 0};

void tap_dance_finished_key_shift_altgr(tap_dance_state_t *state, void *user_data) {
    uint16_t kc      = ((tap_dance_single_key_t *)user_data)->kc;
    xtap_state.state = cur_dance(state);
    switch (xtap_state.state) {
        case SINGLE_TAP:
            register_code(kc);
            break;
        case DOUBLE_TAP:
            register_mods(MOD_BIT(KC_LSFT));
            register_code(kc);
            break;
        case DOUBLE_SINGLE_TAP:
            register_code(kc);
            unregister_code(kc);
            register_code(kc);
            break;
        case TRIPLE_TAP:
            register_mods(MOD_BIT(KC_RALT));
            register_code(kc);
            break;
        case TRIPLE_SINGLE_TAP:
            register_code(kc);
            unregister_code(kc);
            register_code(kc);
            unregister_code(kc);
            register_code(kc);
            break;
    }
}

void tap_dance_reset_key_shift_altgr(tap_dance_state_t *state, void *user_data) {
    uint16_t kc = ((tap_dance_single_key_t *)user_data)->kc;
    switch (xtap_state.state) {
        case SINGLE_TAP:
        case DOUBLE_SINGLE_TAP:
        case TRIPLE_SINGLE_TAP:
            unregister_code(kc);
            break;
        case DOUBLE_TAP:
            unregister_mods(MOD_BIT(KC_LSFT));
            unregister_code(kc);
            break;
        case TRIPLE_TAP:
            unregister_mods(MOD_BIT(KC_RALT));
            unregister_code(kc);
            break;
    }
    xtap_state.state = 0;
}

void tap_dance_finished_key_shift_ctrl(tap_dance_state_t *state, void *user_data) {
    uint16_t kc      = ((tap_dance_single_key_t *)user_data)->kc;
    xtap_state.state = cur_dance(state);
    switch (xtap_state.state) {
        case SINGLE_TAP:
            register_code(kc);
            break;
        case SINGLE_HOLD:
            register_mods(MOD_BIT(KC_LCTL));
            register_code(kc);
            break;
        case DOUBLE_TAP:
            register_mods(MOD_BIT(KC_LSFT));
            register_code(kc);
            break;
        case DOUBLE_HOLD:
            register_mods(MOD_BIT(KC_LCTL));
            register_code(kc);
            break;
        case DOUBLE_SINGLE_TAP:
            register_code(kc);
            unregister_code(kc);
            register_code(kc);
            break;
        case TRIPLE_SINGLE_TAP:
            register_code(kc);
            unregister_code(kc);
            register_code(kc);
            unregister_code(kc);
            register_code(kc);
            break;
            // Last case is for fast typing. Assuming your key is `f`:
            // For example, when typing the word `buffer`, and you want to make sure that you send `ff` and not `Esc`.
            // In order to type `ff` when typing fast, the next character will have to be hit within the `TAPPING_TERM`, which by default is 200ms.
    }
}

void tap_dance_reset_key_shift_ctrl(tap_dance_state_t *state, void *user_data) {
    uint16_t kc = ((tap_dance_single_key_t *)user_data)->kc;
    switch (xtap_state.state) {
        case SINGLE_TAP:
        case DOUBLE_SINGLE_TAP:
        case TRIPLE_SINGLE_TAP:
            unregister_code(kc);
            break;
        case SINGLE_HOLD:
            unregister_mods(MOD_BIT(KC_LCTL));
            unregister_code(kc);
            break;
        case DOUBLE_TAP:
            unregister_mods(MOD_BIT(KC_LSFT));
            unregister_code(kc);
            break;
        case DOUBLE_HOLD:
            unregister_mods(MOD_BIT(KC_LCTL));
            unregister_code(kc);
            break;
    }
    xtap_state.state = 0;
}

void tap_dance_finished_key_shift_mo(tap_dance_state_t *state, void *user_data) {
    tap_dance_key_layer_t *pair  = (tap_dance_key_layer_t *)user_data;
    uint16_t               kc    = pair->kc;
    uint8_t                layer = pair->layer;

    xtap_state.state = cur_dance(state);
    switch (xtap_state.state) {
        case SINGLE_TAP:
            register_code(kc);
            break;
        case SINGLE_HOLD:
            layer_on(layer);
            break;
        case DOUBLE_TAP:
            register_mods(MOD_BIT(KC_LSFT));
            register_code(kc);
            break;
        case DOUBLE_HOLD:
            layer_on(layer);
            break;
        case DOUBLE_SINGLE_TAP:
            register_code(kc);
            unregister_code(kc);
            register_code(kc);
            break;
        case TRIPLE_SINGLE_TAP:
            register_code(kc);
            unregister_code(kc);
            register_code(kc);
            unregister_code(kc);
            register_code(kc);
            break;
            // Last case is for fast typing. Assuming your key is `f`:
            // For example, when typing the word `buffer`, and you want to make sure that you send `ff` and not `Esc`.
            // In order to type `ff` when typing fast, the next character will have to be hit within the `TAPPING_TERM`, which by default is 200ms.
    }
}

void tap_dance_reset_key_shift_mo(tap_dance_state_t *state, void *user_data) {
    tap_dance_key_layer_t *pair  = (tap_dance_key_layer_t *)user_data;
    uint16_t               kc    = pair->kc;
    uint8_t                layer = pair->layer;

    switch (xtap_state.state) {
        case SINGLE_TAP:
        case DOUBLE_SINGLE_TAP:
        case TRIPLE_SINGLE_TAP:
            unregister_code(kc);
            break;
        case SINGLE_HOLD:
            layer_off(layer);
            break;
        case DOUBLE_TAP:
            unregister_mods(MOD_BIT(KC_LSFT));
            unregister_code(kc);
            break;
        case DOUBLE_HOLD:
            layer_off(layer);
            break;
    }
    xtap_state.state = 0;
}

void tap_dance_finished_keys_shift_mo(tap_dance_state_t *state, void *user_data) {
    tap_dance_two_keys_layer_t *pair  = (tap_dance_two_keys_layer_t *)user_data;
    uint16_t                    kc1   = pair->kc1;
    uint16_t                    kc2   = pair->kc2;
    uint8_t                     layer = pair->layer;

    xtap_state.state = cur_dance(state);
    switch (xtap_state.state) {
        case SINGLE_TAP:
            register_code(kc1);
            break;
        case SINGLE_HOLD:
            layer_on(layer);
            break;
        case DOUBLE_TAP:
            register_code(kc2);
            break;
        case DOUBLE_HOLD:
            layer_on(layer);
            break;
        case DOUBLE_SINGLE_TAP:
            register_code(kc1);
            unregister_code(kc1);
            register_code(kc1);
            break;
        case TRIPLE_SINGLE_TAP:
            register_code(kc1);
            unregister_code(kc1);
            register_code(kc1);
            unregister_code(kc1);
            register_code(kc1);
            break;
            // Last case is for fast typing. Assuming your key is `f`:
            // For example, when typing the word `buffer`, and you want to make sure that you send `ff` and not `Esc`.
            // In order to type `ff` when typing fast, the next character will have to be hit within the `TAPPING_TERM`, which by default is 200ms.
    }
}

void tap_dance_reset_keys_shift_mo(tap_dance_state_t *state, void *user_data) {
    tap_dance_two_keys_layer_t *pair  = (tap_dance_two_keys_layer_t *)user_data;
    uint16_t                    kc1   = pair->kc1;
    uint16_t                    kc2   = pair->kc2;
    uint8_t                     layer = pair->layer;

    switch (xtap_state.state) {
        case SINGLE_TAP:
        case DOUBLE_SINGLE_TAP:
        case TRIPLE_SINGLE_TAP:
            unregister_code(kc1);
            break;
        case SINGLE_HOLD:
            layer_off(layer);
            break;
        case DOUBLE_TAP:
            unregister_code(kc2);
            break;
        case DOUBLE_HOLD:
            layer_off(layer);
            break;
    }
    xtap_state.state = 0;
}

void tap_dance_reset_dual(tap_dance_state_t *state, void *user_data) {
    uint16_t kc = ((tap_dance_single_key_t *)user_data)->kc;

    switch (xtap_state.state) {
        case SINGLE_TAP:
        case DOUBLE_SINGLE_TAP:
        case TRIPLE_SINGLE_TAP:
        case SINGLE_HOLD:
            unregister_code(kc);
            break;
        case DOUBLE_TAP:
        case DOUBLE_HOLD:
            unregister_mods(MOD_BIT(KC_LSFT));
            unregister_code(kc);
            break;
    }
    xtap_state.state = 0;
}

void tap_dance_finished_dual(tap_dance_state_t *state, void *user_data) {
    uint16_t kc = ((tap_dance_single_key_t *)user_data)->kc;

    xtap_state.state = cur_dance(state);
    switch (xtap_state.state) {
        case SINGLE_TAP:
        case SINGLE_HOLD:
            register_code(kc);
            break;
        case DOUBLE_TAP:
        case DOUBLE_HOLD:
            register_mods(MOD_BIT(KC_LSFT));
            register_code(kc);
            break;
        case DOUBLE_SINGLE_TAP:
            register_code(kc);
            unregister_code(kc);
            register_code(kc);
            break;
        case TRIPLE_SINGLE_TAP:
            register_code(kc);
            unregister_code(kc);
            register_code(kc);
            unregister_code(kc);
            register_code(kc);
            break;
            // Last case is for fast typing. Assuming your key is `f`:
            // For example, when typing the word `buffer`, and you want to make sure that you send `ff` and not `Esc`.
            // In order to type `ff` when typing fast, the next character will have to be hit within the `TAPPING_TERM`, which by default is 200ms.
    }
}