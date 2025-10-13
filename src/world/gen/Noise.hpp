#pragma once
#include <cmath>
namespace Noise
{
    float lerp(float a0, float a1, float w)
    {
        return (1.0 - w) * a0 + w * a1;
    }
    float fade(float t)
    {
        return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
    }
    float noise(float x, float y, float z)
    {
        float n = x * 3.14 + y * 2137 + z * 79817;
        float nn = (sin(n) * 6664321);
        return nn - floor(nn);
    }
    float perlin(uint seed,float x,float y)
    {
        float x0 = floor(x);
        float x1 = x0 + 1;

        float y0 = floor(y);
        float y1 = y0 + 1;

        float sx = fade(x - x0);
        float sy = fade(y - y0);

        float n0, n1, ix0, ix1;
        n0 = noise(x0, y0, seed);
        n1 = noise(x1, y0, seed);
        ix0 = lerp(n0, n1, sx);

        n0 = noise(x0, y1, seed);
        n1 = noise(x1, y1, seed);
        ix1 = lerp(n0, n1, sx);
        return lerp(ix0, ix1, sy);
    }
} // namespace Noise
