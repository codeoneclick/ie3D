
varying highp   vec2   OUT_TexCoord;
varying mediump float  OUT_ClipPlane;
varying mediump vec3   OUT_CameraPosition;
varying mediump vec3   OUT_LightDirection;
varying mediump vec3   OUT_Position;
varying mediump vec3   OUT_Normal;

uniform sampler2D SAMPLER_01;
uniform sampler2D SAMPLER_02;

void main(void)
{
    if(OUT_ClipPlane < 0.0)
        discard;
    
    lowp vec3 normalColor = normalize(texture2D(SAMPLER_02, OUT_TexCoord).rgb * 2.0 - 1.0);
    
    lowp float selfShadow = clamp(3.0 * OUT_LightDirection.z, 0.0, 1.0);
    lowp float diffuseFactor = max(dot(normalColor, OUT_LightDirection), 0.0);
    
    highp vec3 blending = abs(OUT_Normal);
    blending = blending - 0.5;
    blending = max(blending, 0.0);
    highp float b = (blending.x + blending.y + blending.z);
    blending /= vec3(b, b, b);
    
    lowp vec4 xAxisColor = texture2D(SAMPLER_01, OUT_Position.yz / 4.0);
    lowp vec4 yAxisColor = texture2D(SAMPLER_01, OUT_Position.xz / 4.0);
    lowp vec4 zAxisColor = texture2D(SAMPLER_01, OUT_Position.xy / 4.0);

    lowp vec4 diffuseColor = xAxisColor * blending.x + yAxisColor * blending.y + zAxisColor * blending.z;
    diffuseColor = texture2D(SAMPLER_01, OUT_TexCoord);
    diffuseColor.rgb = diffuseColor.rgb * diffuseFactor;
    
    gl_FragColor = diffuseColor;
}