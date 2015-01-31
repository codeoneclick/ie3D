
#if defined(__OPENGL_30__)

in vec2 OUT_TexCoord;

#else

varying vec2 OUT_TexCoord;

#endif

uniform sampler2D  SAMPLER_01;
uniform sampler2D  SAMPLER_02;

void main(void)
{
    vec4 mainColor = texture2D(SAMPLER_01, OUT_TexCoord);
    vec4 overlappingColor = texture2D(SAMPLER_02, OUT_TexCoord);
    mainColor = mix(mainColor, overlappingColor, overlappingColor.a);
    gl_FragColor = mainColor;
}

