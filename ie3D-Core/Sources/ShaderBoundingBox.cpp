//
//  ShaderBoundingBox.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 11/10/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//
#include "HCommon.h"

const char* ShaderBoundingBox_vert = STRING_SHADER
(
#if defined(__OPENGL_30__)
 
#else
 
#endif
 
uniform mat4 u_matrixM;
uniform mat4 u_matrixV;
uniform mat4 u_matrixP;
 
 void main(void)
{
    vec4 vPosition = u_matrixM * vec4(IN_Position, 1.0);
    gl_Position = u_matrixP * u_matrixV * vPosition;
}
 );

const char* ShaderBoundingBox_frag = STRING_SHADER
(
#if defined(__OPENGL_30__)
 
#else
 
 
#endif
 
 void main()
{
    gl_FragColor = vec4(1.0);
}
 );
