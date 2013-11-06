varying lowp  vec4   OUT_LightPosition;
varying lowp  vec3   OUT_Normal;
varying highp vec2   OUT_TexCoord;
varying lowp  float  OUT_ClipPlane;

uniform sampler2D SAMPLER_01;
uniform sampler2D SAMPLER_02;

void main(void)
{
    //if(OUT_ClipPlane < 0.0)
    //    discard;

    lowp vec4 vDiffuseColor = texture2D(SAMPLER_01, OUT_TexCoord);
    lowp vec4 vAlbedo = vDiffuseColor * 0.25;
    lowp float fDiffuseFactor = max(dot(OUT_Normal, OUT_LightPosition.xyz), 0.0);
    lowp float fSelfShadow = clamp( 2.0 * OUT_LightPosition.z, 0.0, 1.0);
    vDiffuseColor.rgb = vDiffuseColor.rgb * fDiffuseFactor;
    gl_FragColor = vDiffuseColor * clamp(OUT_LightPosition.w, 0.0, 1.0) + vAlbedo;
}