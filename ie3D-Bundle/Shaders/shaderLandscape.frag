
varying highp   vec2   OUT_TexCoord;
varying highp   vec4   OUT_TillingTexcoordLayer_01;
varying highp   vec4   OUT_TillingTexcoordLayer_02;
varying highp   vec4   OUT_TillingTexcoordLayer_03;
varying mediump float  OUT_ClipPlane;
varying mediump vec3   OUT_CameraPosition;
varying mediump vec3   OUT_LightDirection;
varying highp   vec3   OUT_Normal;
varying highp   float  OUT_Fog;

uniform sampler2D SAMPLER_01;
uniform sampler2D SAMPLER_02;
uniform sampler2D SAMPLER_03;
uniform sampler2D SAMPLER_04;

void main(void)
{
    if(OUT_ClipPlane < 0.0)
        discard;
    
    lowp float diffuseFactor = max(dot(OUT_Normal, OUT_LightDirection), 0.25);
    lowp vec4 diffuseColor = vec4(0.0, 0.0, 0.0, 1.0);
    
#if defined(__OSX__) || (defined(__IOS__) && defined(__IOS_HIGH_PERFORMANCE__))
    
    lowp vec4 splattingMask = texture2D(SAMPLER_04, OUT_TexCoord);
    
    highp vec3 blending = abs(OUT_Normal);
    blending = (blending - 0.2) * 7.0;
    blending = max(blending, 0.0);
    highp float b = (blending.x + blending.y + blending.z);
    blending /= vec3(b, b, b);
    
    lowp vec4 xAxisColor;
    lowp vec4 yAxisColor;
    lowp vec4 zAxisColor;
    
    if(splattingMask.x > 0.0)
    {
        xAxisColor = texture2D(SAMPLER_01, OUT_TillingTexcoordLayer_01.yz);
        yAxisColor = texture2D(SAMPLER_01, OUT_TillingTexcoordLayer_01.xz);
        zAxisColor = texture2D(SAMPLER_01, OUT_TillingTexcoordLayer_01.xy);
        diffuseColor = diffuseColor + (xAxisColor * blending.x + yAxisColor * blending.y + zAxisColor * blending.z) * splattingMask.x;
    }
    if(splattingMask.y > 0.0)
    {
        xAxisColor = texture2D(SAMPLER_02, OUT_TillingTexcoordLayer_02.yz);
        yAxisColor = texture2D(SAMPLER_02, OUT_TillingTexcoordLayer_02.xz);
        zAxisColor = texture2D(SAMPLER_02, OUT_TillingTexcoordLayer_02.xy);
        diffuseColor = diffuseColor + (xAxisColor * blending.x + yAxisColor * blending.y + zAxisColor * blending.z) * splattingMask.y;
    }
    if(splattingMask.z > 0.0)
    {
        xAxisColor = texture2D(SAMPLER_03, OUT_TillingTexcoordLayer_03.yz);
        yAxisColor = texture2D(SAMPLER_03, OUT_TillingTexcoordLayer_03.xz);
        zAxisColor = texture2D(SAMPLER_03, OUT_TillingTexcoordLayer_03.xy);
        diffuseColor = diffuseColor + (xAxisColor * blending.x + yAxisColor * blending.y + zAxisColor * blending.z) * splattingMask.z;
    }
#elif defined(__IOS__)
    diffuseColor = texture2D(SAMPLER_01, OUT_TexCoord);
#else
    lowp vec4 splattingMask = texture2D(SAMPLER_04, OUT_TexCoord);
    
    if(splattingMask.x > 0.0)
    {
        diffuseColor = diffuseColor + texture2D(SAMPLER_01, OUT_TillingTexcoordLayer_01.xy) * splattingMask.x;
    }
    if(splattingMask.y > 0.0)
    {
        diffuseColor = diffuseColor + texture2D(SAMPLER_02, OUT_TillingTexcoordLayer_02.xy) * splattingMask.y;
    }
    if(splattingMask.z > 0.0)
    {
        diffuseColor = diffuseColor + texture2D(SAMPLER_03, OUT_TillingTexcoordLayer_03.xy) * splattingMask.z;
    }
#endif
    diffuseColor = vec4(diffuseColor.rgb * min(diffuseFactor, 1.0), 1.0);
    diffuseColor = mix(vec4(vec3(0.16, 0.32, 0.32) * diffuseFactor, 1.0), diffuseColor, OUT_Fog);
    gl_FragColor = diffuseColor;
}