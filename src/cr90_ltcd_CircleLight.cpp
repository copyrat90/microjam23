#include "cr90_ltcd_CircleLight.h"

#include "bn_sprite_items_cr90_ltcd_light.h"

namespace cr90::ltcd
{

namespace
{
constexpr int MIN_RADIUS = 0;
constexpr int MAX_RADIUS = 32;
} // namespace

CircleLight::CircleLight(const bn::fixed_point& position, int radius)
    : Entity(position),
      _spr(bn::sprite_items::cr90_ltcd_light.create_sprite(position, bn::clamp(radius, MIN_RADIUS, MAX_RADIUS)))
{
    BN_ASSERT(MIN_RADIUS <= radius && radius <= MAX_RADIUS, "CircleLight radius=", radius, " not in [", MIN_RADIUS,
              "..", MAX_RADIUS, "]");

    _spr.set_window_enabled(true);
}

void CircleLight::update(const mj::game_data&)
{
    if (animation_running())
        _anim->update();
}

void CircleLight::set_radius(int radius)
{
    BN_ASSERT(MIN_RADIUS <= radius && radius <= MAX_RADIUS, "CircleLight radius=", radius, " not in [", MIN_RADIUS,
              "..", MAX_RADIUS, "]");

    _spr.set_tiles(bn::sprite_items::cr90_ltcd_light.tiles_item(), radius);
}

void CircleLight::set_radius_animation(bool forever, int wait_updates, const bn::span<const uint16_t>& radiuses)
{
    constexpr const auto& TILES = bn::sprite_items::cr90_ltcd_light.tiles_item();

    if (forever)
        _anim = bn::sprite_animate_action<MAX_FRAMES>::forever(_spr, wait_updates, TILES, radiuses);
    else
        _anim = bn::sprite_animate_action<MAX_FRAMES>::once(_spr, wait_updates, TILES, radiuses);
}

bool CircleLight::animation_running() const
{
    return _anim.has_value() && !_anim->done();
}

void CircleLight::set_position(const bn::fixed_point& position)
{
    Entity::set_position(position);

    _spr.set_position(position);
}

void CircleLight::set_x(bn::fixed x)
{
    Entity::set_x(x);

    _spr.set_x(x);
}

void CircleLight::set_y(bn::fixed y)
{
    Entity::set_y(y);

    _spr.set_y(y);
}

} // namespace cr90::ltcd
