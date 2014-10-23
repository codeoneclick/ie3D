#if defined(__OPENGL_30__)

in vec2   OUT_TexCoord;
in float  OUT_ClipPlane;

#else

varying vec2   OUT_TexCoord;
varying float  OUT_ClipPlane;

#endif

uniform sampler2D SAMPLER_01;
uniform sampler2D SAMPLER_02;

void main(void)
{
    if(OUT_ClipPlane < 0.0)
        discard;
    
    vec4 vDiffuseColor = texture2D(SAMPLER_01, vec2(OUT_TexCoord.x * 64.0, OUT_TexCoord.y * 16.0));
    
    vec4 vMaskColor = texture2D(SAMPLER_02, OUT_TexCoord);
    
    vDiffuseColor *= vMaskColor.r;
    
    gl_FragColor = vDiffuseColor;
}