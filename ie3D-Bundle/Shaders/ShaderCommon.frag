
varying lowp  vec3   OUT_LightPosition;
varying lowp  vec3   OUT_Normal;
varying highp vec2   OUT_TexCoord;
varying lowp  float  OUT_ClipPlane;

uniform sampler2D SAMPLER_01;

void main(void)
{
    if(OUT_ClipPlane < 0.0)
        discard;

    lowp vec4 vDiffuseColor = texture2D(SAMPLER_01, OUT_TexCoord);
    lowp float fDiffuseFactor = max(dot(OUT_Normal, OUT_LightPosition), 0.0);
    lowp float fSelfShadow = clamp( 2.0 * OUT_LightPosition.z, 0.0, 1.0);
    vDiffuseColor.rgb = vDiffuseColor.rgb * fDiffuseFactor * fSelfShadow + vDiffuseColor.rgb * 0.66;
    gl_FragColor = vDiffuseColor;
}