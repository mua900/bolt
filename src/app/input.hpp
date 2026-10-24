#ifndef INPUT_HPP
#define INPUT_HPP

#include "util/math_util.hpp"
#include "util/common.hpp"
#include "util/value.hpp"

#include <SDL3/SDL.h>

namespace bolt
{

    using MouseButton = SDL_MouseButtonFlags;

    #define MOUSE_LEFT   SDL_BUTTON_LEFT
    #define MOUSE_MIDDLE SDL_BUTTON_MIDDLE
    #define MOUSE_RIGHT  SDL_BUTTON_RIGHT

    #define MOUSE_LEFT_MASK   SDL_BUTTON_LMASK
    #define MOUSE_MIDDLE_MASK SDL_BUTTON_MMASK
    #define MOUSE_RIGHT_MASK  SDL_BUTTON_RMASK

    #define KEY_W       SDL_SCANCODE_W
    #define KEY_D       SDL_SCANCODE_D
    #define KEY_S       SDL_SCANCODE_S
    #define KEY_A       SDL_SCANCODE_A
    #define KEY_Q       SDL_SCANCODE_Q
    #define KEY_E       SDL_SCANCODE_E
    #define KEY_X       SDL_SCANCODE_X
    #define KEY_Z       SDL_SCANCODE_Z
    #define KEY_C       SDL_SCANCODE_C
    #define KEY_SPACE   SDL_SCANCODE_SPACE
    #define KEY_RETURN  SDL_SCANCODE_RETURN
    #define KEY_UP      SDL_SCANCODE_UP
    #define KEY_DOWN    SDL_SCANCODE_DOWN
    #define KEY_RIGHT   SDL_SCANCODE_RIGHT
    #define KEY_LEFT    SDL_SCANCODE_LEFT
    #define KEY_ESCAPE  SDL_SCANCODE_ESCAPE

    #define KEYMOD_LEFT_SHIFT SDL_KMOD_LSHIFT

    using Scancode = SDL_Scancode;

    using KeyboardEvent = SDL_KeyboardEvent;

    struct KeyboardState {
        const bool* keys = {};
        int num_keys = {};
        SDL_Keymod mod_state = {};

        bool do_input = false;

        bool key_pressed(Scancode code) const;
    };

    struct KeyboardAxis {
        SDL_Scancode negative;
        SDL_Scancode positive;
    };

    struct KeyboardVector {
        KeyboardAxis x;
        KeyboardAxis y;
    };

    struct MouseCursor {
        SDL_Cursor* normal = nullptr;
        SDL_Cursor* text = nullptr;
        SDL_Cursor* resize_ns = nullptr;
        SDL_Cursor* resize_ew = nullptr;
    };

    struct MouseState {
        bolt::vec2 pos = {};
        bolt::vec2 motion = {};
        bolt::vec2 wheel = {};
        SDL_MouseButtonFlags buttonFlags = {};
        MouseCursor cursor = {};
        bool down = false;

        bolt::vec2 dragPosition = {};
        bool drag = false;
    };

    enum GamepadVector
    {
        GamepadVector_Left     = 0,
        GamepadVector_Right    = 1,
        GamepadVector_Triggers = 2,
    };

    #define INPUT_MAX_GAMEPADS 8
    struct GamepadState {
        SDL_Gamepad *gamepad = nullptr;
        float epsilon = 0;

        void init(SDL_Gamepad* pad, float eps)
        {
            gamepad = pad;
            epsilon = eps;
        }

        bool is_connected() const;
        bool has_axis(SDL_GamepadAxis axis) const;
        bool has_button(SDL_GamepadButton button) const;

        // SDL reports down as positive and up as negative on the y axis
        // which is different from what we want
        // raw versions just give the same number as SDL but others negate the y axis value from SDL
        s16 get_axis_raw(SDL_GamepadAxis axis) const;
        float get_axis_normalized(SDL_GamepadAxis axis, float range) const;
        float get_axis(SDL_GamepadAxis axis) const;

        ivec2 get_left_raw() const;
        ivec2 get_right_raw() const;
        vec2 get_left_normalized(float range) const;
        vec2 get_right_normalized(float range) const;
        vec2 get_left() const;
        vec2 get_right() const;

        bool get_button(SDL_GamepadButton button) const;

        float get_left_trigger() const;
        float get_right_trigger() const;
        vec2 get_triggers() const;

        // case by case of get_button
        bool south() const;
        bool north() const;
        bool west() const;
        bool east() const;
        bool dpad_up() const;
        bool dpad_down() const;
        bool dpad_left() const;
        bool dpad_right() const;
        bool left_shoulder() const;
        bool right_shoulder() const;
        bool right_stick() const;
        bool left_stick() const;
        bool start() const;
        bool back() const;
        bool guide() const;
    };

    enum InputKind
    {
        InputKindZero,    // invalid
        InputMouseButton,
        InputMouseWheel,
        InputMouseMotion,
        InputKeyboardKey,
        InputKeyboardAxis,
        InputKeyboardVector,
        InputGamepadButton,
        InputGamepadAxis,
        InputGamepadVector,
    };

    struct Input
    {
        InputKind kind = {};
        u32 device = 0;
        // what these means depends on input kind
        u32 key = 0;
        u32 key2 = 0;
        u32 key3 = 0;
        u32 key4 = 0;



        Input() {}
        Input(InputKind k) :
            kind(k)
        {}
        Input(SDL_MouseButtonFlags flags) :
            kind(InputMouseButton), key(flags)
        {}
        Input(SDL_Scancode scancode) :
            kind(InputKeyboardKey), key(scancode)
        {}
        Input(SDL_GamepadButton button) :
            kind(InputGamepadButton), key(button)
        {}
        Input(SDL_GamepadAxis axis) :
            kind(InputGamepadAxis), key(axis)
        {}
        Input(GamepadVector game_vector) :
            kind(InputGamepadVector), key(game_vector)
        {}
        Input(KeyboardAxis key_axis) :
            kind(InputKeyboardAxis),
            key(key_axis.negative), key2(key_axis.positive)
        {}
        Input(KeyboardVector key_vec) :
            kind(InputKeyboardVector),
            key(key_vec.x.negative), key2(key_vec.x.positive),
            key3(key_vec.y.negative), key4(key_vec.y.positive)
        {}
    };

    struct InputState {
        KeyboardState keyboard = {};
        MouseState mouse = {};

        GamepadState gamepads [INPUT_MAX_GAMEPADS] = {};
        int gamepad_count = 0;
        float gamepad_stick_epsilon = 0;
    };

    const char* get_gamepad_button_name(SDL_GamepadButton button);
    ValueType get_input_value_type(InputKind kind);

} // namespace

#endif // INPUT_HPP