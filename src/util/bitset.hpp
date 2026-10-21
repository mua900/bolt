#ifndef BITSET_HPP
#define BITSET_HPP

#include "common.hpp"
#include "math_util.hpp"

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
            u8 result = bits[byte_index] >> offset;
            if (offset != 0 && byte_index + 1 < Size)
            {
                result |= bits[byte_index + 1] << (8 - offset);
            }
            return result;
        }

        u64 read_interval(int bit_index, int interval)
        {
            if (!in_bounds(bit_index) || !in_bounds(bit_index + interval - 1))
            {
                panic("Out of bounds access to bitset");
            }
            if (interval <= 0 || interval > 64)
            {
                panic("Bitset read_interval misuse, interval should be non zero and at most 64");
            }

            int byte_index = bit_index / 8;
            int offset = bit_index % 8;
            u64 result = 0;
            int num_bits = offset + interval;
            int bytes_to_read = bolt::ceil_of_ratio(num_bits, 8);
            ASSERT(byte_index + bytes_to_read <= Size);

            for (int i = 0; i < bytes_to_read; i++)
            {
                result |= ((u64)bits[byte_index + i]) << (i * 8);
            }

            result >>= offset;
            if (interval < 64) result &= BIT(interval) - 1;

            return result;
        }
    };

} // namespace

#endif // BITSET_HPP