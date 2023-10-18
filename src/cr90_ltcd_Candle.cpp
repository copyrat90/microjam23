#include "cr90_ltcd_Candle.h"

#include "bn_fixed_point.h"

#include "cr90_ltcd_Game.h"

#include "bn_sprite_items_cr90_ltcd_candle.h"

namespace cr90::ltcd
{

namespace
{
constexpr auto CANDLE_DIFF = bn::fixed_point(4, 16);

constexpr int LIGHT_RADIUS = 15;
constexpr int COLL_RADIUS = 8;

constexpr int PARTICLE_INTERVAL = 8;

} // namespace

Candle::Candle(const bn::fixed_point& position, bool fire)
    : Fireable(position, fire, LIGHT_RADIUS, COLL_RADIUS, PARTICLE_INTERVAL),
      _spr_candle(bn::sprite_items::cr90_ltcd_candle.create_sprite(position + CANDLE_DIFF, fire))
{
    set_fire(fire);
    set_position(position);
}

void Candle::update(const mj::game_data& data, Game& game)
{
    Fireable::update(data, game);

    // TODO: Render candle fire animation
}

void Candle::set_fire(bool fire)
{
    Fireable::set_fire(fire);

    _spr_candle.set_tiles(bn::sprite_items::cr90_ltcd_candle.tiles_item(), fire);
}

void Candle::set_position(const bn::fixed_point& position)
{
    Fireable::set_position(position);

    _spr_candle.set_position(position + CANDLE_DIFF);
    _spr_candle.set_z_order((-position.y()).floor_integer());
}

void Candle::set_x(bn::fixed x)
{
    Fireable::set_x(x);

    _spr_candle.set_x(x + CANDLE_DIFF.x());
}

void Candle::set_y(bn::fixed y)
{
    Fireable::set_y(y);

    _spr_candle.set_y(y + CANDLE_DIFF.y());
    _spr_candle.set_z_order((-y).floor_integer());
}

} // namespace cr90::ltcd
