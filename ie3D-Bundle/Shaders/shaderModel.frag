#if defined(__OPENGL_30__)

in vec2 OUT_TexCoord;

#else

varying vec2 OUT_TexCoord;

#endif

uniform sampler2D SAMPLER_01;

void main(void)
{
    gl_FragColor = texture2D(SAMPLER_01, OUT_TexCoord);
}