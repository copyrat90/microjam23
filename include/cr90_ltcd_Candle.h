#pragma once

#include "cr90_ltcd_Fireable.h"

#include "bn_sprite_ptr.h"

#include "cr90_ltcd_CircleCollider.h"
#include "cr90_ltcd_CircleLight.h"

namespace cr90::ltcd
{

class Candle final : public Fireable
{
public:
    Candle(const bn::fixed_point& position, bool fire, bool flying);

    void update(const mj::game_data&, Game&) override;

public:
    bool flying() const;

    void set_fire(bool) override;

public:
    void set_position(const bn::fixed_point&) override;
    void set_x(bn::fixed) override;
    void set_y(bn::fixed) override;

private:
    const bool _flying;

    bn::sprite_ptr _spr_candle;
};

} // namespace cr90::ltcd
