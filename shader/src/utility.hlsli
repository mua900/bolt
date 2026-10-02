#ifndef UTILITY_HLSLI

#define CONSTANT_HALF_PI          1.57079632679
#define CONSTANT_PI               3.14159265359
#define CONSTANT_ONE_AND_HALF_PI  4.71238898038
#define CONSTANT_E                2.71828182846
#define CONSTANT_TAU              6.28318530717

#define VK_LOCATION(index) [[vk::location(index)]]

float unpack_rotation(uint rot)
{
    float x = ((float) rot / float(0xFFFF)) * CONSTANT_TAU;
    return x;
}

#endif // UTILITY_HLSLI