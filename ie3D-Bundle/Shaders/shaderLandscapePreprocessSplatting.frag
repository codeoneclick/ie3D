
#if defined(__OPENGL_30__)

in vec2   OUT_TexCoord;
in vec2   OUT_TexCoordScaled;

#else

varying vec2   OUT_TexCoord;
varying vec2   OUT_TexCoordScaled;

#endif

uniform sampler2D SAMPLER_01;
uniform sampler2D SAMPLER_02;
uniform sampler2D SAMPLER_03;
uniform sampler2D SAMPLER_04;

void main(void)
{
    lowp vec4 vSplattingColor = texture2D(SAMPLER_04, OUT_TexCoord);
    lowp vec4 vDiffuseColor = vec4(0.0, 0.0, 0.0, 1.0);
    
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
    
    gl_FragColor = clamp(vDiffuseColor, vec4(vec3(0.0), 1.0), vec4(1.0));
}
