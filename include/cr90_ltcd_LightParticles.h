#pragma once

#include "bn_deque.h"

#include "cr90_ltcd_CircleLight.h"

namespace cr90::ltcd
{

class LightParticles
{
public:
    static constexpr int MAX_COUNT = 32;

public:
    void add_particle(const bn::fixed_point& position);
    void update(const mj::game_data&, Game&);

private:
    bn::deque<CircleLight, MAX_COUNT> _particles_queue;
};

} // namespace cr90::ltcd
