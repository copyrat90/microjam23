#pragma once

#include "cr90_ltcd_Entity.h"

#include "bn_sprite_ptr.h"

#include "cr90_ltcd_CircleCollider.h"
#include "cr90_ltcd_CircleLight.h"

namespace cr90::ltcd
{

class Matchstick final : public Entity
{
public:
    Matchstick(const bn::fixed_point& position, bool fire);

    void handle_input(const mj::game_data&, Game&) override;
    void update(const mj::game_data&, Game&) override;

public:
    bool fire() const;
    void set_fire(bool);

    auto collider() const -> const CircleCollider&;

public:
    void set_position(const bn::fixed_point&) override;
    void set_x(bn::fixed) override;
    void set_y(bn::fixed) override;

private:
    bn::sprite_ptr _spr_stick;
    CircleLight _light;

    CircleCollider _collider;

    int _particle_emit_countdown;
    bool _fire;
};

} // namespace cr90::ltcd
