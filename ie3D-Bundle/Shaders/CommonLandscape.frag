varying highp vec2   OUT_TexCoord;
varying lowp  float  OUT_ClipPlane;
uniform sampler2D SAMPLER_01;

void main(void)
{
    if(OUT_ClipPlane < 0.0)
        discard;
    
    lowp vec4 vDiffuseColor = texture2D(SAMPLER_01, OUT_TexCoord);
    gl_FragColor = vDiffuseColor;
}