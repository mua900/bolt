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
					out = Value(state.mouse.motion);
					return true;
				}
				case InputMouseWheel:
				{
					vec2 wheel = state.mouse.wheel;
					out = Value(wheel);
					return true;
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
				case InputKeyboardVector:
				{
					if (input[i].key >= state.keyboard.num_keys || input[i].key2 >= state.keyboard.num_keys)
					{
						log_error("Key value out of range");
						// bad input, ignore it
						// panic("Bad key value for input");
						return false;
					}

					bool x_pressed = state.keyboard.keys[input[i].key];
					bool y_pressed = state.keyboard.keys[input[i].key2];
					out = bolt::vec2(x_pressed ? 1 : 0, y_pressed ? 1 : 0);
					return true;
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
					float axis = pad.get_axis(gamepad_axis);
					out = Value(axis);
					return true;
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

					out = Value(v);

					return true;
				}
			}
		}

		return false;
	}

} // namespace
