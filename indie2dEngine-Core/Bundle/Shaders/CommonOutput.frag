varying highp vec2 OUT_TexCoord;
uniform sampler2D SAMPLER_01;

void main(void)
{
    gl_FragColor = texture2D(SAMPLER_01, OUT_TexCoord);
}

