// Documentation: https://docs.qmk.fm/#/feature_rgb_matrix?id=custom-rgb-matrix-effects

// For inspiration and examples, check out the built-in effects under quantum/rgb_matrix/animations/.

// Step 1.
// Declare custom effects using the RGB_MATRIX_EFFECT macro
// (note the lack of semicolon after the macro!)
RGB_MATRIX_EFFECT(custom_gaming)

// Step 2.
// Define effects inside the `RGB_MATRIX_CUSTOM_EFFECT_IMPLS` ifdef block
#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

// e.g: A simple effect, self-contained within a single method
static bool custom_gaming(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    HSV red     = {0, 255, 255};
    HSV green   = {85, 255, 255};
    HSV blue    = {170, 255, 255};
    HSV magenta = {213, 255, 255};
    HSV white   = {0, 0, 64};
    // HSV off     = {0, 0, 0};

    // RGB rgb = rgb_matrix_hsv_to_rgb(rgb_matrix_config.hsv);

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        HSV color;

        switch (i) {
            case 15:
                color = red;
                break;
            case 36:
            case 52 ... 54:
                color = green;
                break;
            case 51:
            case 37:
                color = blue;
                break;
            case 35:
                color = magenta;
                break;
            default:
                color = white;
                break;
        }

        RGB rgb = rgb_matrix_hsv_to_rgb(color);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS