#include "mj/mj_game_scene.h"

#include "bn_bg_palettes.h"
#include "bn_colors.h"
#include "bn_sprite_palettes.h"

#include "mj/mj_core.h"
#include "mj/mj_scene_type.h"

#include "bn_regular_bg_items_mj_big_pumpkin_1.h"
#include "bn_regular_bg_items_mj_big_pumpkin_2.h"
#include "bn_regular_bg_items_mj_big_pumpkin_3.h"
#include "bn_regular_bg_items_mj_big_pumpkin_4.h"
#include "bn_regular_bg_items_mj_big_pumpkin_5.h"
#include "bn_regular_bg_items_mj_big_pumpkin_6.h"
#include "bn_regular_bg_items_mj_big_pumpkin_7.h"
#include "bn_regular_bg_items_mj_big_pumpkin_8.h"
#include "bn_regular_bg_items_mj_big_pumpkin_9.h"
#include "bn_regular_bg_items_mj_big_pumpkin_10.h"
#include "bn_regular_bg_items_mj_big_pumpkin_11.h"
#include "bn_regular_bg_items_mj_big_pumpkin_12.h"

namespace mj
{

namespace
{
    constexpr int exit_frames = 64;
}

game_scene::game_scene(core& core) :
    _core(core),
    _data({ core.text_generator(), core.small_text_generator(), core.random(), 0 }),
    _pause(core)
{
    _print_info();
    _lives.show();
}

game_scene::~game_scene()
{
    bn::bg_palettes::set_fade_intensity(0);
    bn::sprite_palettes::set_fade_intensity(0);
}

bn::optional<scene_type> game_scene::update()
{
    bn::optional<scene_type> result;

    if(_exit_frames)
    {
        --_exit_frames;

        if(_exit_frames)
        {
            bn::fixed fade_intensity = 1 - (bn::fixed(_exit_frames) / exit_frames);
            bn::bg_palettes::set_fade(bn::colors::black, fade_intensity);
            bn::sprite_palettes::set_fade(bn::colors::black, fade_intensity);
        }
        else
        {
            result = scene_type::GAME;
        }
    }
    else
    {
        bool exit = false;

        if(_pause.update(exit))
        {
            if(exit)
            {
                _exit_frames = exit_frames;
            }
        }
        else
        {
            if(_playing)
            {
                _data.pending_frames = _pending_frames;
                --_pending_frames;

                _update_play();
            }
            else
            {
                _data.pending_frames = 0;

                if(_update_fade())
                {
                    _exit_frames = exit_frames;
                }
            }

            _lives.update();
            _title.update();
            _timer.update(_pending_frames, _total_frames);
        }
    }

    if(! _pause.paused())
    {
        _backdrop.update(_core);
    }

    return result;
}

void game_scene::_print_info()
{
    bn::sprite_text_generator& text_generator = _core.text_generator();
    text_generator.set_bg_priority(0);

    bn::fixed x = 10 - 120;
    bn::fixed y = 16 - 80;
    text_generator.generate(x, y, "Stage: " + bn::to_string<16>(_completed_games + 1), _info_sprites);

    text_generator.set_bg_priority(3);
}

void game_scene::_update_play()
{
    game& game = _game_manager->game();
    game_result game_result = game.play(_data);
    _playing = ! game_result.exit && _pending_frames;

    if(! _playing || game_result.remove_title)
    {
        _title.clear();
    }

    if(! _playing)
    {
        if(game.victory())
        {
            ++_completed_games;
        }
        else
        {
            _lives.decrease();
        }
    }
}

bool game_scene::_update_fade()
{
    bool exit = false;

    if(_big_pumpkin_counter)
    {
        --_big_pumpkin_counter;
    }
    else
    {
        if(_big_pumpkin_inc)
        {
            if(_big_pumpkin_stage < 13)
            {
                ++_big_pumpkin_stage;
            }
            else
            {
                --_big_pumpkin_stage;
                _big_pumpkin_inc = false;
            }
        }
        else
        {
            if(_big_pumpkin_stage > 1)
            {
                --_big_pumpkin_stage;
            }
            else
            {
                if(_lives.left())
                {
                    ++_big_pumpkin_stage;
                    _big_pumpkin_inc = true;
                }
                else
                {
                    exit = true;
                }
            }
        }

        const bn::regular_bg_item* bg_item = nullptr;
        _big_pumpkin_counter = 2;

        switch(_big_pumpkin_stage)
        {

        case 1:
            bg_item = &bn::regular_bg_items::mj_big_pumpkin_1;
            _big_pumpkin_counter = 70;
            break;

        case 2:
            bg_item = &bn::regular_bg_items::mj_big_pumpkin_2;

            if(_big_pumpkin_inc)
            {
                _backdrop.fade_out();
                _lives.hide();
            }
            break;

        case 3:
            bg_item = &bn::regular_bg_items::mj_big_pumpkin_3;
            break;

        case 4:
            bg_item = &bn::regular_bg_items::mj_big_pumpkin_4;

            if(! _big_pumpkin_inc)
            {
                _game_manager.reset();
                _backdrop.fade_in();
                _print_info();
                _lives.show();
                _pending_frames = 0;
                _total_frames = 1;
            }
            break;

        case 5:
            bg_item = &bn::regular_bg_items::mj_big_pumpkin_5;

            if(_big_pumpkin_inc)
            {
                _info_sprites.clear();

                game_manager& game_manager = _game_manager.emplace(_completed_games, _data, _core);
                int total_frames = game_manager.game().total_frames();
                BN_ASSERT(total_frames >= game::minimum_frames && total_frames <= game::maximum_frames,
                          "Invalid game total frames: ", total_frames);

                _pending_frames = total_frames;
                _total_frames = total_frames;
            }
            break;

        case 6:
            bg_item = &bn::regular_bg_items::mj_big_pumpkin_6;

            if(_big_pumpkin_inc)
            {
                _title.show(_game_manager->game().title(), _core);
            }
            break;

        case 7:
            bg_item = &bn::regular_bg_items::mj_big_pumpkin_7;
            break;

        case 8:
            bg_item = &bn::regular_bg_items::mj_big_pumpkin_8;
            break;

        case 9:
            bg_item = &bn::regular_bg_items::mj_big_pumpkin_9;
            break;

        case 10:
            bg_item = &bn::regular_bg_items::mj_big_pumpkin_10;
            break;

        case 11:
            bg_item = &bn::regular_bg_items::mj_big_pumpkin_11;
            break;

        case 12:
            bg_item = &bn::regular_bg_items::mj_big_pumpkin_12;
            break;

        default:
            if(_big_pumpkin_inc)
            {
                _playing = true;
                _big_pumpkin_counter = 0;
            }
            else
            {
                _big_pumpkin_counter = 60;
            }
            break;
        }

        if(bg_item)
        {
            if(_big_pumpkin)
            {
                _big_pumpkin->set_item(*bg_item);
            }
            else
            {
                bn::regular_bg_ptr big_pumpkin = bg_item->create_bg(0, (256 - 160) / 2);
                big_pumpkin.set_priority(0);
                _big_pumpkin = bn::move(big_pumpkin);
            }
        }
        else
        {
            _big_pumpkin.reset();
        }
    }

    if(! _playing)
    {
        if(_game_manager)
        {
            game& game = _game_manager->game();

            if(_big_pumpkin_inc)
            {
                game.fade_in(_data);
            }
            else
            {
                game.fade_out(_data);

                if(_pending_frames)
                {
                    --_pending_frames;
                }
            }
        }
    }

    return exit;
}

}
