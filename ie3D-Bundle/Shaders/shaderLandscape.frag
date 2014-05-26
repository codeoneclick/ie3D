
varying highp   vec2   OUT_TexCoord;
varying mediump float  OUT_ClipPlane;
varying mediump vec3   OUT_CameraPosition;
varying mediump vec3   OUT_LightDirection;
varying highp vec3   OUT_Position;
varying highp vec3   OUT_Normal;
varying highp float  OUT_Fog;

uniform sampler2D SAMPLER_01;
uniform sampler2D SAMPLER_02;
uniform sampler2D SAMPLER_03;
uniform sampler2D SAMPLER_04;

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
    
    bool isTriplanarFeatureEnabled = false;
    if(isTriplanarFeatureEnabled)
    {
        if(splattingMask.x > 0.0)
        {
            lowp vec4 xAxisColor = texture2D(SAMPLER_01, OUT_Position.yz / 16.0);
            lowp vec4 yAxisColor = texture2D(SAMPLER_01, OUT_Position.xz / 16.0);
            lowp vec4 zAxisColor = texture2D(SAMPLER_01, OUT_Position.xy / 16.0);
            diffuseColor = diffuseColor + (xAxisColor * blending.x + yAxisColor * blending.y + zAxisColor * blending.z) * splattingMask.x;
        }
        if(splattingMask.y > 0.0)
        {
            lowp vec4 xAxisColor = texture2D(SAMPLER_02, OUT_Position.yz / 8.0);
            lowp vec4 yAxisColor = texture2D(SAMPLER_02, OUT_Position.xz / 8.0);
            lowp vec4 zAxisColor = texture2D(SAMPLER_02, OUT_Position.xy / 8.0);
            diffuseColor = diffuseColor + (xAxisColor * blending.x + yAxisColor * blending.y + zAxisColor * blending.z) * splattingMask.y;
        }
        if(splattingMask.z > 0.0)
        {
            lowp vec4 xAxisColor = texture2D(SAMPLER_03, OUT_Position.yz / 8.0);
            lowp vec4 yAxisColor = texture2D(SAMPLER_03, OUT_Position.xz / 8.0);
            lowp vec4 zAxisColor = texture2D(SAMPLER_03, OUT_Position.xy / 8.0);
            diffuseColor = diffuseColor + (xAxisColor * blending.x + yAxisColor * blending.y + zAxisColor * blending.z) * splattingMask.z;
        }
    }
    else
    {
        if(splattingMask.x > 0.0)
        {
            diffuseColor = diffuseColor + texture2D(SAMPLER_01, OUT_TexCoord * 32.0) * splattingMask.x;
        }
        if(splattingMask.y > 0.0)
        {
            diffuseColor = diffuseColor + texture2D(SAMPLER_02, OUT_TexCoord * 64.0) * splattingMask.y;
        }
        if(splattingMask.z > 0.0)
        {
            diffuseColor = diffuseColor + texture2D(SAMPLER_03, OUT_TexCoord * 64.0) * splattingMask.z;
        }
    }
    diffuseColor = vec4(diffuseColor.rgb * diffuseFactor, 1.0);
    diffuseColor = mix(vec4(0.16, 0.32, 0.32, 1.0), diffuseColor, OUT_Fog);
    gl_FragColor = diffuseColor;
}