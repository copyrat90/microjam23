#pragma once

#include "mj/mj_game.h"

#include "bn_regular_bg_ptr.h"

#include "cr90_ltcd_Matchstick.h"

namespace cr90::ltcd
{

class Game final : public mj::game
{
public:
    Game(int completed_games, const mj::game_data&);

    auto title() const -> bn::string<16> final;

    int total_frames() const final;
    bool victory() const final;

public:
    void fade_in(const mj::game_data&) final;
    auto play(const mj::game_data&) -> mj::game_result final;
    void fade_out(const mj::game_data&) final;

public:
    auto handle_input(const mj::game_data&) -> mj::game_result;
    auto update(const mj::game_data&) -> mj::game_result;

private:
    const mj::difficulty_level _difficulty;
    const int _candles;

    bn::regular_bg_ptr _bg_cake;
    bn::regular_bg_ptr _bg_black;

    Matchstick _matchstick;

    int _total_frames;
    int _show_result_frames = 60;
    bool _victory = false;
    bool _defeat = false;
};

} // namespace cr90::ltcd
