#if defined(__OPENGL_30__)

in vec4 OUT_Color;

#else

varying vec4 OUT_Color;

#endif

void main(void)
{
    gl_FragColor = OUT_Color;
}