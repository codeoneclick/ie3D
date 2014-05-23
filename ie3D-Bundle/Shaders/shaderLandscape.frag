
varying highp   vec2   OUT_TexCoord;
varying mediump float  OUT_ClipPlane;
varying mediump vec3   OUT_CameraPosition;
varying mediump vec3   OUT_LightDirection;
varying highp vec3   OUT_Position;
varying highp vec3   OUT_Normal;

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
    
    lowp vec4 xAxisColor = texture2D(SAMPLER_01, OUT_Position.yz / 16.0);
    lowp vec4 yAxisColor = texture2D(SAMPLER_01, OUT_Position.xz / 16.0);
    lowp vec4 zAxisColor = texture2D(SAMPLER_01, OUT_Position.xy / 16.0);
    lowp vec4 diffuseColor_01 = xAxisColor * blending.x + yAxisColor * blending.y + zAxisColor * blending.z;
    
    xAxisColor = texture2D(SAMPLER_02, OUT_Position.yz / 8.0);
    yAxisColor = texture2D(SAMPLER_02, OUT_Position.xz / 8.0);
    zAxisColor = texture2D(SAMPLER_02, OUT_Position.xy / 8.0);
    lowp vec4 diffuseColor_02 = xAxisColor * blending.x + yAxisColor * blending.y + zAxisColor * blending.z;
    
    xAxisColor = texture2D(SAMPLER_03, OUT_Position.yz / 8.0);
    yAxisColor = texture2D(SAMPLER_03, OUT_Position.xz / 8.0);
    zAxisColor = texture2D(SAMPLER_03, OUT_Position.xy / 8.0);
    lowp vec4 diffuseColor_03 = xAxisColor * blending.x + yAxisColor * blending.y + zAxisColor * blending.z;
    
    lowp vec4 splattingMask = texture2D(SAMPLER_04, OUT_TexCoord);
    
    lowp vec4 diffuseColor = diffuseColor_01 * splattingMask.r + diffuseColor_02 * splattingMask.g + diffuseColor_03 * splattingMask.b;
    diffuseColor = vec4(diffuseColor.rgb * diffuseFactor, 1.0);
    
    gl_FragColor = diffuseColor;
}