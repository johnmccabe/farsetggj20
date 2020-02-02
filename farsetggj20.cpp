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

#define PLAYER_W 8
#define PLAYER_H 16

#define TILE_SIZE 8

vec2 player_position(80.0f, SCREEN_H - PLAYER_H - TILE_SIZE);

#define PLAYER_TOP (player_position.y)
#define PLAYER_BOTTOM (player_position.y + PLAYER_H)
#define PLAYER_RIGHT (player_position.x + PLAYER_W)
#define PLAYER_LEFT (player_position.x)

// Keep track of game state
enum enum_state {
    menu = 0,
    intro = 1,
    play = 2,
    dead = 3
};
enum_state game_state = enum_state::menu;

blit::timer greta_move_timer;
int big_greta_y = 120;

struct Title {
    float t = 0.0;
    uint8_t start_text_bounce = 0;

    void render(uint32_t time_ms)
    {
        fb.pen(rgba(0, 0, 0));
        fb.clear();

        t = t + (float)0.3;
        draw_title();
        play_soundtrack();
    }

    void update(uint32_t time_ms, uint16_t btn_down, uint16_t btn_up)
    {
        if((btn_down & button::A) || (btn_down & button::B) || (btn_down & button::X) || (btn_down & button::Y) || (btn_down & button::HOME) || (btn_down & button::MENU))
        {
            game_state = enum_state::intro;
        }
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
            int32_t x = (SCREEN_W / 2) + cos(t * (float)i / (float)150.0) * (float)60.0;
            int32_t y = 108;
            int32_t width = abs((cos(t * (float)i / (float)150.0) * (float)60.0) - (cos(t * (float)i / (float)150.0 + (float)0.02) * (float)60.0)) * 10;
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

    uint8_t BOUNCE_MAX = 60;

    void draw_startbutton_text(std::string text) {
        if (start_text_bounce < BOUNCE_MAX)
        {
            fb.pen(rgba(0x00, 0x00, 0x00));
        }
        if (start_text_bounce < BOUNCE_MAX/2)
        {
            fb.pen(rgba(0xff, 0xff, 0xff));
        }
        start_text_bounce++;
        if (start_text_bounce == BOUNCE_MAX) {
            start_text_bounce = 0;
        }

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

struct Intro {
    void render(uint32_t time_ms)
    {
        fb.pen(rgba(0, 0, 0));
        fb.clear();
        draw_intro();
    }

    void update(uint32_t time_ms, uint16_t btn_down, uint16_t btn_up)
    {
        if((btn_down & button::A) || (btn_down & button::B) || (btn_down & button::X) || (btn_down & button::Y) || (btn_down & button::HOME) || (btn_down & button::MENU))
        {
            game_state = enum_state::play;
        }
    }

    void draw_background()
    {

    }

    void draw_text()
    {
        std::string intro_text[] = {
            "It is the year 2050.",
            "Planet Earth is broken.", 
            "Humanity's last hope rests ", 
            "with a young girl...", 
            "well an old young girl...", 
            "or to be precise an army", 
            "of young clones.", 
            "", 
            "Only the Greta swarm can", 
            "repair the environment and", 
            "prevent the apocalypse"
        };

        uint8_t xorigin = 14;
        uint8_t yorigin = 5;
        uint8_t line_height = 10;
        uint8_t i = 0;
        fb.pen(rgba(0xff, 0xff, 0xff));

        for(const std::string &text : intro_text) {
            fb.text(text, &minimal_font[0][0], point(xorigin, (yorigin + (i * line_height))));
            i++;
        }
    }

    void draw_intro()
    {
        draw_background();
        draw_text();
    }

} intro_;

struct Flower {
    vec2 flower_position = vec2(76, 56);

    void draw_flower(vec2 position) {
        fb.sprite(rect(11,6,1,1), position);
    }

    void update(uint32_t time_ms, uint16_t btn_down, uint16_t btn_up) {
        //
    }

    void render(uint32_t time_ms) {
        draw_flower(flower_position);
    }

} flower_;

struct Player {

    void draw_player(vec2 position) {
        fb.sprite(rect(12,4,1,2), position);
    }

    void update(uint32_t time_ms, uint16_t btn_down, uint16_t btn_up) {

        int16_t JOY_DEADZONE = 300;

        int16_t JOY_X = (int16_t)(blit::joystick.x * 1024);
        int16_t JOY_Y = (int16_t)(blit::joystick.y * 1024);

        if((pressed(button::DPAD_UP)) || (JOY_Y < -JOY_DEADZONE)) {
            if (player_position.y != TILE_SIZE)
            {
                player_position.y--;
            }
        }
        if((pressed(button::DPAD_DOWN)) || (JOY_Y > JOY_DEADZONE)) {
            if (player_position.y + PLAYER_H != SCREEN_H - TILE_SIZE)
            {
                player_position.y++;
            }
        }
        if((pressed(button::DPAD_RIGHT)) || (JOY_X > JOY_DEADZONE)) {
            if (player_position.x + PLAYER_W != SCREEN_W - TILE_SIZE)
            {
                player_position.x++;
            }
        }
        if((pressed(button::DPAD_LEFT)) || (JOY_X < -JOY_DEADZONE)) {
            if (player_position.x != TILE_SIZE)
            {
                player_position.x--;
            }
        }
    }

    bool flower_collison(struct Flower flower) {
        
        uint8_t flower_top = (uint8_t)flower.flower_position.y;
        uint8_t flower_bottom = (uint8_t)flower.flower_position.y + TILE_SIZE;
        uint8_t flower_left = (uint8_t)flower.flower_position.x;
        uint8_t flower_right = (uint8_t)flower.flower_position.x + TILE_SIZE;

        if ((flower_bottom > (uint8_t)PLAYER_TOP) && (flower_top < (uint8_t)PLAYER_BOTTOM) && (flower_right > (uint8_t)PLAYER_LEFT) && (flower_left < (uint8_t)PLAYER_RIGHT))
        {
            return true;
        }

        return false;
    }

    void render(uint32_t time_ms) {
        draw_player(player_position);
    }

} player_;

struct Game
{
    uint16_t flower_love = 0;

    void render(uint32_t time_ms)
    {
        fb.pen(rgba(0, 0, 0));
        fb.clear();

        player_.render(time_ms);
        flower_.render(time_ms);

        draw_border();

        char text_buf[100] = {0};
        fb.pen(rgba(0xff, 0xff, 0xff));
        sprintf(text_buf, "Flower Love: %d", flower_love);
        fb.text(text_buf, &minimal_font[0][0], point(10, 8));
    }

    void draw_border_brick(vec2 position) {
        fb.sprite(rect(11,7,1,1), position);
    }

    void draw_border()
    {
        for (int x = 0; x < SCREEN_W; x = x + TILE_SIZE)
        {
            draw_border_brick(vec2(x, 0));
            draw_border_brick(vec2(x, SCREEN_H - TILE_SIZE));
        }

        for (int y = 0; y < SCREEN_H; y = y + TILE_SIZE)
        {
            draw_border_brick(vec2(0, y));
            draw_border_brick(vec2(SCREEN_W - TILE_SIZE, y));
        }
    }

    void update(uint32_t time_ms, uint16_t btn_down, uint16_t btn_up)
    {   
        player_.update(time_ms, btn_down, btn_up);
        if (player_.flower_collison(flower_)) {
            flower_love++;
        }
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
        case enum_state::intro:
            intro_.render(time_ms);
            break;
        case enum_state::dead:
            // endgame_.render(time_ms);
            break;
    }
}

void update(uint32_t time_ms)
{
    static uint16_t last_buttons = 0;
    uint16_t changed = buttons ^ last_buttons;
    uint16_t btn_down = changed & buttons;
    uint16_t btn_up = changed & ~buttons;

    switch(game_state) {
        case enum_state::play:
            game_.update(time_ms, btn_down, btn_up);
            break;
        case enum_state::menu:
            title_.update(time_ms, btn_down, btn_up);
            break;
        case enum_state::intro:
            intro_.update(time_ms, btn_down, btn_up);
            break;
        case enum_state::dead:
            // endgame_.update(time_ms);
            break;
    }

    last_buttons = buttons;
}