#include "cr90_ltcd_Game.h"

#include "bn_blending.h"
#include "bn_fixed_point.h"
#include "bn_format.h"
#include "bn_window.h"

#include "mj/mj_game_list.h"

#include "bn_regular_bg_items_cr90_ltcd_black.h"
#include "bn_regular_bg_items_cr90_ltcd_cake.h"

namespace
{
constexpr bn::string_view code_credits[] = {"copyrat90"};
constexpr bn::string_view graphics_credits[] = {"copyrat90"};
} // namespace

MJ_GAME_LIST_ADD(cr90::ltcd::Game)
MJ_GAME_LIST_ADD_CODE_CREDITS(code_credits)
MJ_GAME_LIST_ADD_GRAPHICS_CREDITS(graphics_credits)
// MJ_GAME_LIST_ADD_MUSIC_CREDITS(music_credits)
// MJ_GAME_LIST_ADD_SFX_CREDITS(sfx_credits)

namespace cr90::ltcd
{

static int init_candle_count(mj::difficulty_level difficulty)
{
    constexpr bn::array<int, 3> CANDLE_COUNTS = {4, 5, 7};

    return CANDLE_COUNTS[static_cast<int>(difficulty)];
}

static bool init_matchstick_fire(mj::difficulty_level difficulty, bn::random& random)
{
    if (difficulty == mj::difficulty_level::EASY)
        return true;
    if (difficulty == mj::difficulty_level::NORMAL)
        return random.get_fixed(0, 1) <= 0.65;
    // difficulty: HARD
    return random.get_fixed(0, 1) <= 0.35;
}

Game::Game(int completed_games, const mj::game_data& data)
    : _difficulty(recommended_difficulty_level(completed_games, data)), _candles(init_candle_count(_difficulty)),
      _bg_cake(bn::regular_bg_items::cr90_ltcd_cake.create_bg((256 - 240) / 2, (256 - 160) / 2)),
      _bg_black(bn::regular_bg_items::cr90_ltcd_black.create_bg((256 - 240) / 2, (256 - 160) / 2)),
      _matchstick(bn::fixed_point(-20, -20), init_matchstick_fire(_difficulty, data.random)),
      _total_frames(play_jingle(mj::game_jingle_type::TOTSNUK14, completed_games, data))
{
    _bg_black.set_priority(2);
    _bg_black.set_blending_enabled(true);

    bn::blending::set_transparency_alpha(0.5);

    bn::window::sprites().set_show_bg(_bg_black, false);
}

auto Game::title() const -> bn::string<16>
{
    return bn::format<16>("Light {} candle{}!", _candles, (_candles > 1 ? "s" : ""));
}

int Game::total_frames() const
{
    return _total_frames;
}

bool Game::victory() const
{
    return _victory;
}

auto Game::particles() -> LightParticles&
{
    return _particles;
}

void Game::fade_in(const mj::game_data& data)
{
    update(data);
}

auto Game::play(const mj::game_data& data) -> mj::game_result
{
    auto result = handle_input(data);
    auto result_2 = update(data);

    result.exit |= result_2.exit;
    result.remove_title |= result_2.remove_title;

    return result;
}

void Game::fade_out(const mj::game_data& data)
{
    fade_in(data);
}

auto Game::handle_input(const mj::game_data& data) -> mj::game_result
{
    _matchstick.handle_input(data, *this);

    return mj::game_result{.exit = false, .remove_title = false};
}

auto Game::update(const mj::game_data& data) -> mj::game_result
{
    _matchstick.update(data, *this);
    _particles.update(data, *this);

    return mj::game_result{.exit = false, .remove_title = false};
}

} // namespace cr90::ltcd
