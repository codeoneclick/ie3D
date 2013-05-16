const char* ShaderModelF = STRING_SHADER(
                                                    varying lowp     vec3   OUT_Light;
                                                    varying lowp     vec3   OUT_Normal;                                     
                                                    varying highp    vec2   OUT_TexCoord;
                                                    varying lowp     float  OUT_Clip;
                                                    uniform sampler2D EXT_TEXTURE_01;
void main(void)
{
    if(OUT_Clip < 0.0)
          discard;
          
    lowp vec4 vDiffuseColor = texture2D(EXT_TEXTURE_01, OUT_TexCoord);
    lowp float fDiffuseFactor = max(dot(OUT_Normal, OUT_Light), 0.0);
    lowp float fSelfShadow = clamp( 2.0 * OUT_Light.z, 0.0, 1.0);
    vDiffuseColor.rgb = vDiffuseColor.rgb * fDiffuseFactor * fSelfShadow + vDiffuseColor.rgb * 0.66;
    gl_FragColor = vDiffuseColor;
}
);
