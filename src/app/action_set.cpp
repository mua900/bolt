#include "action_set.hpp"

namespace bolt {

	bool ActionSet::add_action(const char* name, Input& input)
	{
		String s = String(name);
		Action* action = action_set.find(s);
		bool added = false;
		if (!action)
		{
			Action naction = {};
			added = naction.add_input(input);
			if (added) action_set.add(s, naction);
		}
		else
		{
			added = action->add_input(input);
		}
		return added;
	}

	bool ActionSet::read_action(InputState& state, const char* name, Value& out)
	{
		String s = String(name);
		Action* action = action_set.find(s);
		if (!action)
		{
			return false;
		}

		return action->read_value(state, out);
	}

	bool Action::add_input(Input& in)
	{
		if (num_input < ACTION_MAX_INPUT)
		{
			input[num_input] = in;
			num_input += 1;
			return true;
		}
		else return false;
	}

	bool Action::read_value(InputState& state, Value& out)
	{
		// currently the if no satisfactory input is found between alternatives, the last one is chosen
		for (int i = 0; i < num_input; i++)
		{
			bool last = (i == num_input - 1);

			switch (input[i].kind)
			{
				case InputMouseButton:
				{
					SDL_MouseButtonFlags flags = (SDL_MouseButtonFlags)input[i].key;
					bool down = flags & state.mouse.buttonFlags;
					if (down)
					{
						out = Value(true);
						return true;
					}
					else if (last)
					{
						out = Value(false);
						return true;
					}
					else break;
				}
				case InputMouseMotion:
				{
					vec2 val = state.mouse.motion;
					if (last || read_analog_vector(val))
					{
						out = Value(val);
						return true;
					}
					else break;
				}
				case InputMouseWheel:
				{
					vec2 val = state.mouse.wheel;
					if (last || read_analog_vector(val))
					{
						out = Value(val);
						return true;
					}
					else break;
				}
				case InputKeyboardKey:
				{
					if (input[i].key >= state.keyboard.num_keys)
					{
						log_error("Key value out of range");
						// bad input, ignore it
						// panic("Bad key value for input");
						return false;
					}

					bool down = state.keyboard.keys[input[i].key];
					if (down)
					{
						out = Value(true);
						return true;
					}
					else if (last)
					{
						out = Value(false);
						return true;
					}
					else break;
				}
				case InputKeyboardAxis:
				{
					if (input[i].key >= state.keyboard.num_keys ||
						input[i].key2 >= state.keyboard.num_keys)
					{
						log_error("Key value out of range");
						// bad input, ignore it
						// panic("Bad key value for input");
						return false;
					}

					int negative = state.keyboard.keys[input[i].key];
					int positive = state.keyboard.keys[input[i].key2];
					float val = (float) (positive - negative);
					if (last || read_analog_float(val))
					{
						out = Value(val);
						return true;
					}
					else break;
				}
				case InputKeyboardVector:
				{
					if (input[i].key >= state.keyboard.num_keys  ||
						input[i].key2 >= state.keyboard.num_keys ||
						input[i].key3 >= state.keyboard.num_keys ||
						input[i].key4 >= state.keyboard.num_keys)
					{
						log_error("Key value out of range");
						// bad input, ignore it
						// panic("Bad key value for input");
						return false;
					}

					int x_negative = state.keyboard.keys[input[i].key];
					int x_positive = state.keyboard.keys[input[i].key2];
					int y_negative = state.keyboard.keys[input[i].key3];
					int y_positive = state.keyboard.keys[input[i].key4];
					vec2 val = vec2((float)(x_positive - x_negative), (float)(y_positive - y_negative));
					if (last || read_analog_vector(val))
					{
						out = Value(val);
						return true;
					}
					else break;
				}
				case InputGamepadButton:
				{
					GamepadState& pad = state.gamepads[input[i].device];
					if (!pad.is_connected())
					{
						break;
					}
					auto button = (SDL_GamepadButton)input[i].key;
					bool down = pad.get_button(button);
					if (down)
					{
						out = Value(true);
						return true;
					}
					else if (last)
					{
						out = Value(false);
						return true;
					}
					else break;
				}
				case InputGamepadAxis:
				{
					GamepadState& pad = state.gamepads[input[i].device];
					if (!pad.is_connected())
					{
						break;
					}
					auto gamepad_axis = (SDL_GamepadAxis) input[i].key;
					float val = pad.get_axis(gamepad_axis);
					if (last || read_analog_float(val))
					{
						out = Value(val);
						return true;
					}
					else break;
				}
				case InputGamepadVector:
				{
					GamepadState& pad = state.gamepads[input[i].device];
					if (!pad.is_connected())
					{
						break;
					}
					vec2 v = {};
					if (input[i].key == GamepadVector_Left)
					{
						v = pad.get_left();
					}
					else if (input[i].key == GamepadVector_Right)
					{
						v = pad.get_right();
					}
					else if (input[i].key == GamepadVector_Triggers)
					{
						v = pad.get_triggers();
					}
					else return false;

					if (last || read_analog_vector(v))
					{
						out = Value(v);
						return true;
					}
					else break;
				}
			}
		}

		return false;
	}

	bool Action::read_analog_float(float v) const
	{
		switch (analog_behavior)
		{
			case AnalogInputFirstWins:		return true;
			case AnalogInputIgnoreZero:
			{
				if (v == 0) break;
				return true;
			}
			case AnalogInputIgnoreThreshold:
			{
				if (std::fabsf(v) < threshold) break;
				return true;
			}
		}

		return false;
	}

	bool Action::read_analog_vector(vec2 v) const
	{
		switch (analog_behavior)
		{
			case AnalogInputFirstWins:		return true;
			case AnalogInputIgnoreZero:
				if (v.magnitude() == 0) break;
				return true;
			case AnalogInputIgnoreThreshold:
				if (v.magnitude() < threshold) break;
				return true;
		}

		return false;
	}

} // namespace
