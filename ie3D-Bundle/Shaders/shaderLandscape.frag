
#if defined(__OPENGL_30__)

in vec2   OUT_TexCoord;
in vec4   OUT_TillingTexcoordLayer_01;
in vec4   OUT_TillingTexcoordLayer_02;
in vec4   OUT_TillingTexcoordLayer_03;
in float  OUT_ClipPlane;
in vec3   OUT_CameraPosition;
in vec3   OUT_LightDirection;
in vec3   OUT_Normal;
in float  OUT_Fog;
in vec4   OUT_ShadowParameters;

#else

varying vec2   OUT_TexCoord;
varying vec4   OUT_TillingTexcoordLayer_01;
varying vec4   OUT_TillingTexcoordLayer_02;
varying vec4   OUT_TillingTexcoordLayer_03;
varying float  OUT_ClipPlane;
varying vec3   OUT_CameraPosition;
varying vec3   OUT_LightDirection;
varying vec3   OUT_Normal;
varying float  OUT_Fog;
varying vec4   OUT_ShadowParameters;

#endif

uniform sampler2D SAMPLER_01;
uniform sampler2D SAMPLER_02;
uniform sampler2D SAMPLER_03;
uniform sampler2D SAMPLER_04;
uniform sampler2D SAMPLER_05;

const  vec2 vCameraRange = vec2(0.01, 1024.0);

float getShadowMapPassDepth(in vec2 vTexCoord)
{
    float fNearZ = vCameraRange.x;
    float fFarZ = vCameraRange.y;
    float fDepth = texture2D(SAMPLER_05, vTexCoord).x;
    return (2.0 * fNearZ) / (fNearZ + fFarZ - fDepth * (fFarZ - fNearZ));
}

float getCurrentDepth(in  float fZ)
{
    float fDepth = fZ;
    float fNearZ = vCameraRange.x;
    float fFarZ = vCameraRange.y;
    fDepth = (2.0 * fNearZ) / (fNearZ + fFarZ - fDepth * (fFarZ - fNearZ));
    return fDepth;
}

void main(void)
{
    float diffuseFactor = max(dot(OUT_Normal, OUT_LightDirection), 0.25);
    vec4 diffuseColor = vec4(0.0, 0.0, 0.0, 1.0);
    
/*#if defined(__OSX__) || (defined(__IOS__) && defined(__IOS_HIGH_PERFORMANCE__))
    
    vec4 splattingMask = texture2D(SAMPLER_04, OUT_TexCoord);
    
    vec3 blending = abs(OUT_Normal);
    blending = (blending - 0.2) * 7.0;
    blending = max(blending, 0.0);
    float b = (blending.x + blending.y + blending.z);
    blending /= vec3(b, b, b);
    
    vec4 xAxisColor;
    vec4 yAxisColor;
    vec4 zAxisColor;
    
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
#elif defined(__IOS__)*/
    diffuseColor = texture2D(SAMPLER_01, OUT_TexCoord);
/*#else
    vec4 splattingMask = texture2D(SAMPLER_04, OUT_TexCoord);
    
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
#endif*/
    
    vec2 vTexCoord = OUT_ShadowParameters.st / OUT_ShadowParameters.w;
    float fZ = OUT_ShadowParameters.z / OUT_ShadowParameters.w;
    float fBias = 0.0005 * tan(acos(dot(OUT_Normal, OUT_LightDirection)));
    float fShadow = 1.0;
    if (OUT_ShadowParameters.w > 0.0)
    {
        fShadow = max(step(getCurrentDepth(fZ), getShadowMapPassDepth(vTexCoord)), 0.5);
    }
    
    diffuseColor = vec4(diffuseColor.rgb * min(diffuseFactor, 1.0), 1.0);
    diffuseColor = mix(vec4(vec3(0.16, 0.32, 0.32) * diffuseFactor, 1.0), diffuseColor, OUT_Fog);
    diffuseColor.rgb *= fShadow;
    gl_FragColor = diffuseColor;
}