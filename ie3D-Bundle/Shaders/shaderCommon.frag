#if defined(__IOS__)

precision highp float;

#endif

#if defined(__OPENGL_30__)

layout (location = 0) out vec4 OUT_Attachment_01;

#define gl_FragColor OUT_Attachment_01
#define texture2D texture
#define textureCube texture

#endif
