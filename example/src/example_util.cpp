#include <util/bitset.hpp>
#include <util/log.hpp>

int main()
{
    bolt::Bitset<> bitset = {};
    for (int i = 0; i < 7; i++)
    {
        bitset.set_bit(i, true);
    }

    bitset.bits[3] = 0x11;

    u8 byte = bitset.read_byte(0);
    u64 read = bitset.read_interval(25, 8);
    bolt::log_info("byte: %d, read: %x", byte, read);

    return 0;
}
