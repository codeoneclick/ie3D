
#if defined(__OPENGL_30__)

in  vec4   OUT_TexCoordProjection;
in  vec2   OUT_TexCoordDisplace_01;
in  vec2   OUT_TexCoordDisplace_02;
in  vec2   OUT_TexCoord;
in  vec3   OUT_LightPosition;
in  vec3   OUT_CameraPosition;
in  vec3   OUT_Position;
in  vec4   OUT_Extra;

#else

varying  vec4   OUT_TexCoordProjection;
varying  vec2   OUT_TexCoordDisplace_01;
varying  vec2   OUT_TexCoordDisplace_02;
varying  vec2   OUT_TexCoord;
varying  vec3   OUT_LightPosition;
varying  vec3   OUT_CameraPosition;
varying  vec3   OUT_Position;
varying  vec4   OUT_Extra;

#endif

uniform sampler2D SAMPLER_01;
uniform sampler2D SAMPLER_02;
uniform sampler2D SAMPLER_03;
uniform sampler2D SAMPLER_04;

const  vec4 vSpecularColor = vec4(1.0, 0.5, 0.25, 1.0);

const  float fLightConst = 0.3;
const  float fLightLinear = 0.007;
const  float fLightExponential = 0.00008;
const  vec2 vPerturbationFactor = vec2(0.025, 0.025);

const  vec3 k_vBinormal = vec3(1.0, 0.0, 0.0);
const  vec3 k_vTangent = vec3(0.0, 0.0, 1.0);
const  vec3 k_vNormal = vec3(0.0, 1.0, 0.0);

void main(void)
{
     vec4 vNormalColor_01 = texture2D(SAMPLER_03, OUT_TexCoordDisplace_01 / OUT_Extra.y) * 2.0 - 1.0;
     vec4 vNormalColor_02 = texture2D(SAMPLER_03, OUT_TexCoordDisplace_02 / OUT_Extra.y) * 2.0 - 1.0;
     vec3 vNormalColor = mix(vNormalColor_01, vNormalColor_02, 0.5).xyz;
    
     vec2 vTexCoordProj = OUT_TexCoordProjection.xy;
    vTexCoordProj = 0.5 + 0.5 * vTexCoordProj / OUT_TexCoordProjection.w * vec2(-1.0, 1.0);
    
     float fresnel = dot(normalize(OUT_CameraPosition - OUT_Position), k_vNormal);
     vec4 vHeightmapColor = texture2D(SAMPLER_04, OUT_TexCoord) * OUT_Extra.x + vec4(1.0 - OUT_Extra.x);
     vec2 vPerturbation = vPerturbationFactor * vNormalColor.xy * (vHeightmapColor.a * 2.0);
     vec2 vPerturbatedTexCoord = vTexCoordProj + vPerturbation;
    
     vec4 vReflectionColor = texture2D(SAMPLER_01, vPerturbatedTexCoord);
     vec4 vRefractionColor = texture2D(SAMPLER_02, vec2(0.5 + (0.5 - vPerturbatedTexCoord.x), vPerturbatedTexCoord.y));
    
     vec3 vCameraDirection = normalize(OUT_Position - OUT_CameraPosition);
    
     vec3 vTemp;
    vTemp.x = dot(vCameraDirection, k_vTangent);
    vTemp.y = dot(vCameraDirection, k_vBinormal);
    vTemp.z = dot(vCameraDirection, k_vNormal);
    vCameraDirection = normalize(vTemp);
    
     vec3 vLightDirection = normalize(vec3(512.0, 1024.0, 64.0) - OUT_Position);
    vTemp.x = dot(vLightDirection, k_vTangent);
    vTemp.y = dot(vLightDirection, k_vBinormal);
    vTemp.z = dot(vLightDirection, k_vNormal);
    vLightDirection = normalize(vTemp);
    
     vec3 vReflect = reflect(vCameraDirection, vNormalColor);
     float fSpecularFactor = clamp(pow(max(dot(vLightDirection, vReflect), 0.0), 32.0), 0.0, 1.0);
     float fDiffuseFactor = max(dot(vNormalColor, vLightDirection), 0.5);
     vec4 vDiffuseColor = fDiffuseFactor * mix(vec4(0.16, 0.32, 0.16, 1.0), vec4(0.16, 0.32, 0.32, 1.0), vHeightmapColor.a * 2.0);
     float diffuseFactor = max(dot(k_vNormal, normalize(OUT_LightPosition - OUT_Position)), 0.25);

    vReflectionColor = mix(vDiffuseColor, vReflectionColor, vReflectionColor.a);
    vRefractionColor = mix(vDiffuseColor, vRefractionColor, vHeightmapColor.a * 4.0 * fresnel * OUT_Extra.x);
     vec4 color = mix(vReflectionColor, vRefractionColor, fresnel) * min(diffuseFactor, 1.0);
    
     float fFogDistance = length(vec3(256.0, 0.0, 256.0) - OUT_Position);
     float fFogFactor = clamp((fFogDistance - 384.0) / 448.0, 0.0, 1.0);
    
    color.a = 1.0 - fFogFactor;
    gl_FragColor = color + vec4(fSpecularFactor);
}

