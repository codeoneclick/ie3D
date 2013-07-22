varying highp vec2 OUT_TexCoord;
uniform sampler2D SAMPLER_01;
uniform sampler2D SAMPLER_02;

void main(void)
{
    mediump vec3 vLightPosition = vec3(32.0, 4.0, 32.0);
    mediump vec3 vVertexPosition = texture2D(SAMPLER_01, OUT_TexCoord).xyz;
    mediump vec3 vVertexNormal = texture2D(SAMPLER_02, OUT_TexCoord).xyz;
    vVertexNormal = normalize(vVertexNormal * 2.0 - 1.0);
    mediump vec3 vLight = vec3(0.0, 1.0, 0.0);//normalize(vLightPosition - vVertexPosition);
    
    lowp float fDiffuseFactor = max(dot(vVertexNormal, vLight), 0.0);
    //lowp float fSelfShadow = clamp( 2.0 * vLight.z, 0.0, 1.0);
    //lowp float vW = 1.0 - dot(vLight / 8.0, vLight / 8.0);
    lowp vec4 vDiffuseColor = vec4(1.0, 0.0, 0.0, 1.0) * fDiffuseFactor;// * fSelfShadow;
    gl_FragColor = vDiffuseColor;
}

