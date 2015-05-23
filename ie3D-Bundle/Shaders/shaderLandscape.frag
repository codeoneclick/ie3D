
#if defined(__OPENGL_30__)

in vec2   OUT_TexCoord;
in vec4   OUT_TillingTexcoordLayer_01;
in vec4   OUT_TillingTexcoordLayer_02;
in vec4   OUT_TillingTexcoordLayer_03;
in float  OUT_ClipPlane;
//in vec3   OUT_CameraPosition;
//in vec3   OUT_LightDirection;
in vec3   OUT_Normal;
in float  OUT_Fog;
in vec4   OUT_ShadowParameters;

in vec3 v_positionWS;
in mat3 v_tangentMatrix;

#else

varying vec2   OUT_TexCoord;
varying vec4   OUT_TillingTexcoordLayer_01;
varying vec4   OUT_TillingTexcoordLayer_02;
varying vec4   OUT_TillingTexcoordLayer_03;
varying float  OUT_ClipPlane;
//varying vec3   OUT_CameraPosition;
//varying vec3   OUT_LightDirection;
varying vec3   OUT_Normal;
varying float  OUT_Fog;
varying vec4   OUT_ShadowParameters;

varying vec3 v_positionWS;
varying mat3 v_tangentMatrix;

#endif

uniform sampler2D SAMPLER_01;
uniform sampler2D SAMPLER_02;
uniform sampler2D SAMPLER_03;
uniform sampler2D SAMPLER_04;
uniform sampler2D SAMPLER_05;

uniform vec3 VECTOR_CameraPosition;
uniform vec3 VECTOR_GlobalLightPosition;

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
    vec3 eyeDirTS = normalize((VECTOR_CameraPosition - v_positionWS.xyz) * v_tangentMatrix);
    vec3 lightDirTS = normalize((VECTOR_GlobalLightPosition - v_positionWS.xyz) * v_tangentMatrix);
    
    float scale = 0.04;
    float bias = 0.02;
    
    float fDepth = texture2D(SAMPLER_03, OUT_TexCoord * 8.0).x * bias - scale;
    vec2 vTexCoord = OUT_TexCoord * 8.0 + (fDepth * eyeDirTS.xy);
    
    //vec3 eyeDirTS = normalize(v_eyeDirTS);
    //vec3 lightDirTS = normalize(v_lightDirTS);
    
    vec3 normalColor = texture2D(SAMPLER_02, vTexCoord).rgb * 2.0 - 1.0;
    
    float diffuseIntensity = clamp(dot(normalColor, lightDirTS), 0.0, 1.0);
    vec3 reflectVector = normalize(2.0 * diffuseIntensity * normalColor - lightDirTS);
    reflectVector.y = max(reflectVector.y , 0.0);
    float specularIntensity = clamp(pow(dot(reflectVector, eyeDirTS), 128.0) * diffuseIntensity, 0.0, 1.0);
    
    
    /*float diffuseFactor = max(dot(OUT_Normal, OUT_LightDirection), 0.25);
    vec4 diffuseColor = texture2D(SAMPLER_02, OUT_TexCoord);

    vec2 vTexCoord = OUT_ShadowParameters.st / OUT_ShadowParameters.w;
    float fZ = OUT_ShadowParameters.z / OUT_ShadowParameters.w;
    float fBias = 0.0005 * tan(acos(dot(OUT_Normal, OUT_LightDirection)));
    float fShadow = max(step(getCurrentDepth(fZ), getShadowMapPassDepth(vTexCoord)), 0.5);
    
    diffuseColor = vec4(diffuseColor.rgb * min(diffuseFactor, 1.0), 1.0);
    diffuseColor = mix(vec4(vec3(0.16, 0.32, 0.32) * diffuseFactor, 1.0), diffuseColor, OUT_Fog);
    diffuseColor.rgb *= fShadow;*/
    vec4 color = diffuseIntensity * texture2D(SAMPLER_01, OUT_TexCoord) + specularIntensity * vec4(1.2, 1.0, 0.75, 1.0);
    color.a = 1.0;
    gl_FragColor = color;// ;
}