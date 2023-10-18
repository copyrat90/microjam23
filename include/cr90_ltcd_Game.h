#pragma once

#include "mj/mj_game.h"

#include "bn_regular_bg_ptr.h"
#include "bn_vector.h"

#include "cr90_ltcd_Candle.h"
#include "cr90_ltcd_LightParticles.h"
#include "cr90_ltcd_Matchstick.h"

namespace cr90::ltcd
{

class Game final : public mj::game
{
public:
    static constexpr int MAX_CANDLES = 8;

public:
    Game(int completed_games, const mj::game_data&);

    auto title() const -> bn::string<16> final;

    int total_frames() const final;
    bool victory() const final;

public:
    auto particles() -> LightParticles&;

public:
    void fade_in(const mj::game_data&) final;
    auto play(const mj::game_data&) -> mj::game_result final;
    void fade_out(const mj::game_data&) final;

public:
    auto handle_input(const mj::game_data&) -> mj::game_result;
    auto update(const mj::game_data&) -> mj::game_result;

private:
    const mj::difficulty_level _difficulty;

    int _not_fire_candles_count;

    bn::regular_bg_ptr _bg_cake;
    bn::regular_bg_ptr _bg_black;

    Matchstick _matchstick;
    bn::vector<Candle, MAX_CANDLES> _candles;
    LightParticles _particles;

    int _total_frames;
    int _show_result_frames = 60;
    bool _victory = false;
    bool _defeat = false;
};

} // namespace cr90::ltcd
