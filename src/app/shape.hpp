#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <util/math_util.hpp>
#include <util/template.hpp>

namespace bolt
{

    // it is here to reduce data duplication between different systems like graphics, ui and physics
    // and it needs to server all of them so the code here can be all over the place

    using ShapeId = u32;

    enum ShapeType
    {
        ShapeCircle    = 0,
        ShapeRectangle = 1,
        ShapePolygon   = 2,
        ShapeCount
    };

    struct Circle
    {
        vec2 center = {};
        float radius = 0;
    };

    struct Rectangle {
        float x = 0.0f;
        float y = 0.0f;
        float w = 0.0f;
        float h = 0.0f;

        Rectangle() {}
        Rectangle(vec2 pos, vec2 scale) : x(pos.x), y(pos.y), w(scale.x), h(scale.y) {}
        Rectangle(float p_x, float p_y, float p_w, float p_h)
            : x(p_x), y(p_y), w(p_w), h(p_h)
        {}

        bool contains_top_left(vec2 p) const;
        bool contains_centered(vec2 p) const;
        Rectangle to_top_left() const {
            return Rectangle(x - w / 2, y - h / 2, w, h);
        }
        Rectangle to_center() const {
            return Rectangle(x + w / 2, y + h / 2, w, h);
        }

        Direction on_edge(vec2 position, float d) const
        {
            vec2 topLeft = get_top_left();
            vec2 relative = position - topLeft;

            bool left = std::fabsf(relative.x) < d;
            bool right = std::fabsf(relative.x - w) < d;
            bool down = std::fabsf(relative.y) < d;
            bool up = std::fabsf(relative.y - h) < d;

            bool horizontal = topLeft.x - d <= position.x && topLeft.x + w + d >= position.x;
            bool vertical = topLeft.y - d <= position.y && topLeft.y + h + d >= position.y;

            Direction west = (left && vertical) ? DirWest : DirNone;
            Direction east = (right && vertical) ? DirEast : DirNone;
            Direction south = (down && horizontal) ? DirSouth : DirNone;
            Direction north = (up && horizontal) ? DirNorth : DirNone;

            return Direction(west | east | south | north);
        }

        vec2 get_point_at_direction(Direction dir) const;

        vec2 get_position() const {
            return vec2(x, y);
        }

        vec2 get_scale() const {
            return vec2(w, h);
        }

        vec2 get_top_left() const {
            return vec2(x - w / 2, y - h / 2);
        }
        vec2 get_center() const {
            return vec2(x + w / 2, y +h / 2);
        }
    };

    Rectangle merge_volumes(Rectangle v1, Rectangle v2);

    enum QuadVertexPosition {
        QuadTopLeft = 0,
        QuadTopRight = 1,
        QuadBottomLeft = 2,
        QuadBottomRight = 3,
    };

    struct RectPoints {
    	vec2 p[4];
    };

    RectPoints get_rotated_points(Rectangle rect, float rot);

    #define POLYGON_MAX_VERTICES 8
    struct Polygon
    {
        vec2 vertices[POLYGON_MAX_VERTICES];
        int num_vertices = 0;

        vec2 get_vertex_in_direction(vec2 dir) const;  // highest dot product
        void sort_vertices();  // sort in ascending order according to their angle with vec2(1,0)
    };

    struct Shape
    {
        ShapeType type = {};
        union {
            Rectangle rect;
            Circle circle;
            Polygon polygon;
        } data = {};
    };

    struct ShapeData
    {
        BucketList<Shape> shapes = {};
    };

} // namespace

#endif // SHAPE_HPP