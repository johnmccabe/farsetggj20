#include <cstdlib>
#include <algorithm>
#include <array>
#include <math.h>

#include "graphics/color.hpp"
#include "farsetggj20.hpp"
#include "assets.hpp"
#include "greta.hpp"

using namespace blit;

struct Game
{
    bool game_running = false;
    float_t t = 0.0;

    void render(uint32_t time_ms)
    {
        fb.pen(rgba(0, 0, 0));
        fb.clear();

        draw_background();
        t = t + 0.3;

        if (game_running)
        {
            // game loop
        }
        else
        {
            draw_title();
            play_soundtrack();
        }
    }

    void update(uint32_t time_ms)
    {
        //
    }

    void draw_background()
    {
        for (int b = 0; b < SCREEN_WIDTH; b++)
        {
            for (int v = 0; v < SCREEN_HEIGHT; v++)
            {
                fb.pen(blit::hsv_to_rgba(float(b) / (float)(SCREEN_WIDTH), 1.0f, float(v) / (float)(SCREEN_HEIGHT)));
                fb.pixel(point(b, v));
            }
        }
    }

    void draw_title()
    {



        uint8_t sprite_width = 8;
        uint8_t sprite_height = 8;
        uint8_t sprite_index = 0;
        for (uint8_t y = 0; y < 15; y++)
        {
            for (uint8_t x = 0; x < 11; x++)
            {
                fb.sprite(sprite_index, point(x*sprite_width, y*sprite_height));
                sprite_index++;
            }
        }


        for (uint8_t i = 7; i < 15; i++)
        {
            int32_t x = (SCREEN_WIDTH / 2) + cos(t * (float)i / 150.0) * 60.0;
            int32_t y = 78;
            int32_t width = abs((cos(t * (float)i / 150.0) * 60.0) - (cos(t * (float)i / 150.0 + 0.02) * 60.0)) * 10;
            int32_t height = 20;

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

        // uint8_t offset = 36;
        // uint8_t y_pos = 45;

        // uint8_t sprite_width = 32;
        // uint8_t cursor_pos_x = 0;

        // fb.sprite(FontSprite::g, point(offset + (sprite_width * cursor_pos_x++), y_pos), point(0, 0), 4.0f, sprite_transform::NONE);
        // fb.sprite(FontSprite::t, point(offset + (sprite_width * cursor_pos_x++), y_pos), point(0, 0), 4.0f, sprite_transform::NONE);
        // fb.sprite(FontSprite::a, point(offset + (sprite_width * cursor_pos_x), y_pos), point(0, 0), 4.0f, sprite_transform::NONE);

        fb.pen(rgba(0x00, 0x00, 0x00));
        blit::rect text_rect(0, 80, SCREEN_WIDTH, 20);

        bool variable_width = false;

        blit::text_align alignment = blit::text_align::center_center;
        std::string text = "Greta is fucked off";
        text = blit::fb.wrap_text(text, text_rect.w, &outline_font[0][0], variable_width);

        blit::fb.text(text, &outline_font[0][0], text_rect, variable_width, alignment);
    }

    void play_soundtrack()
    {
        audio::channels[0].frequency = 140;
        audio::channels[0].trigger_attack();
    }

} game_;

void load_assets()
{
    // fb.sprites = spritesheet::load(packed_data);
    fb.sprites = spritesheet::load(greta_packed_data);
}

/* setup */
void init()
{
    // std::cout << "init\n";
    set_screen_mode(lores);

    // Load sprites
    load_assets();

    // configure voices
    // melody track
    audio::channels[0].voices = audio::audio_voice::TRIANGLE | audio::audio_voice::SQUARE;
    audio::channels[0].attack_ms = 16;
    audio::channels[0].decay_ms = 168;
    audio::channels[0].sustain = 0xafff;
    audio::channels[0].release_ms = 168;

    // set global volume
    audio::volume = 0x0fff;

    fb.pen(rgba(0, 0, 0, 255));
    fb.clear();
}

void render(uint32_t time_ms)
{
    game_.render(time_ms);
}

void update(uint32_t time_ms)
{
    game_.update(time_ms);
}