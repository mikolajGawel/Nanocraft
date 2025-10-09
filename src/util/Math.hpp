#pragma once
namespace Math {
    float slowdown(float brakePerSecond, float current);
    float accelerate(float accelerationPerSecond, float destination, float current);
    float clamp(float value, float min, float max);
    float smoothstep(float value, float min, float max);
}