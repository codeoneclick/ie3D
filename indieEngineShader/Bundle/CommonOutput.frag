varying highp vec2 OUT_TexCoord;
uniform sampler2D SAMPLER_01;

void main(void)
{
    gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0); //texture2D(SAMPLER_01, OUT_TexCoord);
}

