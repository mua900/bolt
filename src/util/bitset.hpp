#ifndef BITSET_HPP
#define BITSET_HPP

#include "common.hpp"

namespace bolt
{

    template<int Size = 8>
    struct Bitset
    {
        u8 bits[Size];

        bool in_bounds(int index)
        {
            return (index >= 0) && (index < Size * 8);
        }

        void set_bit(int index, bool value)
        {
            if (!in_bounds(index))
            {
                panic("Out of bounds access to bitset");
            }

            int mod = index % 8;
            if (value)
            {
                bits[index / 8] |= BIT(mod);
            }
            else
            {
                bits[index / 8] &= ~BIT(mod);
            }
        }

        bool read_bit(int index)
        {
            if (!in_bounds(index))
            {
                panic("Out of bounds access to bitset");
            }

            return bool(bits[(index / 8)] & BIT(index % 8));
        }

        u8 read_byte(int bit_index)
        {
            if (!in_bounds(bit_index))
            {
                panic("Out of bounds access to bitset");
            }

            int byte_index = bit_index / 8;
            int offset = bit_index % 8;
            u8 result = bits[byte] >> offset;
            if (offset != 0 && byte_index + 1 < Size)
            {
                result |= bits[byte_index + 1] << (8 - offset);
            }
            return result;
        }

        u32 read_interval(int bit_index, int interval)
        {
            if (interval > 32)
            {
                panic("Bitset read_interval misuse, maximum interval is 32 bits");
            }

            int byte_index = bit_index / 8;
            int offset = bit_index % 8;
            u32 result = read_byte(bit_index);
            int diff = Size - byte_index;
            ASSERT(diff != 0 && diff > 0);
            switch (diff)
            {
                case 1: break;
                case 2: {
                    result |= read_byte(byte_index + 1) << 8;
                    break;
                }
                case 3: {
                    result |= read_byte(byte_index + 1) << 8;
                    result |= read_byte(byte_index + 2) << 16;
                    break;
                }
                case 4:  // fallthrough
                default: {
                    result |= read_byte(byte_index + 1) << 8;
                    result |= read_byte(byte_index + 2) << 16;
                    result |= read_byte(byte_index + 3) << 24;
                    break;
                }
            }

            return result;
        }
    };

} // namespace

#endif // BITSET_HPP