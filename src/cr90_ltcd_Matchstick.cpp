#include "cr90_ltcd_Matchstick.h"

#include "bn_display.h"
#include "bn_fixed_point.h"
#include "bn_keypad.h"

#include "cr90_ltcd_Game.h"

#include "bn_sprite_items_cr90_ltcd_matchstick.h"

namespace cr90::ltcd
{

namespace
{
constexpr auto STICK_DIFF = bn::fixed_point(-15, -7);

constexpr int LIGHT_RADIUS = 15;
constexpr int COLL_RADIUS = 8;

constexpr int PARTICLE_INTERVAL = 8;

} // namespace

Matchstick::Matchstick(const bn::fixed_point& position, bool fire)
    : Fireable(position, fire, LIGHT_RADIUS, COLL_RADIUS, PARTICLE_INTERVAL),
      _spr_stick(bn::sprite_items::cr90_ltcd_matchstick.create_sprite(position + STICK_DIFF, fire))
{
    set_fire(fire);

    _spr_stick.set_bg_priority(2);
}

void Matchstick::handle_input(const mj::game_data&, Game&)
{
    constexpr bn::fixed MOVE_SPEED = 2.0f;

    if (bn::keypad::left_held())
        set_x(bn::max(x() - MOVE_SPEED, bn::fixed(-bn::display::width() / 2)));
    else if (bn::keypad::right_held())
        set_x(bn::min(x() + MOVE_SPEED, bn::fixed(+bn::display::width() / 2)));
    if (bn::keypad::up_held())
        set_y(bn::max(y() - MOVE_SPEED, bn::fixed(-bn::display::height() / 2)));
    else if (bn::keypad::down_held())
        set_y(bn::min(y() + MOVE_SPEED, bn::fixed(+bn::display::height() / 2)));
}

void Matchstick::update(const mj::game_data& data, Game& game)
{
    Fireable::update(data, game);

    // TODO: Render matchstick fire animation
}

void Matchstick::set_fire(bool fire)
{
    Fireable::set_fire(fire);

    _spr_stick.set_tiles(bn::sprite_items::cr90_ltcd_matchstick.tiles_item(), fire);
}

void Matchstick::set_position(const bn::fixed_point& position)
{
    Fireable::set_position(position);

    _spr_stick.set_position(position + STICK_DIFF);
}

void Matchstick::set_x(bn::fixed x)
{
    Fireable::set_x(x);

    _spr_stick.set_x(x + STICK_DIFF.x());
}

void Matchstick::set_y(bn::fixed y)
{
    Fireable::set_y(y);

    _spr_stick.set_y(y + STICK_DIFF.y());
}

} // namespace cr90::ltcd
