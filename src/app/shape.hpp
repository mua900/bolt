#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <util/math_util.hpp>
#include <util/template.hpp>

namespace bolt
{

    using ShapeId = u32;

    struct Shape
    {
        DArray<vec2> points = {};
        DArray<int> indices = {};
    };

    struct ShapeData
    {
        BucketList<Shape> shapes = {};
    };

} // namespace

#endif // SHAPE_HPP