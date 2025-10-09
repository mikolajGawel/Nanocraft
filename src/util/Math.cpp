#include "Math.hpp"
#include <cmath>
#include <iostream>
#include "Time.hpp"
float Math::slowdown(float brakePerSecond, float current)
{
    if (brakePerSecond < 0.0)
        std::cerr << "Slowing speed can't be smaller than zero\n";
    float brake = static_cast<float>(abs(brakePerSecond)) * Time::DELTA_TIME;
    brake = current > 0 ? -brake : brake;
    if (current > 0 ? (current - brake < 0.0) : (current + brake > 0.0))
        return 0.0f;

    return current + brake;
}

float Math::accelerate(float accelerationPerSecond, float destination, float current)
{
    if (accelerationPerSecond <= 0.0)
        std::cerr << "Acceleration speed can't be smaller or equal zero\n";

    float acceleration = static_cast<float>(abs(accelerationPerSecond)) * Time::DELTA_TIME;
    if ((destination > 0 && current < 0) || (destination < 0 && current > 0))
        current = slowdown(acceleration * 2, current);

    acceleration = current < destination ? acceleration : -acceleration;
    if (destination > 0 ? (current + acceleration > destination) : (current - acceleration < destination))
        return destination;

    return current + acceleration;
}

float Math::clamp(float value, float min, float max)
{
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}

float Math::smoothstep(float value, float min, float max)
{
    value = Math::clamp((value - min) / (max - min), 0, 1);
    return value * value * (3.0f - 2.0f * value);
}