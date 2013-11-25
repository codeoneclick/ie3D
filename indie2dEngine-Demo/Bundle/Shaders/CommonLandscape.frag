varying highp vec2   OUT_TexCoord;
uniform sampler2D SAMPLER_01;

void main(void)
{
    lowp vec4 vDiffuseColor = texture2D(SAMPLER_01, OUT_TexCoord);
    gl_FragColor = vDiffuseColor;
}