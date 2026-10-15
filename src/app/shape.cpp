#include "shape.hpp"

namespace bolt
{

    RectPoints get_rotated_points(Rectangle rect, float angle)
    {
        float s = std::sinf(angle);
        float c = std::cosf(angle);
        float hw = rect.w / 2;
        float hh = rect.h / 2;

        vec2 diag0 = vec2(hw * c - hh * s, hw * s + hh * c);
        vec2 diag1 = vec2(-hw * c - hh * s, -hw * s + hh * c);

    	RectPoints quad;
    	quad.p[QuadTopLeft]     = vec2(rect.x + diag1.x, rect.y - diag1.y);
    	quad.p[QuadTopRight]    = vec2(rect.x + diag0.x, rect.y - diag0.y);
    	quad.p[QuadBottomLeft]  = vec2(rect.x - diag0.x, rect.y + diag0.y);
    	quad.p[QuadBottomRight] = vec2(rect.x - diag1.x, rect.y + diag1.y);

    	return quad;
    }

    Rectangle merge_volumes(Rectangle v1, Rectangle v2)
    {
        Rectangle res = {};
        vec2 p = vec2(v1.x, v1.y) - vec2(v1.w / 2, v1.h / 2);
        vec2 q = vec2(v2.x, v2.y) - vec2(v2.w / 2, v2.h / 2);
        vec2 r = vec2(v1.x, v1.y) + vec2(v1.w / 2, v1.h / 2);
        vec2 w = vec2(v2.x, v2.y) + vec2(v2.w / 2, v2.h / 2);

        vec2 min = vec2(bolt::min(p.x, q.x), bolt::min(p.y, q.y));
        vec2 max = vec2(bolt::max(r.x, w.x), bolt::max(r.y, w.y));

        res.x = (min.x + max.x) / 2;
        res.y = (min.y + max.y) / 2;
        res.w = max.x - min.x;
        res.h = max.y - min.y;

        return res;
    }

    vec2 Rectangle::get_point_at_direction(Direction dir) const {
        switch (dir) {
        case DirNone:      return vec2(x, y);  // ???
        case DirEast:      return vec2(x + w / 2, y);
        case DirWest:      return vec2(x - w / 2, y);
        case DirSouth:     return vec2(x, y - h / 2);
        case DirNorth:     return vec2(x, y + h / 2);
        case DirNorthEast: return vec2(x + w / 2, y + h / 2);
        case DirNorthWest: return vec2(x - w / 2, y + h / 2);
        case DirSouthEast: return vec2(x + w / 2, y - h / 2);
        case DirSouthWest: return vec2(x - w / 2, y - h / 2);
        default:           panic("Invalid direction");
        }
    }

    bool Rectangle::contains_top_left(vec2 p) const
    {
        return p.x >= x && p.x <= x + w &&
            p.y >= y && p.y <= y + h;
    }

    bool Rectangle::contains_centered(vec2 p) const
    {
        return p.x >= x - w / 2 && p.x <= x + w / 2 &&
            p.y >= y - h / 2 && p.y <= y + h / 2;
    }

} // namespace