#include "draw.hpp"

namespace bolt
{

	bool update_text(RenderContext* render, Text& text, String new_string, Font font)
	{
		if (string_compare(text.string, new_string))
		{
			return true;
		}

		text.string = new_string;
		return update_text_texture(*render, text.texture, new_string, font, text.color);
	}

} // namespace
