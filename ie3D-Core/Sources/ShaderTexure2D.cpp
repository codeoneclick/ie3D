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
 attribute vec3 IN_Position;
 attribute vec2 IN_TexCoord;

 varying vec2   OUT_TexCoord;
 
 void main(void)
{
    OUT_TexCoord = IN_TexCoord / 32767.0  - 1.0;
    gl_Position = vec4(IN_Position, 1.0);
}
 );

const char* ShaderTexure2D_frag = STRING_SHADER
(
 varying vec2 OUT_TexCoord;
 uniform sampler2D  SAMPLER_01;
 
 void main()
{
	gl_FragColor = texture2D(SAMPLER_01, OUT_TexCoord);
}
 );
