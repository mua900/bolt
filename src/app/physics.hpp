#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "shape.hpp"

namespace bolt
{
    // @todo
    // we probably want to add something like box2d and link it or not based on user configuration
    // but we also want to have some simple replacement
    // with collision detection and resolution, something that abstracts things like solver logic etc.

    bool aabb_overlap(Rectangle& rect1, Rectangle& rect2);
    bool point_in_rectangle(Rectangle& rect, vec2 p);
    bool point_in_rectangle_rotated(Rectangle& rect, float rot, vec2 p);
} // namespace

#endif // PHYSICS_HPP