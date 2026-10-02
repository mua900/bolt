#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "util/common.hpp"
#include "util/math_util.hpp"

namespace bolt
{

    using CameraId = u32;

    struct Camera {
        bolt::vec2 position = {};
        float zoom = {};
        float rotation = 0;

        bolt::vec2 world_to_screen(bolt::vec2 p) const;
        bolt::vec2 screen_to_world(bolt::vec2 p) const;
    };

    // get an camera with identity transform
    Camera init_camera();

} // namespace

#endif // CAMERA_HPP
