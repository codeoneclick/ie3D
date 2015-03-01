#if defined(__IOS__)

#define GL_APPLE_clip_distance 1
#extension GL_APPLE_clip_distance : enable
precision highp float;

#endif

#if defined(__OPENGL_30__)

layout (location = 0) in vec3 IN_Position;
layout (location = 1) in vec2 IN_TexCoord;
layout (location = 2) in vec4 IN_Normal;
layout (location = 3) in vec4 IN_Tangent;
layout (location = 4) in vec4 IN_Color;
layout (location = 5) in vec4 IN_Extra;

#else

attribute vec3 IN_Position;
attribute vec2 IN_TexCoord;
attribute vec4 IN_Normal;
attribute vec4 IN_Tangent;
attribute vec4 IN_Color;
attribute vec4 IN_Extra;

#endif
