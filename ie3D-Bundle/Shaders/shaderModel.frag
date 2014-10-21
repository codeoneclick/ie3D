varying vec2   OUT_TexCoord;
varying float  OUT_ClipPlane;

uniform sampler2D SAMPLER_01;

void main(void)
{
    if(OUT_ClipPlane < 0.0)
        discard;

    gl_FragColor = texture2D(SAMPLER_01, OUT_TexCoord);
}