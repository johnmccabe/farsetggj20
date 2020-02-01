#include <cstdlib>
#include <algorithm>
#include <array>
#include <math.h>
#include <stdint.h>

#include "graphics/color.hpp"

#include "farsetggj20.hpp"
#include "greta.hpp"

using namespace blit;

#define SCREEN_W 160
#define SCREEN_H 120

// Keep track of game state
enum enum_state {
    menu = 0,
    play = 1,
    dead = 2
};
enum_state game_state = enum_state::menu;

blit::timer greta_move_timer;
int big_greta_y = 120;

struct Title {
    float_t t = 0.0;

    void render(uint32_t time_ms)
    {
        fb.pen(rgba(0, 0, 0));
        fb.clear();

        t = t + 0.3;
        draw_title();
        play_soundtrack();
    }

    void update(uint32_t time_ms)
    {

    }

    void draw_background()
    {
        for (int b = 0; b < SCREEN_W; b++)
        {
            for (int v = 0; v < SCREEN_H; v++)
            {
                fb.pen(blit::hsv_to_rgba(float(b) / (float)(SCREEN_W), 1.0f, float(v) / (float)(SCREEN_H)));
                fb.pixel(point(b, v));
            }
        }
    }

    void draw_greta(int8_t x, int8_t y) {
        point position(x, y);
        fb.sprite(rect(0,0,11,15), position);
    }

    void draw_logo(int8_t x, int8_t y) {
        point position(x, y);
        fb.sprite(rect(11,0,9,4), position);
    }

    void draw_animated_boxes() {
        for (uint8_t i = 7; i < 15; i++)
        {
            int32_t x = (SCREEN_W / 2) + cos(t * (float)i / 150.0) * 60.0;
            int32_t y = 108;
            int32_t width = abs((cos(t * (float)i / 150.0) * 60.0) - (cos(t * (float)i / 150.0 + 0.02) * 60.0)) * 10;
            int32_t height = 11;

            switch (i)
            {
            case 8:
                fb.pen(rgba(0xff, 0x00, 0x00));
                break;
            case 9:
                fb.pen(rgba(0xff, 0xa5, 0x00));
                break;
            case 10:
                fb.pen(rgba(0xff, 0xff, 0x00));
                break;
            case 11:
                fb.pen(rgba(0x00, 0xff, 0x00));
                break;
            case 12:
                fb.pen(rgba(0x00, 0x00, 0xff));
                break;
            case 13:
                fb.pen(rgba(0x80, 0x00, 0x80));
                break;
            case 14:
                fb.pen(rgba(0xff, 0x69, 0xb4));
                break;
            }

            fb.rectangle(rect(
                x,
                y,
                width,
                height));
        }
    }

    void draw_subtitle_text() {
        fb.pen(rgba(0xff, 0xff, 0xff));
        fb.text("Greta", &minimal_font[0][0], point(95, 65));
        fb.text("Thunbergs", &minimal_font[0][0], point(95, 75));
        fb.text("Apocolypse", &minimal_font[0][0], point(95, 85));
    }

    void draw_startbutton_text(std::string text) {
        fb.pen(rgba(0xff, 0xff, 0xff));
        blit::rect text_rect(10, 105, SCREEN_W-20, 20);
        bool variable_width = false;
        blit::text_align alignment = blit::text_align::center_center;
        text = blit::fb.wrap_text(text, text_rect.w, &minimal_font[0][0], variable_width);
        blit::fb.text(text, &minimal_font[0][0], text_rect, variable_width, alignment);
    }

    void draw_title()
    {
        draw_background();
        draw_greta(0, big_greta_y);
        draw_logo(80, 30);
        draw_animated_boxes();
        draw_subtitle_text();
        draw_startbutton_text("Press A to start");
    }

    void play_soundtrack()
    {
        audio::channels[0].frequency = 140;
        audio::channels[0].trigger_attack();
    }


} title_;

struct Game
{
    float_t t = 0.0;

    void render(uint32_t time_ms)
    {
        fb.pen(rgba(0, 0, 0));
        fb.clear();

        // draw_background();
        t = t + 0.3;
    }

    void update(uint32_t time_ms)
    {
        //
    }

} game_;

void load_assets()
{
    fb.sprites = spritesheet::load(packed_data);
}


void greta_move_update(blit::timer &t)
{
    if(big_greta_y == 2) {
        t.stop();
    }
    big_greta_y = big_greta_y - 2;
}

void init_audio()
{
    // configure voices
    // melody track
    audio::channels[0].voices = audio::audio_voice::TRIANGLE | audio::audio_voice::SQUARE;
    audio::channels[0].attack_ms = 16;
    audio::channels[0].decay_ms = 168;
    audio::channels[0].sustain = 0xafff;
    audio::channels[0].release_ms = 168;

    // set global volume
    audio::volume = 0x0;
}

/* setup */
void init()
{
    // std::cout << "init\n";
    set_screen_mode(lores);

    // Load sprites
    load_assets();

    // Start timers
    greta_move_timer.init(greta_move_update, 10, -1);
    greta_move_timer.start();
}

void render(uint32_t time_ms)
{
    switch(game_state) {
        case enum_state::play:
            game_.render(time_ms);
            break;
        case enum_state::menu:
            title_.render(time_ms);
            break;
        case enum_state::dead:
            // endgame_.render(time_ms);
            break;
    }
}

void update(uint32_t time_ms)
{
    switch(game_state) {
        case enum_state::play:
            game_.update(time_ms);
            break;
        case enum_state::menu:
            title_.update(time_ms);
            break;
        case enum_state::dead:
            // endgame_.update(time_ms);
            break;
    }
}