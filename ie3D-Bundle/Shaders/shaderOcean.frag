
#if defined(__OPENGL_30__)

in  vec4   OUT_TexCoordProjection;
in  vec2   OUT_TexCoordDisplace_01;
in  vec2   OUT_TexCoordDisplace_02;
in  vec2   OUT_TexCoord;

#else

varying vec4 OUT_TexCoordProjection;
varying vec2 OUT_TexCoordDisplace_01;
varying vec2 OUT_TexCoordDisplace_02;
varying vec2 OUT_TexCoord;
varying vec3 v_cameraDirWS;
varying vec3 v_cameraDirTS;
varying vec3 v_lightDirWS;
varying vec3 v_lightDirTS;
varying vec3 v_positionWS;
varying float v_fogDistance;

#endif

uniform sampler2D SAMPLER_01;
uniform sampler2D SAMPLER_02;
uniform sampler2D SAMPLER_03;
uniform sampler2D SAMPLER_04;

const vec4 k_specularColor = vec4(1.0, 1.0, 0.75, 1.0);
const float k_specularShine = 512.0;
const vec2 k_perturbationFactor = vec2(0.025, 0.025);

const  vec3 k_vBinormal = vec3(1.0, 0.0, 0.0);
const  vec3 k_vTangent = vec3(0.0, 0.0, 1.0);
const  vec3 k_vNormal = vec3(0.0, 1.0, 0.0);

const float k_0 = 0.0;
const float k_1 = 1.0;
const float k_2 = 2.0;
const float k_05 = 0.5;

const vec4 k_waterColorBlue = vec4(0.16, 0.32, 0.32, 1.0);
const vec4 k_waterColorGreen = vec4(0.16, 0.32, 0.16, 1.0);

void main(void)
{
    vec3 cameraDirWS = normalize(v_cameraDirWS - v_positionWS);
    vec3 lightDirWS = normalize(v_lightDirWS - v_positionWS);

#define __PIXEL_TBN__ 1
#if defined(__PIXEL_TBN__)
    
    vec3 cameraDirTS;
    cameraDirTS.x = dot(cameraDirWS, k_vTangent);
    cameraDirTS.y = dot(cameraDirWS, k_vBinormal);
    cameraDirTS.z = dot(cameraDirWS, k_vNormal);
    cameraDirTS = normalize(cameraDirTS);
    
    vec3 lightDirTS;
    lightDirTS.x = dot(lightDirWS, k_vTangent);
    lightDirTS.y = dot(lightDirWS, k_vBinormal);
    lightDirTS.z = dot(lightDirWS, k_vNormal);
    lightDirTS = normalize(lightDirTS);
    
#else
    
    vec3 cameraDirTS = normalize(v_cameraDirTS);
    vec3 lightDirTS = normalize(v_lightDirTS);
    
#endif
    
    vec4 normalColor_01 = texture2D(SAMPLER_03, OUT_TexCoordDisplace_01) * k_2 - k_1;
    vec4 normalColor_02 = texture2D(SAMPLER_03, OUT_TexCoordDisplace_02) * k_2 - k_1;
    vec3 normalColor = normalize(mix(normalColor_01, normalColor_02, k_05).xyz);
    
    float diffuseIntensity = clamp(dot(normalColor, lightDirTS), k_0, k_1);
    vec3 reflectVector = normalize(2.0 * diffuseIntensity * normalColor - lightDirTS);
    float specularIntensity = pow(dot(reflectVector, cameraDirTS), k_specularShine);
    
    vec2 texCoordProj = OUT_TexCoordProjection.xy;
    texCoordProj = k_05 + k_05 * texCoordProj / OUT_TexCoordProjection.w * vec2(-k_1, k_1);
    
    vec2 perturbationIntensity = k_perturbationFactor * normalColor.xy;
    vec2 perturbatedTexCoord = texCoordProj + perturbationIntensity;
    
    vec4 reflectionColor = texture2D(SAMPLER_01, perturbatedTexCoord);
    vec4 refractionColor = texture2D(SAMPLER_02, vec2(k_05 + (k_05 - perturbatedTexCoord.x), perturbatedTexCoord.y));
    
    float fresnel = dot(k_vNormal, cameraDirWS);
    vec4 diffuseColor = mix(k_waterColorGreen, k_waterColorBlue, diffuseIntensity);
    
    reflectionColor = mix(diffuseColor, reflectionColor, reflectionColor.a);
    refractionColor = mix(diffuseColor, refractionColor, refractionColor.a);
    
    vec4 color = mix(reflectionColor, refractionColor, fresnel);
    color = color * diffuseIntensity + k_specularColor * specularIntensity;
    
    color.a = k_1 - v_fogDistance;
    
    gl_FragColor = color;
}

