
varying highp   vec2   OUT_TexCoord;
varying mediump float  OUT_ClipPlane;

uniform sampler2D SAMPLER_01;
uniform sampler2D SAMPLER_02;

void main(void)
{
    if(OUT_ClipPlane < 0.0)
        discard;
    
    lowp vec4 vDiffuseColor = texture2D(SAMPLER_01, vec2(OUT_TexCoord.x * 64.0, OUT_TexCoord.y * 16.0));
    lowp vec4 vMaskColor = texture2D(SAMPLER_02, OUT_TexCoord);
    vDiffuseColor *= vMaskColor.r;
    gl_FragColor = vDiffuseColor;
}