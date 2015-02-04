//
//  CPerlinNoise.cpp
//  ie3D-Core
//
//  Created by Sergey Sergeev on 2/4/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#include "CPerlinNoise.h"

CPerlinNoise::CPerlinNoise(ui32 seed)
{
    if(seed == 0)
    {
        seed = std::mt19937::default_seed;
    }

    std::iota(std::begin(p), std::begin(p) + 256, 0);
    std::shuffle(std::begin(p), std::begin(p) + 256, std::mt19937(seed));
    for(i32 i = 0; i < 256; ++i)
    {
        p[256 + i] = p[i];
    }
}

f32 CPerlinNoise::noise(f32 x) const
{
    return noise(x, 0.0f, 0.0f);
}

f32 CPerlinNoise::noise(f32 x, f32 y) const
{
    return noise(x, y, 0.0f);
}

f32 CPerlinNoise::noise(f32 x, f32 y, f32 z) const
{
    const i32 X = static_cast<i32>(::floor(x)) & 255;
    const i32 Y = static_cast<i32>(::floor(y)) & 255;
    const i32 Z = static_cast<i32>(::floor(z)) & 255;
    x -= ::floor(x);
    y -= ::floor(y);
    z -= ::floor(z);
    const f32 u = fade(x);
    const f32 v = fade(y);
    const f32 w = fade(z);
    const i32 A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z;
    const i32 B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;
    return lerp(w, lerp(v, lerp(u, grad(p[AA], x , y , z),
                                grad(p[BA], x - 1, y , z )),
                        lerp(u, grad(p[AB], x , y - 1, z),
                             grad(p[BB], x - 1, y - 1, z))),
                lerp(v, lerp(u, grad(p[AA + 1], x , y , z - 1),
                             grad(p[BA + 1], x - 1, y , z - 1)),
                     lerp(u, grad(p[AB + 1], x , y - 1, z - 1),
                          grad(p[BB + 1], x - 1, y - 1, z - 1 ))));
}

f32 CPerlinNoise::octaveNoise(f32 x, i32 octaves) const
{
    f32 result = 0.0;
    f32 amp = 1.0;
    for(i32 i = 0; i < octaves; ++i)
    {
        result += noise(x) * amp;
        x *= 2.0;
        amp *= 0.5;
    }
    return result;
}

f32 CPerlinNoise::octaveNoise(f32 x, f32 y, i32 octaves) const
{
    f32 result = 0.0;
    f32 amp = 1.0;
    for(i32 i = 0; i < octaves; ++i)
    {
        result += noise(x, y) * amp;
        x *= 2.0;
        y *= 2.0;
        amp *= 0.5;
    }
    return result;
}

f32 CPerlinNoise::octaveNoise(f32 x, f32 y, f32 z, i32 octaves) const
{
    f32 result = 0.0;
    f32 amp = 1.0;
    for(i32 i = 0; i < octaves; ++i)
    {
        result += noise(x, y, z) * amp;
        x *= 2.0;
        y *= 2.0;
        z *= 2.0;
        amp *= 0.5;
    }
    return result;
}

f32 CPerlinNoise::fade(f32 t) const
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}

f32 CPerlinNoise::lerp(f32 t, f32 a, f32 b) const
{
    return a + t * (b - a);
}

f32 CPerlinNoise::grad(i32 hash, f32 x, f32 y, f32 z) const
{
    const i32 h = hash & 15;
    const f32 u = h < 8 ? x : y, v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
}