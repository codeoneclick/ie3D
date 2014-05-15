
varying highp   vec2   OUT_TexCoord;
varying mediump float  OUT_ClipPlane;
varying mediump vec3   OUT_CameraPosition;
varying mediump vec3   OUT_LightDirection;
varying mediump vec3   OUT_Position;
varying mediump vec3   OUT_Tangent;
varying mediump vec3   OUT_Normal;
varying mediump vec3   OUT_Binormal;

uniform sampler2D SAMPLER_01;
uniform sampler2D SAMPLER_02;

void main(void)
{
    if(OUT_ClipPlane < 0.0)
        discard;
    
    lowp vec4 vDiffuseColor = texture2D(SAMPLER_01, OUT_TexCoord);
    lowp vec3 vNormalColor = normalize(texture2D(SAMPLER_02, OUT_TexCoord).rgb * 2.0 - 1.0);
    
    lowp float fSelfShadow = clamp( 3.0 * OUT_LightDirection.z, 0.0, 1.0);
    lowp float fDiffuseFactor = max(dot(vNormalColor, OUT_LightDirection), 0.0);
    vDiffuseColor.rgb = vDiffuseColor.rgb * fDiffuseFactor;

    gl_FragColor = vDiffuseColor;
}