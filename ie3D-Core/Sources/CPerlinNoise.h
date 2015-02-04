//
//  CPerlinNoise.h
//  ie3D-Core
//
//  Created by Sergey Sergeev on 2/4/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#ifndef CPerlinNoise_h
#define CPerlinNoise_h

#include "HCommon.h"

class CPerlinNoise
{
private:
    
    f32 fade(f32 t) const;
    f32 lerp(f32 t, f32 a, f32 b) const;
    f32 grad(i32 hash, f32 x, f32 y, f32 z) const;
    i32 p[512];
    
protected:
    
public:
    
    CPerlinNoise(ui32 seed = 1);
    ~CPerlinNoise(void) = default;
    
    f32 noise(f32 x) const;
    f32 noise(f32 x, f32 y) const;
    f32 noise(f32 x, f32 y, f32 z) const;
    f32 octaveNoise(f32 x, i32 octaves) const;
    f32 octaveNoise(f32 x, f32 y, i32 octaves) const;
    f32 octaveNoise(f32 x, f32 y, f32 z, i32 octaves) const;
    
private:

};

#endif
