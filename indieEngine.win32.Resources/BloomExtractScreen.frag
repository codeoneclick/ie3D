varying highp vec2 OUT_TexCoord;
uniform sampler2D SAMPLER_01;

void main(void)
{
    mediump float threshold = 0.2;
    mediump vec4 color = texture2D(SAMPLER_01, OUT_TexCoord);
    color = clamp((color - threshold) / (1.0 - threshold), 0.0, 1.0);
    gl_FragColor = color;
}

