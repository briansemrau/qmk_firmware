
RGB_MATRIX_EFFECT(simple_fluid)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#define SIMPLE_FLUID_UPDATE_MS scale16by8(50, qadd8(rgb_matrix_config.speed, 1))

typedef struct {
    uint8_t x_start, x_end;
    uint8_t y_start, y_end;
} led_area_map_t;

#define FLUID_W_SCALE 2
#define FLUID_W (MATRIX_ROWS*FLUID_W_SCALE)
typedef struct {
    float h[FLUID_W];
    float v[FLUID_W];
    uint16_t compute_timer;
    uint8_t ticks;
} simple_fluid_state_t;

// #define READ_H(x) ((x < 0 || x >= FLUID_W) ? 0.f : simple_fluid_state.h[x])
#ifndef CLAMP
#define CLAMP(x, a, b) (((x) < (a)) ? (a) : (((x) > (b)) ? (b) : (x)))
#endif
#ifndef ABS
#define ABS(x) ((x) < 0 ? -(x) : (x))
#endif

#define READ_H(x) ((x < 0) ? simple_fluid_state.h[0] : ((x >= FLUID_W) ? simple_fluid_state.h[FLUID_W-1] : simple_fluid_state.h[x]))

static simple_fluid_state_t simple_fluid_state;

static bool simple_fluid(effect_params_t* params) {
    if (params->init) {
        for (uint8_t i = 0; i < FLUID_W; ++i) {
            simple_fluid_state.h[i] = 0.f;
        }
        simple_fluid_state.ticks = 0;
        // simple_fluid_state.h[5] = 1.f;
    }

    if (params->iter == 0 && timer_elapsed(simple_fluid_state.compute_timer) >= SIMPLE_FLUID_UPDATE_MS) {
        simple_fluid_state.compute_timer = timer_read();

        // velocity update
        #define UPDATE_V(a, i, b) simple_fluid_state.v[i] += (simple_fluid_state.h[a] + simple_fluid_state.h[b]) / 2 - simple_fluid_state.h[i];
        // boundary conditions
        UPDATE_V(0, 0, 1)
        UPDATE_V(FLUID_W-2, FLUID_W-1, FLUID_W-1)
        for (uint8_t x = 1; x < FLUID_W-1; ++x) {
            UPDATE_V(x-1, x, x+1)
        }
        // position update, damping, normalization
        float total = 0.f;
        for (uint8_t x = 0; x < FLUID_W; ++x) {
            simple_fluid_state.v[x] *= 0.97;
            simple_fluid_state.h[x] += simple_fluid_state.v[x]; // semi-implicit Euler
            total += simple_fluid_state.h[x];
        }
        float avg = total /= FLUID_W;
        for (uint8_t x = 0; x < FLUID_W; ++x) {
            simple_fluid_state.h[x] -= avg;
        }
        #undef UPD_V
        if (simple_fluid_state.ticks < 255) {
            ++simple_fluid_state.ticks;
        }
    }

    // Reactive
    if (simple_fluid_state.ticks > 1) {
        for (uint8_t i = 0; i < g_last_hit_tracker.count; ++i) {
            if (g_last_hit_tracker.tick[i] >= SIMPLE_FLUID_UPDATE_MS) // only when first pressed
                continue;
            uint8_t x = g_last_hit_tracker.x[i] * FLUID_W / 224;
            simple_fluid_state.h[CLAMP(x, 0, FLUID_W-1)] += 0.125;
            simple_fluid_state.h[CLAMP(x-1, 0, FLUID_W-1)] -= 0.0625;
            simple_fluid_state.h[CLAMP(x+1, 0, FLUID_W-1)] -= 0.0625;
        }
    }

    // Rendering
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    for (uint8_t index = led_min; index < led_max; ++index) {
        // float x = g_led_config.point[index].x / 224.f * FLUID_W;
        float y = 1.f - g_led_config.point[index].y / 64.f - (1.f / (MATRIX_COLS*2));

        // float frac = x - (int)x;
        // float h = READ_H((int)x) * (1.f - frac) + READ_H((int)x+1) * frac;
        float h = 0.f;
        uint8_t w = MAX(1, (int)(((float)g_led_sizes[index][0])*FLUID_W/(MATRIX_ROWS*4)));
        uint8_t x_offs = qadd8(g_led_config.point[index].x*FLUID_W/224, w/2);
        uint8_t x0 = MAX(x_offs, 0);
        uint8_t x1 = MAX(MIN(x_offs+w, FLUID_W), x0+1);
        for (uint8_t x = x0; x < x1; ++x) {
            h += simple_fluid_state.h[x];
        }
        h /= (x1 - x0);

        h += 0.5f;
        float a = (h-y) * MATRIX_COLS;
        a = (a > 1.f ? 1.f : (a < 0.f ? 0.f : a));

        rgb_t rgb = hsv_to_rgb((hsv_t){(rgblight_get_hue()) % 255, rgblight_get_sat(), (int)(rgblight_get_val() * a)});
        rgb_matrix_set_color(index, rgb.r, rgb.g, rgb.b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
