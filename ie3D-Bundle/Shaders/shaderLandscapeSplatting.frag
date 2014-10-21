varying  vec2 OUT_TexCoord;
varying vec2 OUT_TexCoordScaled;

uniform sampler2D SAMPLER_01;
uniform sampler2D SAMPLER_02;
uniform sampler2D SAMPLER_03;
uniform sampler2D SAMPLER_04;

void main(void)
{
    vec4 vSplattingColor = texture2D(SAMPLER_04, OUT_TexCoord);
    vec4 vDiffuseColor = vec4(0.0, 0.0, 0.0, 1.0);
    
    if(vSplattingColor.x > 0.0)
    {
        vDiffuseColor = vDiffuseColor + texture2D(SAMPLER_01, OUT_TexCoordScaled) * vSplattingColor.x;
    }
    if(vSplattingColor.y > 0.0)
    {
        vDiffuseColor = vDiffuseColor + texture2D(SAMPLER_02, OUT_TexCoordScaled) * vSplattingColor.y;
    }
    if(vSplattingColor.z > 0.0)
    {
        vDiffuseColor = vDiffuseColor + texture2D(SAMPLER_03, OUT_TexCoordScaled) * vSplattingColor.z;
    }
    
    gl_FragColor = vDiffuseColor;
}

