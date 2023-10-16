#pragma once

#include "bn_fixed_point.h"

namespace mj
{
struct game_data;
}

namespace cr90::ltcd
{

class Entity
{
public:
    virtual ~Entity() = 0;
    Entity(const bn::fixed_point& position);

    virtual void handle_input(const mj::game_data&){};
    virtual void update(const mj::game_data&){};

public:
    auto position() const -> const bn::fixed_point&;
    auto x() const -> bn::fixed;
    auto y() const -> bn::fixed;

    virtual void set_position(const bn::fixed_point&);
    virtual void set_x(bn::fixed);
    virtual void set_y(bn::fixed);

private:
    bn::fixed_point _position;
};

} // namespace cr90::ltcd
