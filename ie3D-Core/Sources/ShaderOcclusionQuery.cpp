//
//  ShaderOcclusionQuery.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 11/24/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//
#include "HCommon.h"

const char* ShaderOcclusionQuery_vert = STRING_SHADER
(
#if defined(__OPENGL_30__)
 
#else
 
#endif
 
 uniform mat4   MATRIX_Projection;
 uniform mat4   MATRIX_View;
 uniform mat4   MATRIX_World;
 
 void main(void)
{
    vec4 vPosition = MATRIX_World * vec4(IN_Position, 1.0);
    gl_Position = MATRIX_Projection * MATRIX_View * vPosition;
}
 );

const char* ShaderOcclusionQuery_frag = STRING_SHADER
(
#if defined(__OPENGL_30__)
 
#else
 
 
#endif
 
 void main()
{
    gl_FragColor = vec4(1.0);
}
 );
