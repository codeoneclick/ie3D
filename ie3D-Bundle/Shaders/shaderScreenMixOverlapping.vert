
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
