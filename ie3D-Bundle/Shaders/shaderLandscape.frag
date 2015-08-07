#if defined(__OPENGL_30__)

in vec2   OUT_TexCoord;
in float  OUT_ClipPlane;
in vec4   OUT_ShadowParameters;

#if defined PP_PARALLAX_MAPPING

in vec3 v_positionWS;
in mat3 v_tangentMatrix;

#else

in vec3 v_eyeDirTS;
in vec3 v_lightDirTS;

#endif

#else

varying vec2   OUT_TexCoord;
varying float  OUT_ClipPlane;
varying vec4   OUT_ShadowParameters;

#if defined PP_PARALLAX_MAPPING

varying vec3 v_positionWS;
varying mat3 v_tangentMatrix;

#else

varying vec3 v_eyeDirTS;
varying vec3 v_lightDirTS;

#endif

#endif

uniform sampler2D SAMPLER_01;
uniform sampler2D SAMPLER_02;
uniform sampler2D SAMPLER_03;

#if defined PP_PARALLAX_MAPPING

uniform vec3 VECTOR_CameraPosition;
uniform vec3 VECTOR_GlobalLightPosition;

#endif

const vec2 vCameraRange = vec2(0.01, 1024.0);
const float k_fTexCoordScale = 8.0;

float getShadowMapPassDepth(in vec2 vTexCoord)
{
    float fNearZ = vCameraRange.x;
    float fFarZ = vCameraRange.y;
    float fDepth = texture2D(SAMPLER_03, vTexCoord).x;
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
    
#if defined PP_PARALLAX_MAPPING
    
    vec3 eyeDirTS = normalize((VECTOR_CameraPosition - v_positionWS.xyz) * v_tangentMatrix);
    vec3 lightDirTS = normalize((VECTOR_GlobalLightPosition - v_positionWS.xyz) * v_tangentMatrix);
    
#else
    
    vec3 eyeDirTS = v_eyeDirTS;
    vec3 lightDirTS = v_lightDirTS;
    
#endif
    
    vec2 vTexCoord = OUT_TexCoord;
    
#if defined PARALLAX_MAPPING
    
    float scale = 0.01;
    float bias = 0.005;
    
    float fDepth = texture2D(SAMPLER_02, vTexCoord).a * scale - bias;
    vec2 offset = (fDepth * eyeDirTS.xy) / eyeDirTS.z;
    vTexCoord += offset;
    
#endif
    
    vec3 normalColor = texture2D(SAMPLER_02, vTexCoord).rgb * 2.0 - 1.0;
    
    float diffuseIntensity = clamp(dot(normalColor, lightDirTS), 0.0, 1.0);
    
    vTexCoord = OUT_ShadowParameters.st / OUT_ShadowParameters.w;
    float fZ = OUT_ShadowParameters.z / OUT_ShadowParameters.w;
    float fBias = 0.0005 * tan(acos(dot(normalColor, lightDirTS)));
    float fShadow = max(step(getCurrentDepth(fZ), getShadowMapPassDepth(vTexCoord)), 0.5);
    
#if defined PARALLAX_MAPPING
    
    vec4 color = diffuseIntensity * texture2D(SAMPLER_01, OUT_TexCoord + offset);
    
#else
    
    vec4 color = diffuseIntensity * texture2D(SAMPLER_01, OUT_TexCoord);
    
#endif
    
    color.rgb *= fShadow;
    color.a = 1.0;
    gl_FragColor = color;
}