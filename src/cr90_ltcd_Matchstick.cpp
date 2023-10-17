#include "cr90_ltcd_Matchstick.h"

#include "bn_fixed_point.h"
#include "bn_keypad.h"

#include "cr90_ltcd_Game.h"

#include "bn_sprite_items_cr90_ltcd_matchstick.h"

namespace cr90::ltcd
{

namespace
{
constexpr auto STICK_DIFF = bn::fixed_point(-15, -7);
constexpr auto LIGHT_DIFF = bn::fixed_point(0, 0);
constexpr auto COLL_DIFF = bn::fixed_point(0, 0);

constexpr int LIGHT_RADIUS = 15;
constexpr int COLL_RADIUS = 8;

constexpr int PARTICLE_INTERVAL = 8;

} // namespace

Matchstick::Matchstick(const bn::fixed_point& position, bool fire)
    : Entity(position), _spr_stick(bn::sprite_items::cr90_ltcd_matchstick.create_sprite(position + STICK_DIFF, fire)),
      _light(position, (fire ? LIGHT_RADIUS : 0)), _collider(position + COLL_DIFF, COLL_RADIUS),
      _particle_emit_countdown(PARTICLE_INTERVAL), _fire(fire)
{
    set_position(position);
    set_fire(fire);

    _spr_stick.set_bg_priority(2);
}

void Matchstick::handle_input(const mj::game_data&, Game&)
{
    constexpr bn::fixed MOVE_SPEED = 2.0f;

    if (bn::keypad::left_held())
        set_x(x() - MOVE_SPEED);
    else if (bn::keypad::right_held())
        set_x(x() + MOVE_SPEED);
    if (bn::keypad::up_held())
        set_y(y() - MOVE_SPEED);
    else if (bn::keypad::down_held())
        set_y(y() + MOVE_SPEED);
}

void Matchstick::update(const mj::game_data& data, Game& game)
{
    _light.update(data, game);

    if (_fire)
    {
        if (--_particle_emit_countdown <= 0)
        {
            _particle_emit_countdown = PARTICLE_INTERVAL;

            auto particle_position = position();
            particle_position.set_x(x() + data.random.get_int(-3, 3));
            particle_position.set_y(y() - 10);
            game.particles().add_particle(particle_position);
        }
    }
}

bool Matchstick::fire() const
{
    return _fire;
}

void Matchstick::set_fire(bool fire)
{
    _fire = fire;

    _spr_stick.set_tiles(bn::sprite_items::cr90_ltcd_matchstick.tiles_item(), fire);

    if (fire)
        _light.set_radius_animation(true, 4, bn::array<const uint16_t, 4>{15, 16, 15, 14});
    else
        _light.set_radius(0);
}

void Matchstick::set_position(const bn::fixed_point& position)
{
    Entity::set_position(position);

    _spr_stick.set_position(position + STICK_DIFF);
    _light.set_position(position + LIGHT_DIFF);
    _collider.set_position(position + COLL_DIFF);
}

void Matchstick::set_x(bn::fixed x)
{
    Entity::set_x(x);

    _spr_stick.set_x(x + STICK_DIFF.x());
    _light.set_x(x + LIGHT_DIFF.x());
    _collider.set_x(x + COLL_DIFF.x());
}

void Matchstick::set_y(bn::fixed y)
{
    Entity::set_y(y);

    _spr_stick.set_y(y + STICK_DIFF.y());
    _light.set_y(y + LIGHT_DIFF.y());
    _collider.set_y(y + COLL_DIFF.y());
}

} // namespace cr90::ltcd
