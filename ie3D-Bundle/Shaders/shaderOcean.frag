varying mediump vec4   OUT_TexCoordProjection;
varying mediump vec2   OUT_TexCoordDisplace_01;
varying mediump vec2   OUT_TexCoordDisplace_02;
varying mediump vec2   OUT_TexCoord;
varying mediump vec3   OUT_LightPosition;
varying mediump vec3   OUT_CameraPosition;
varying mediump vec3   OUT_Position;

uniform sampler2D SAMPLER_01;
uniform sampler2D SAMPLER_02;
uniform sampler2D SAMPLER_03;
uniform sampler2D SAMPLER_04;

const lowp vec4 vSpecularColor = vec4(1.0, 0.5, 0.25, 1.0);

const mediump float fLightConst = 0.3;
const mediump float fLightLinear = 0.007;
const mediump float fLightExponential = 0.00008;
const mediump vec2 vPerturbationFactor = vec2(0.025, 0.025);

const lowp vec3 k_vBinormal = vec3(1.0, 0.0, 0.0);
const lowp vec3 k_vTangent = vec3(0.0, 0.0, 1.0);
const lowp vec3 k_vNormal = vec3(0.0, 1.0, 0.0);

void main(void)
{
    lowp vec4 vNormalColor_01 = texture2D(SAMPLER_03, OUT_TexCoordDisplace_01) * 2.0 - 1.0;
    lowp vec4 vNormalColor_02 = texture2D(SAMPLER_03, OUT_TexCoordDisplace_02) * 2.0 - 1.0;
    lowp vec3 vNormalColor = mix(vNormalColor_01, vNormalColor_02, 0.5).xyz;
    
    mediump vec2 vTexCoordProj = OUT_TexCoordProjection.xy;
    vTexCoordProj = 0.5 + 0.5 * vTexCoordProj / OUT_TexCoordProjection.w * vec2(-1.0, 1.0);
    
    mediump float fresnel = dot(normalize(OUT_CameraPosition - OUT_Position), k_vNormal);
    lowp vec4 vHeightmapColor = texture2D(SAMPLER_04, OUT_TexCoord);
    mediump vec2 vPerturbation = vPerturbationFactor * vNormalColor.xy * (vHeightmapColor.a * 2.0);
    mediump vec2 vPerturbatedTexCoord = vTexCoordProj + vPerturbation;
    
    lowp vec4 vReflectionColor = texture2D(SAMPLER_01, vPerturbatedTexCoord);
    lowp vec4 vRefractionColor = texture2D(SAMPLER_02, vec2(0.5 + (0.5 - vPerturbatedTexCoord.x), vPerturbatedTexCoord.y));
    
    mediump vec3 vCameraDirection = normalize(OUT_Position - OUT_CameraPosition);
    
    mediump vec3 vTemp;
    vTemp.x = dot(vCameraDirection, k_vTangent);
    vTemp.y = dot(vCameraDirection, k_vBinormal);
    vTemp.z = dot(vCameraDirection, k_vNormal);
    vCameraDirection = normalize(vTemp);
    
    mediump vec3 vLightDirection = normalize(vec3(512.0, 1024.0, 64.0) - OUT_Position);
    vTemp.x = dot(vLightDirection, k_vTangent);
    vTemp.y = dot(vLightDirection, k_vBinormal);
    vTemp.z = dot(vLightDirection, k_vNormal);
    vLightDirection = normalize(vTemp);
    
    mediump vec3 vReflect = reflect(vCameraDirection, vNormalColor);
    mediump float fSpecularFactor = clamp(pow(max(dot(vLightDirection, vReflect), 0.0), 32.0), 0.0, 1.0);
    lowp float fDiffuseFactor = max(dot(vNormalColor, vLightDirection), 0.5);
    lowp vec4 vDiffuseColor = fDiffuseFactor * mix(vec4(0.16, 0.32, 0.32, 1.0), vec4(0.16, 0.32, 0.16, 1.0), vHeightmapColor.a * 2.0);
    vDiffuseColor.a = 1.0;
    
    vReflectionColor = mix(vDiffuseColor, vReflectionColor, vReflectionColor.a);
    vRefractionColor = mix(vDiffuseColor, vRefractionColor, vHeightmapColor.a * 4.0 * fresnel);
    
    gl_FragColor = mix(vReflectionColor, vRefractionColor, fresnel) + vec4(fSpecularFactor);
}

