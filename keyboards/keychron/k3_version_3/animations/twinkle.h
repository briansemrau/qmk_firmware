
RGB_MATRIX_EFFECT(twinkle)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#define TWINKLE_DECAY_UPDATE_MS 40
#define TWINKLE_RANDOM_UPDATE_MS 250

typedef struct { uint64_t state;  uint64_t inc; } pcg32_random_t;

static uint32_t pcg32_random_r(pcg32_random_t* rng)
{
    uint64_t oldstate = rng->state;
    // Advance internal state
    rng->state = oldstate * 6364136223846793005ULL + (rng->inc|1);
    // Calculate output function (XSH RR), uses old state for max ILP
    uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    uint32_t rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

static pcg32_random_t random_state;

static uint16_t twinkle_decay_timer;
static uint16_t twinkle_compute_timer;

static uint32_t timer;

static bool twinkle(effect_params_t* params) {
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
    if (params->iter == 0 && timer_elapsed(twinkle_decay_timer) >= TWINKLE_DECAY_UPDATE_MS) {
        twinkle_decay_timer = timer_read();
        for (uint8_t index = 0; index < RGB_MATRIX_LED_COUNT; ++index) {
            if (g_rgb_frame_buffer[index/MATRIX_COLS][index%MATRIX_COLS] > 0) {
                g_rgb_frame_buffer[index/MATRIX_COLS][index%MATRIX_COLS] = qsub8(g_rgb_frame_buffer[index/MATRIX_COLS][index%MATRIX_COLS], 4);
            }
        }
    }

    // Random
    if (params->iter == 0 && timer_elapsed(twinkle_compute_timer) >= TWINKLE_RANDOM_UPDATE_MS) {
        twinkle_compute_timer = timer_read();
        uint8_t index = pcg32_random_r(&random_state) % RGB_MATRIX_LED_COUNT;
        g_rgb_frame_buffer[index/MATRIX_COLS][index%MATRIX_COLS] = 64 + (pcg32_random_r(&random_state) % 255) / 2;
    }
    
    // Reactive
    if (!params->init) {
        for (uint8_t i = 0; i < g_last_hit_tracker.count; ++i) {
            if (g_last_hit_tracker.tick[i] >= deltaTime) // only when first pressed
                continue;
            uint8_t index = g_last_hit_tracker.index[i];
            g_rgb_frame_buffer[index/MATRIX_COLS][index%MATRIX_COLS] = 255;
        }
    }

    // Rendering
    // for (uint8_t i = led_min; i < led_max; ++i) {
    for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; ++i) {
        uint8_t buffer_v = g_rgb_frame_buffer[i/MATRIX_COLS][i%MATRIX_COLS];
        rgb_t rgb = hsv_to_rgb((hsv_t){(rgblight_get_hue() + buffer_v/4) % 255, rgblight_get_sat()*3/4+buffer_v/4, rgblight_get_val()});
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return false;//rgb_matrix_check_finished_leds(led_max);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
