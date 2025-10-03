#include "Math.hpp" 
#include <cmath>
#include <iostream>

float Math::slowdown(float brake, float current) {
        if (brake < 0.0)
            std::cerr << "Slowing speed can't be smaller than zero\n";
        brake = static_cast<float>(abs(brake));
        brake = current > 0 ? -brake : brake;
        if (current > 0 ? (current - brake < 0.0) : (current + brake > 0.0))
            return 0.0f;

        return current + brake;
    }

    float Math::accelerate(float acceleration, float destination, float current) {
        if (acceleration <= 0.0)
            std::cerr << "Acceleration speed can't be smaller or equal zero\n";

        acceleration = static_cast<float>(abs(acceleration));
        if ((destination > 0 && current < 0) || (destination < 0 && current > 0))
            current = slowdown(acceleration * 2, current);

        acceleration = current < destination ? acceleration : -acceleration;
        if (destination > 0 ? (current + acceleration > destination) : (current - acceleration < destination))
            return destination;

        return current + acceleration;
    }

    float Math::clamp(float value, float min, float max) {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }

    float Math::smoothstep(float value, float min, float max) {
        value = Math::clamp((value - min) / (max - min), 0, 1);
        return value * value * (3.0f - 2.0f * value);
    }