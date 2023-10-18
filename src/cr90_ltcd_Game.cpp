#include "cr90_ltcd_Game.h"

#include "bn_blending.h"
#include "bn_display.h"
#include "bn_fixed_rect.h"
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

namespace
{

constexpr bn::fixed_rect GROUNDED_CANDLE_RANGE = []() {
    bn::fixed_rect result;
    constexpr int CANDLE_BOTTOM_DIFF = 48;

    constexpr bn::fixed_point TOP_LEFT = {
        40 - bn::display::width() / 2,
        64 - CANDLE_BOTTOM_DIFF - bn::display::height() / 2,
    };
    constexpr bn::fixed_point BOTTOM_RIGHT = {
        206 - bn::display::width() / 2,
        124 - CANDLE_BOTTOM_DIFF - bn::display::height() / 2,
    };

    result.set_position((TOP_LEFT.x() + BOTTOM_RIGHT.x()) / 2, (TOP_LEFT.y() + BOTTOM_RIGHT.y()) / 2);
    result.set_dimensions(BOTTOM_RIGHT.x() - TOP_LEFT.x(), BOTTOM_RIGHT.y() - TOP_LEFT.y());

    return result;
}();

} // namespace

static int init_not_fire_candles_count(mj::difficulty_level difficulty)
{
    constexpr bn::array<int, 3> NOT_FIRE_CANDLES_COUNTS = {4, 5, 7};

    static_assert(
        [NOT_FIRE_CANDLES_COUNTS]() -> bool {
            for (int note_fire_candles_count : NOT_FIRE_CANDLES_COUNTS)
                if (note_fire_candles_count + 1 > Game::MAX_CANDLES)
                    return false;
            return true;
        }(),
        "`not_fire_candles_count` must be lower than `MAX_CANDLES`");

    return NOT_FIRE_CANDLES_COUNTS[(int)(difficulty)];
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
    : _difficulty(recommended_difficulty_level(completed_games, data)),
      _not_fire_candles_count(init_not_fire_candles_count(_difficulty)),
      _bg_cake(bn::regular_bg_items::cr90_ltcd_cake.create_bg((256 - 240) / 2, (256 - 160) / 2)),
      _bg_black(bn::regular_bg_items::cr90_ltcd_black.create_bg((256 - 240) / 2, (256 - 160) / 2)),
      _matchstick(bn::fixed_point(-20, -20), init_matchstick_fire(_difficulty, data.random)),
      _total_frames(play_jingle(mj::game_jingle_type::TOTSNUK14, completed_games, data))
{
    // init shadow effect
    _bg_black.set_priority(2);
    _bg_black.set_blending_enabled(true);
    bn::blending::set_transparency_alpha(0.5);
    bn::window::sprites().set_show_bg(_bg_black, false);

    // init candles
    for (int i = 0; i < _not_fire_candles_count + !_matchstick.fire(); ++i)
    {
        const auto x = data.random.get_fixed(GROUNDED_CANDLE_RANGE.left(), GROUNDED_CANDLE_RANGE.right());
        const auto y = data.random.get_fixed(GROUNDED_CANDLE_RANGE.top(), GROUNDED_CANDLE_RANGE.bottom());

        Candle candle({x, y}, (i >= _not_fire_candles_count));
        _candles.push_back(std::move(candle));
    }
}

auto Game::title() const -> bn::string<16>
{
    return bn::format<16>("Light {} candle{}!", _not_fire_candles_count, (_not_fire_candles_count > 1 ? "s" : ""));
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
    for (auto& candle : _candles)
        candle.update(data, *this);

    _particles.update(data, *this);

    return mj::game_result{.exit = false, .remove_title = false};
}

} // namespace cr90::ltcd
