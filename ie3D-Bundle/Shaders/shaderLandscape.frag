
varying highp   vec2   OUT_TexCoord;
varying highp   vec4   OUT_SplattingTexCoord;
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
    
    lowp float diffuseFactor = max(dot(OUT_Normal, OUT_LightDirection), 0.0);
    lowp vec4 diffuseColor = vec4(0.0, 0.0, 0.0, 1.0);
    
#if defined(DETAIL_LEVEL_2)
    
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
        xAxisColor = texture2D(SAMPLER_01, OUT_SplattingTexCoord.yz);
        yAxisColor = texture2D(SAMPLER_01, OUT_SplattingTexCoord.xz);
        zAxisColor = texture2D(SAMPLER_01, OUT_SplattingTexCoord.xy);
        diffuseColor = diffuseColor + (xAxisColor * blending.x + yAxisColor * blending.y + zAxisColor * blending.z) * splattingMask.x;
    }
    if(splattingMask.y > 0.0)
    {
        xAxisColor = texture2D(SAMPLER_02, OUT_SplattingTexCoord.yz);
        yAxisColor = texture2D(SAMPLER_02, OUT_SplattingTexCoord.xz);
        zAxisColor = texture2D(SAMPLER_02, OUT_SplattingTexCoord.xy);
        diffuseColor = diffuseColor + (xAxisColor * blending.x + yAxisColor * blending.y + zAxisColor * blending.z) * splattingMask.y;
    }
    if(splattingMask.z > 0.0)
    {
        xAxisColor = texture2D(SAMPLER_03, OUT_SplattingTexCoord.yz);
        yAxisColor = texture2D(SAMPLER_03, OUT_SplattingTexCoord.xz);
        zAxisColor = texture2D(SAMPLER_03, OUT_SplattingTexCoord.xy);
        diffuseColor = diffuseColor + (xAxisColor * blending.x + yAxisColor * blending.y + zAxisColor * blending.z) * splattingMask.z;
    }
#elif defined(DETAIL_LEVEL_1)
    lowp vec4 splattingMask = texture2D(SAMPLER_04, OUT_TexCoord);
    
    if(splattingMask.x > 0.0)
    {
        diffuseColor = diffuseColor + texture2D(SAMPLER_01, OUT_SplattingTexCoord.xy) * splattingMask.x;
    }
    if(splattingMask.y > 0.0)
    {
        diffuseColor = diffuseColor + texture2D(SAMPLER_02, OUT_SplattingTexCoord.xy) * splattingMask.y;
    }
    if(splattingMask.z > 0.0)
    {
        diffuseColor = diffuseColor + texture2D(SAMPLER_03, OUT_SplattingTexCoord.xy) * splattingMask.z;
    }
#else
    diffuseColor = texture2D(SAMPLER_01, OUT_TexCoord);
#endif
    diffuseColor = vec4(diffuseColor.rgb * diffuseFactor, 1.0);
    diffuseColor = mix(vec4(0.16, 0.32, 0.32, 1.0), diffuseColor, OUT_Fog);
    gl_FragColor = diffuseColor;
}