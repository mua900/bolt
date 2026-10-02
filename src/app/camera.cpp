#include "camera.hpp"

namespace bolt
{

    Camera init_camera()
    {
        Camera camera = {};
        camera.position = { 0, 0 };
        camera.zoom = 1;
        camera.rotation = 0;

        return camera;
    }

    bolt::vec2 Camera::world_to_screen(bolt::vec2 p) const
    {
        p = (p - position).rotated(rotation);
        p *= zoom;
        return bolt::vec2(p.x, -p.y);
    }

    bolt::vec2 Camera::screen_to_world(bolt::vec2 p) const
    {
        auto q = bolt::vec2(p.x, -p.y);
        q /= zoom;
        q = q.rotated(-rotation) + position;
        return q;
    }

} // namespace
