
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
    
    //lowp vec4 vDiffuseColor = texture2D(SAMPLER_01, OUT_TexCoord);
    //lowp vec3 vNormalColor = normalize(texture2D(SAMPLER_02, OUT_TexCoord).rgb * 2.0 - 1.0);
    
    /*lowp float fSelfShadow = clamp( 3.0 * OUT_LightDirection.z, 0.0, 1.0);
     lowp float fDiffuseFactor = max(dot(vNormalColor, OUT_LightDirection), 0.0);
     vDiffuseColor.rgb = vDiffuseColor.rgb * fDiffuseFactor;*/
    
    highp vec3 blending = abs(OUT_Normal);
    blending = (blending - 0.2) * 7.0;
    blending = max(blending, 0.0); // Force weights to sum to 1.0
    highp float b = (blending.x + blending.y + blending.z);
    blending /= vec3(b, b, b);
    
    lowp vec4 xaxis = texture2D( SAMPLER_01, OUT_Position.yz / 8.0);
    lowp vec4 yaxis = texture2D( SAMPLER_01, OUT_Position.xz / 8.0);
    lowp vec4 zaxis = texture2D( SAMPLER_01, OUT_Position.xy / 8.0);
    // blend the results of the 3 planar projections.
    lowp vec4 tex = xaxis * blending.x + yaxis * blending.y + zaxis * blending.z;
    
    /*lowp vec4 color = vec4(0.0, 0.0, 0.0, 1.0);
    if(blending.x >= blending.y && blending.x >= blending.z)
    {
        color = texture2D( SAMPLER_01, OUT_Position.yz / 8.0);
    }
    else if(blending.y >= blending.x && blending.y >= blending.z)
    {
        color = texture2D( SAMPLER_01, OUT_Position.xz / 8.0);
    }
    else if(blending.z >= blending.x && blending.z >= blending.y)
    {
        color = texture2D( SAMPLER_01, OUT_Position.xy / 8.0);
    }*/
    
    gl_FragColor = tex;
}