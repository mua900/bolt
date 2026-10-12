#include "serialize.hpp"

#include "util/file_util.hpp"

namespace bolt {

    // @todo for later binary formats
    const u32 Magic = 0xDEFC;
    const u32 VersionNumber = 1;

    bool serialize_text(const char* outputName, SerializeState& state)
    {
        String_Builder builder(state.values.count * 16);

        // header
        builder.append_char('[');
        builder.append_integer(VersionNumber);
        builder.append_char(']');
        builder.append_char('\n');

        for (auto kv : state.values)
        {
            builder.append(kv.key);
            builder.append_char(' ');
            switch (kv.value->type)
            {
                case ValueType::INTEGER: {
                    builder.append_integer(kv.value->data.integer);
                    break;
                }
                case ValueType::REAL: {
                    builder.append_float(kv.value->data.real);
                    break;
                }
                default:
                    break;
            }
            builder.append_char('\n');
        }

        size_t written = write_to_file(outputName, builder, "w");

        return written == builder.cursor;
    }

    bool readback_text(const char* fileName, SerializeState* state)
    {
        BinaryData data;
        if (!load_file(fileName, data))
        {
            return false;
        }

        return readback_text_mem((const char*) data.data, data.size, state);
    }

    bool readback_text_mem(const char* data, size_t size, SerializeState* state)
    {
        String s = String(data, size);

        s.trim();

        int cursor = 0;
        String line = s;

        // comment
        while (line.size > 0 && line.data[0] == ';')
        {
            line = next_word(s, cursor, '\n');
            line = next_word(line, cursor, ' ');
        }

        String versionString = string_get_paren_content(s, cursor, '[', ']');
        versionString.trim();

        bool convertSuccess = false;
        int version = string_to_integer(versionString, &convertSuccess);
        if (!convertSuccess)
        {
            return false;
        }

        // if (version > VersionNumber)
        if (version != VersionNumber)
        {
            return false;
        }

        line = string_slice_to_character(s, cursor, '\n');
        cursor += line.size;
        line.trim();

        while (cursor < s.size)
        {
            line = next_word(s, cursor, '\n');
            line.trim();
            // comment
            ASSERT(line.size >= 0);
            if (line.size == 0 || line.data[0] == ';')
            {
                continue;
            }

            int lineCursor = 0;
            String a0 = next_word(line, lineCursor, ' ');
            // since we called trim() already, this is really just the rest of it
            String a1 = next_word(line, lineCursor, ' ');

            if (line.size != lineCursor)
            {
                return false;
            }

            a0.trim();
            a1.trim();

            if (!(a0.size > 0 && is_valid_ident_character(a0.data[0])))
            {
                return false;
            }

            bool convert = false;

            // @todo something more robust

            int integer = string_to_integer(a1, &convert);
            if (convert)
            {
                state->values.add(a0, Value(s64(integer)));
                continue;
            }
            double real = string_to_real(a1, &convert);
            if (convert)
            {
                state->values.add(a0, Value(real));
                continue;
            }

            // invalid value
            return false;
        }

        return true;
    }

} // namespace