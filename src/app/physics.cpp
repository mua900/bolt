#include "physics.hpp"

namespace bolt
{
    Direction aabb_overlap(Rectangle& r1, Rectangle& r2)
    {
        bool x = r1.x + r1.w / 2 > r2.x - r2.w / 2 && r2.x + r2.w / 2 > r1.x - r1.w / 2;
        bool y = r1.y + r1.h / 2 > r2.y - r2.h / 2 && r2.y + r2.h / 2 > r1.y - r1.h / 2;

        Direction dir = {};
        if (x) {
            int r = (r1.x < r2.x) ? DirWest : DirEast;
            dir = (Direction) ((int) dir | r);
        }
        if (y) {
            int r = (r1.y < r2.y) ? DirSouth : DirNorth;
            dir = (Direction) ((int) dir | r);
        }

        return dir;
    }

    bool point_in_rectangle(Rectangle& rect, vec2 p)
    {
        p -= rect.get_position();
        return (std::fabs(p.x) < rect.w / 2) && (std::fabs(p.y) < rect.h / 2);
    }

    bool point_in_rectangle(Rectangle& rect, float rot, vec2 p)
    {
        p -= rect.get_position();
        float c = std::cos(rot);
        float s = std::sin(rot);
        vec2 right = vec2(c,s);
        vec2 up = vec2(-s, c);
        return  std::fabs(dot2(p, right)) <= rect.w / 2 &&
                std::fabs(dot2(p, up) <= rect.h / 2);
    }
} // namespace