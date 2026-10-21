#include "physics.hpp"

namespace bolt
{
    bool aabb_overlap(Rectangle& r1, Rectangle& r2)
    {
        return  ((r1.x < r2.x && r1.x + r1.w / 2 < r2.x) ||
                 (r2.x < r1.x && r2.x + r2.w / 2 < r1.x) ||
                 (r1.y < r2.y && r1.y + r1.h / 2 < r2.y) ||
                 (r2.y < r1.y && r2.y + r2.h / 2 < r1.y));
    }

    bool point_in_rectangle(Rectangle& rect, vec2 p)
    {
        p -= rect.get_position();
        return (std::fabsf(p.x) < rect.w / 2) && (std::fabsf(p.y) < rect.h / 2);
    }

    bool point_in_rectangle(Rectangle& rect, float rot, vec2 p)
    {
        p -= rect.get_position();
        float c = std::cos(rot);
        float s = std::sin(rot);
        vec2 right = vec2(c,s);
        vec2 up = vec2(-s, c);
        return  std::fabsf(dot2(p, right)) <= rect.w / 2 &&
                std::fabsf(dot2(p, up) <= rect.h / 2);
    }
} // namespace