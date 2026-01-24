
RGB_MATRIX_EFFECT(waves)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#define WAVES_DECAY_UPDATE_MS 50
#define WAVES_UPDATE_MS 50

#define SCALE_TIME(ms) (scale16by8(ms, qadd8(rgb_matrix_config.speed, 1)))

// static uint16_t waves_decay_timer;
// static uint16_t waves_timer;

static uint32_t timer;

static bool waves(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    if (params->init) {
        random_state = (pcg32_random_t){123, 0};
        rgb_matrix_set_color_all(0, 0, 0);
        memset(g_rgb_frame_buffer, 0, sizeof g_rgb_frame_buffer);
        timer = sync_timer_read32();
    }

    uint32_t deltaTime = sync_timer_elapsed32(timer);
    timer = sync_timer_read32();

    // Decay
    // if (params->iter == 0 && timer_elapsed(waves_decay_timer) >= WAVES_DECAY_UPDATE_MS) {
        // waves_decay_timer = timer_read();
        for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; ++i) {
        // for (uint8_t i = led_min; i < led_max; i++) {
            uint8_t *buffer_p = &(g_rgb_frame_buffer[i/MATRIX_COLS][i%MATRIX_COLS]);
            if (*buffer_p > 0) {
                // *buffer_p = qsub8(*buffer_p, SCALE_TIME(16*deltaTime));
                *buffer_p = qsub8(*buffer_p, SCALE_TIME(4*deltaTime));
                // *buffer_p /= 2;
            }
        }
    // }
    
    // Reactive
    // for (uint8_t c = 0; c < MATRIX_COLS; ++c) {
    //     for (uint8_t r = 0; r < MATRIX_ROWS; ++r) {
    //         led_point_t mp = g_led_config.points[g_led_config.matrix_co[c][r]];
    //         for (uint8_t i = 0; i < g_last_hit_tracker.count; ++i) {
    //             int16_t dx = (int16_t)mp.x - g_last_hit_tracker.x[i];
    //             int16_t dy = (int16_t)mp.y - g_last_hit_tracker.y[i];
    //             sqrt()
    //             uint8_t radius = g_last_hit_tracker.tick[i] / 250;
    //             uint8_t index = g_last_hit_tracker.index[i];
    //             g_rgb_frame_buffer[index/MATRIX_COLS][index%MATRIX_COLS] = 255 / ;
    //         }
    //     }
    // }

    // if (params->iter == 0 && timer_elapsed(waves_timer) >= WAVES_UPDATE_MS) {
    //     waves_timer = timer_read();
        // for (uint8_t i = led_min; i < led_max; i++) {
        for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
            // RGB_MATRIX_TEST_LED_FLAGS();
            for (uint8_t j = 0; j < g_last_hit_tracker.count; j++) {
                int16_t  dx   = ((int16_t)g_led_config.point[i].x) - ((int16_t)g_last_hit_tracker.x[j]);
                int16_t  dy   = ((int16_t)g_led_config.point[i].y) - ((int16_t)g_last_hit_tracker.y[j]);
                uint8_t  dist = sqrt16(dx * dx + dy * dy);
                uint16_t tick = SCALE_TIME(g_last_hit_tracker.tick[j]);
                // uint16_t tick = scale16by8(g_last_hit_tracker.tick[j], qadd8(rgb_matrix_config.speed, 1));
                
                // int32_t wave_radius = tick - dist;
                int32_t wave_radius = dist - tick + (224/15);
                if (ABS(wave_radius) < (224/15)) {
                    uint8_t *buffer_p = &(g_rgb_frame_buffer[i/MATRIX_COLS][i%MATRIX_COLS]);
                    // *buffer_p = MAX(*buffer_p, 255 - MIN(255, scale16by8(wave_radius, 127)));
                    *buffer_p = qadd8(*buffer_p, scale16by8(qsub8(224, dist), SCALE_TIME(8*deltaTime)));
                    // *buffer_p = qadd8(*buffer_p, scale16by8(qsub8(224, dist), SCALE_TIME(32*deltaTime)));
                }
            }
        }
    // }
 
    // Rendering
    // for (uint8_t i = led_min; index < led_max; ++i) {
    for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
        uint8_t buffer_v = g_rgb_frame_buffer[i/MATRIX_COLS][i%MATRIX_COLS];
        rgb_t rgb = hsv_to_rgb((hsv_t){(rgblight_get_hue() + buffer_v/4) % 255, rgblight_get_sat(), rgblight_get_val()});//*3/4+buffer_v/4});
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return false;//rgb_matrix_check_finished_leds(led_max);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
