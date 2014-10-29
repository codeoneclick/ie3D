#if defined(__OPENGL_30__)

#else

#endif

uniform vec4 IN_color;

void main(void)
{
    gl_FragColor = IN_color;
}