
#if defined(__OPENGL_30__)

in vec3 OUT_Normal;

#else

varying vec3 OUT_Normal;

#endif

void main(void)
{
    gl_FragColor = vec4(0.5 * OUT_Normal + vec3(0.5), 1.0);
}