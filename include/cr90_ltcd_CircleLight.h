#pragma once

#include "cr90_ltcd_Entity.h"

#include "bn_sprite_animate_actions.h"
#include "bn_sprite_ptr.h"

namespace cr90::ltcd
{

class CircleLight final : public Entity
{
public:
    static constexpr int MAX_FRAMES = 8;

public:
    CircleLight(const bn::fixed_point& position, int radius);

    void update(const mj::game_data&) override;

public:
    /**
     * @brief Set the `CircleLight` sprite radius.
     * Calling this stops animation, too.
     *
     * @param radius radius in the range [0..32].
     */
    void set_radius(int radius);

    /**
     * @brief Set the `CircleLight` sprite animation.
     *
     * @param radiuses radiuses in the range [0..32]. max frames are 8.
     */
    void set_radius_animation(bool forever, int wait_updates, const bn::span<const uint16_t>& radiuses);

    bool animation_running() const;

public:
    void set_position(const bn::fixed_point&) override;
    void set_x(bn::fixed) override;
    void set_y(bn::fixed) override;

private:
    bn::sprite_ptr _spr;
    bn::optional<bn::sprite_animate_action<MAX_FRAMES>> _anim;
};

} // namespace cr90::ltcd
