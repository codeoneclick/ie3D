
varying highp   vec2   OUT_TexCoord;
varying mediump float  OUT_ClipPlane;
varying mediump vec3   OUT_CameraPosition;
varying mediump vec3   OUT_LightDirection;
varying highp   vec3   OUT_Position;
varying highp   vec3   OUT_Normal;
varying highp   float  OUT_Fog;

uniform sampler2D SAMPLER_01;
uniform sampler2D SAMPLER_02;
uniform sampler2D SAMPLER_03;
uniform sampler2D SAMPLER_04;

uniform highp float IN_HeightmapSizeX;
uniform highp float IN_HeightmapSizeZ;

uniform highp float IN_SplattingTillingLayer_01;
uniform highp float IN_SplattingTillingLayer_02;
uniform highp float IN_SplattingTillingLayer_03;

void main(void)
{
    if(OUT_ClipPlane < 0.0)
        discard;
    
    lowp float diffuseFactor = max(dot(OUT_Normal, OUT_LightDirection), 0.0);
    
    highp vec3 blending = abs(OUT_Normal);
    blending = (blending - 0.2) * 7.0;
    blending = max(blending, 0.0);
    highp float b = (blending.x + blending.y + blending.z);
    blending /= vec3(b, b, b);
    
    lowp vec4 diffuseColor = vec4(0.0, 0.0, 0.0, 1.0);
    lowp vec4 splattingMask = texture2D(SAMPLER_04, OUT_TexCoord);
    
    bool isTriplanarFeatureEnabled = true;
    if(isTriplanarFeatureEnabled)
    {
        if(splattingMask.x > 0.0)
        {
            lowp vec4 xAxisColor = texture2D(SAMPLER_01, OUT_Position.yz / IN_SplattingTillingLayer_01);
            lowp vec4 yAxisColor = texture2D(SAMPLER_01, OUT_Position.xz / IN_SplattingTillingLayer_01);
            lowp vec4 zAxisColor = texture2D(SAMPLER_01, OUT_Position.xy / IN_SplattingTillingLayer_01);
            diffuseColor = diffuseColor + (xAxisColor * blending.x + yAxisColor * blending.y + zAxisColor * blending.z) * splattingMask.x;
        }
        if(splattingMask.y > 0.0)
        {
            lowp vec4 xAxisColor = texture2D(SAMPLER_02, OUT_Position.yz / IN_SplattingTillingLayer_02);
            lowp vec4 yAxisColor = texture2D(SAMPLER_02, OUT_Position.xz / IN_SplattingTillingLayer_02);
            lowp vec4 zAxisColor = texture2D(SAMPLER_02, OUT_Position.xy / IN_SplattingTillingLayer_02);
            diffuseColor = diffuseColor + (xAxisColor * blending.x + yAxisColor * blending.y + zAxisColor * blending.z) * splattingMask.y;
        }
        if(splattingMask.z > 0.0)
        {
            lowp vec4 xAxisColor = texture2D(SAMPLER_03, OUT_Position.yz / IN_SplattingTillingLayer_03);
            lowp vec4 yAxisColor = texture2D(SAMPLER_03, OUT_Position.xz / IN_SplattingTillingLayer_03);
            lowp vec4 zAxisColor = texture2D(SAMPLER_03, OUT_Position.xy / IN_SplattingTillingLayer_03);
            diffuseColor = diffuseColor + (xAxisColor * blending.x + yAxisColor * blending.y + zAxisColor * blending.z) * splattingMask.z;
        }
    }
    else
    {
        if(splattingMask.x > 0.0)
        {
            diffuseColor = diffuseColor + texture2D(SAMPLER_01, OUT_TexCoord * (vec2(IN_HeightmapSizeX, IN_HeightmapSizeZ) / IN_SplattingTillingLayer_01)) * splattingMask.x;
        }
        if(splattingMask.y > 0.0)
        {
            diffuseColor = diffuseColor + texture2D(SAMPLER_02, OUT_TexCoord * (vec2(IN_HeightmapSizeX, IN_HeightmapSizeZ) /IN_SplattingTillingLayer_02)) * splattingMask.y;
        }
        if(splattingMask.z > 0.0)
        {
            diffuseColor = diffuseColor + texture2D(SAMPLER_03, OUT_TexCoord * (vec2(IN_HeightmapSizeX, IN_HeightmapSizeZ) /IN_SplattingTillingLayer_03)) * splattingMask.z;
        }
    }
    diffuseColor = vec4(diffuseColor.rgb * diffuseFactor, 1.0);
    diffuseColor = mix(vec4(0.16, 0.32, 0.32, 1.0), diffuseColor, OUT_Fog);
    gl_FragColor = diffuseColor;
}