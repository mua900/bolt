#ifndef ACTION_SET_HPP
#define ACTION_SET_HPP

#include "input.hpp"
#include <util/hash_table.hpp>
#include <util/value.hpp>

namespace bolt {

    // @todo
    // actions that look for events instead of reading the current state
    // maybe they take callbacks or something

    #define ACTION_MAX_INPUT 4

    // digital: MouseButton, KeyboardKey, GamepadButton
    // analog:  MouseMotion, MouseWheel, KeyboardAxis, KeyboardVector, GamepadAxis, GamepadVector

    enum AnalogInputBehavior
    {
        AnalogInputFirstWins = 0,
        AnalogInputIgnoreZero = 1,
        AnalogInputIgnoreThreshold = 2,
    };

    struct Action
    {
        Input input[ACTION_MAX_INPUT] = {};
        int num_input = 0;
        AnalogInputBehavior analog_behavior = {};
        float threshold = 0;

        bool add_input(Input& input);
        bool read_value(InputState& state, Value& out);

        bool read_analog_float(float v) const;
        bool read_analog_vector(vec2 v) const;
    };

    struct ActionSet
    {
        HashTable<Action> action_set = {};

        bool add_action(const char* name, Input& input);
        bool read_action(InputState& input, const char* name, Value& out);
    };

} // namespace

#endif // ACTION_SET_HPP