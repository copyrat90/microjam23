#pragma once

#include "cr90_ltcd_Entity.h"

namespace cr90::ltcd
{

class CircleCollider : public Entity
{
public:
    CircleCollider(const bn::fixed_point position, bn::fixed radius);

    bool collide_with(const CircleCollider& other) const;

private:
    bn::fixed _radius;
};

} // namespace cr90::ltcd
