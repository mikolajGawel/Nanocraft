#pragma once
namespace Math {
    float slowdown(float brake, float current);
    float accelerate(float acceleration, float destination, float current);
    float clamp(float value, float min, float max);
    float smoothstep(float value, float min, float max);
}