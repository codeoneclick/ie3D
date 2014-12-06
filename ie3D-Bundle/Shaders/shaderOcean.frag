
#if defined(__OPENGL_30__)

in  vec4   OUT_TexCoordProjection;
in  vec2   OUT_TexCoordDisplace_01;
in  vec2   OUT_TexCoordDisplace_02;
in  vec2   OUT_TexCoord;
//in  vec3   OUT_LightPosition;
//in  vec3   OUT_CameraPosition;
//in  vec3   OUT_Position;
in  vec4   OUT_Extra;

#else

varying  vec4   OUT_TexCoordProjection;
varying  vec2   OUT_TexCoordDisplace_01;
varying  vec2   OUT_TexCoordDisplace_02;
varying  vec2   OUT_TexCoord;
//varying  vec3   OUT_LightPosition;
//varying  vec3   OUT_CameraPosition;
varying vec3 v_cameraDirWS;
varying vec3 v_cameraDirTS;
varying vec3 v_lightDirWS;
varying vec3 v_lightDirTS;
varying vec3 v_positionWS;
varying float v_fogDistance;
//varying  vec3   OUT_Position;
varying  vec4   OUT_Extra;

#endif

uniform sampler2D SAMPLER_01;
uniform sampler2D SAMPLER_02;
uniform sampler2D SAMPLER_03;
uniform sampler2D SAMPLER_04;

const  vec4 vSpecularColor = vec4(1.0, 1.0, 0.75, 1.0);

const  float fLightConst = 0.3;
const  float fLightLinear = 0.007;
const  float fLightExponential = 0.00008;
const  vec2 vPerturbationFactor = vec2(0.025, 0.025);

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
    
#if defined(__PIXEL_TBN__)
    
    vec3 cameraDirTS;
    cameraDirTS.x = dot(cameraDirWS, k_vTangent);
    cameraDirTS.y = dot(cameraDirWS, k_vBinormal);
    cameraDirTS.z = dot(cameraDirWS, k_vNormal);
    cameraDirTS = normalize(cameraDirTS);
    
    vTemp.x = dot(lightDirWS, k_vTangent);
    vTemp.y = dot(lightDirWS, k_vBinormal);
    vTemp.z = dot(lightDirWS, k_vNormal);
    lightDirTS = normalize(vTemp);
    
#else
    
    vec3 cameraDirTS = normalize(v_cameraDirTS);
    vec3 lightDirTS = normalize(v_lightDirTS);
    
#endif
    
    vec4 vNormalColor_01 = texture2D(SAMPLER_03, OUT_TexCoordDisplace_01 / OUT_Extra.y) * k_2 - k_1;
    vec4 vNormalColor_02 = texture2D(SAMPLER_03, OUT_TexCoordDisplace_02 / OUT_Extra.y) * k_2 - k_1;
    vec3 vNormalColor = normalize(mix(vNormalColor_01, vNormalColor_02, k_05).xyz);
    
    vec2 vTexCoordProj = OUT_TexCoordProjection.xy;
    vTexCoordProj = k_05 + k_05 * vTexCoordProj / OUT_TexCoordProjection.w * vec2(-k_1, k_1);
    
    float fresnel = dot(k_vNormal, cameraDirWS);
    vec4 vHeightmapColor = texture2D(SAMPLER_04, OUT_TexCoord) * OUT_Extra.x;
    float deep = clamp(vHeightmapColor.a, k_05, k_1);
    vec2 vPerturbation = vPerturbationFactor * vNormalColor.xy;
    vec2 vPerturbatedTexCoord = vTexCoordProj + vPerturbation;
    
    vec4 vReflectionColor = texture2D(SAMPLER_01, vPerturbatedTexCoord);
    vec4 vRefractionColor = texture2D(SAMPLER_02, vec2(k_05 + (k_05 - vPerturbatedTexCoord.x), vPerturbatedTexCoord.y));
    
    
    
    float diffuseFactor = clamp(dot(vNormalColor, lightDirTS), k_0, k_1);
    vec3 reflectFactor = reflect(-lightDirTS, vNormalColor);
    float specularFactor = pow(dot(reflectFactor, cameraDirTS), 512.0);
    //vec3 vReflect = reflect(cameraDirTS, vNormalColor);
    //float fSpecularFactor = clamp(pow(max(dot(lightDirTS, vReflect), 0.0), 32.0), 0.0, 1.0);
    
    vec4 vDiffuseColor = mix(k_waterColorGreen, k_waterColorBlue, diffuseFactor);
    diffuseFactor = max(dot(k_vNormal, lightDirWS), k_0);
    //float diffuseFactor = max(dot(k_vNormal, normalize(OUT_LightPosition - OUT_Position)), 0.25);
    
    //vRefractionColor.a = fresnel * vHeightmapColor.a;
    //vReflectionColor.a = 1.0 - vReflectionColor.a;
    vReflectionColor = mix(vDiffuseColor, vReflectionColor, vReflectionColor.a);
    vRefractionColor = mix(vDiffuseColor, vRefractionColor, deep);
    vec4 color = mix(vReflectionColor, vRefractionColor, fresnel) * max(diffuseFactor, k_05);
    
    //float fFogDistance = length(vec3(256.0, 0.0, 256.0) - OUT_Position);
    //float fogFactor = clamp((v_fogDistance - 384.0) / 448.0, 0.0, 1.0);
    
    color.a = k_1 - v_fogDistance;
    gl_FragColor = color + vSpecularColor * specularFactor * diffuseFactor;
}

