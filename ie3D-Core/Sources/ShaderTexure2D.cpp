//
//  ShaderTexure2D.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 8/8/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//
#include "HCommon.h"

const char* ShaderTexure2D_vert = STRING_SHADER
(
#if defined(__OPENGL_30__)
 
 out vec2   OUT_TexCoord;
 
#else
 
 varying vec2   OUT_TexCoord;
 
#endif
 
 void main(void)
{
    OUT_TexCoord = IN_TexCoord;
    gl_Position = vec4(IN_Position, 1.0);
}
 );

const char* ShaderTexure2D_frag = STRING_SHADER
(
#if defined(__OPENGL_30__)
 
 in vec2 OUT_TexCoord;
 
#else
 
 varying vec2 OUT_TexCoord;
 
#endif
 
 uniform sampler2D  SAMPLER_01;
 
 void main()
{
    gl_FragColor = texture2D(SAMPLER_01, OUT_TexCoord);
}
 );
