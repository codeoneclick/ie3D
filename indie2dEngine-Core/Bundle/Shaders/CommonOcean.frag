varying mediump vec4   OUT_TexCoordProj;
varying mediump vec2   OUT_TexCoord;
varying mediump vec3   OUT_LightDirection;
varying mediump vec3   OUT_LightPosition;
varying mediump vec3   OUT_CameraDirection;
varying mediump vec3   OUT_CameraPosition;
varying mediump vec3   OUT_Position;

uniform highp float  FLOAT_Timer;

uniform sampler2D SAMPLER_01;
uniform sampler2D SAMPLER_02;
uniform sampler2D SAMPLER_03;
uniform sampler2D SAMPLER_04;

const lowp vec4 vSpecularColor = vec4(1.25, 0.5, 0.25, 1.0);
const lowp vec4 vWaterColor = vec4(0.17, 0.27, 0.26, 1.0);

const mediump float fLightConst = 0.3;
const mediump float fLightLinear = 0.007;
const mediump float fLightExponential = 0.00008;
const mediump float fPerturbationFactor = 0.1;

const mediump vec3 k_vBinormal = vec3(1.0, 0.0, 0.0);
const mediump vec3 k_vTangent = vec3(0.0, 0.0, 1.0);
const mediump vec3 k_vNormal = vec3(0.0, 1.0, 0.0);

void main(void)
{
    mediump vec2 vTexCoord_01 = vec2(OUT_TexCoord.x + sin(FLOAT_Timer) * 0.33,
								     OUT_TexCoord.y + cos(FLOAT_Timer) * 0.66);
	
	mediump vec2 vTexCoord_02 = vec2(OUT_TexCoord.x - sin(FLOAT_Timer) * 0.75,
								     OUT_TexCoord.y - cos(FLOAT_Timer) * 0.25);
                                     
    lowp vec4 vNormalColor_01 = texture2D(SAMPLER_04, vTexCoord_01) * 2.0 - 1.0;
    lowp vec4 vNormalColor_02 = texture2D(SAMPLER_04, vTexCoord_02) * 2.0 - 1.0;
    lowp vec3 vNormalColor = mix(vNormalColor_01, vNormalColor_02, 0.5).xyz;                                 

    mediump vec2 vTexCoordProj = OUT_TexCoordProj.xy;
	vTexCoordProj.x = 0.5 - 0.5 * vTexCoordProj.x / OUT_TexCoordProj.w;
	vTexCoordProj.y = 0.5 + 0.5 * vTexCoordProj.y / OUT_TexCoordProj.w;
    
    mediump vec2 vPerturbation = fPerturbationFactor * vNormalColor.rg;
    mediump vec2 vPerturbatedTexCoord = vTexCoordProj + vPerturbation;
    vPerturbatedTexCoord = clamp(vPerturbatedTexCoord, 0.001, 0.999);
    
    lowp vec4 vReflectionColor = texture2D(SAMPLER_01, vPerturbatedTexCoord);
    lowp vec4 vRefractionColor = texture2D(SAMPLER_02, vec2(0.5 + (0.5 - vPerturbatedTexCoord.x), vPerturbatedTexCoord.y));
    
    mediump vec3 vCameraDirection = normalize(OUT_Position - OUT_CameraPosition);
    mediump vec3 vTemp;
    vTemp.x = dot(vCameraDirection, k_vTangent);
    vTemp.y = dot(vCameraDirection, k_vBinormal);
    vTemp.z = dot(vCameraDirection, k_vNormal);
    vCameraDirection = normalize(vTemp);
    
    mediump vec3 vLightDirection = normalize(OUT_LightPosition - OUT_Position);
    vTemp.x = dot(vLightDirection, k_vTangent);
    vTemp.y = dot(vLightDirection, k_vBinormal);
    vTemp.z = dot(vLightDirection, k_vNormal);
    vLightDirection = normalize(vTemp);
    
    mediump vec3 vReflect = reflect(vCameraDirection, vNormalColor);
    lowp vec4 vColor = mix(vReflectionColor, vRefractionColor, 0.75);
    
    mediump float fLightDistance = length(OUT_Position - OUT_LightPosition);
    mediump float fLightAttitude = fLightConst + fLightLinear * fLightDistance + fLightExponential * fLightDistance * fLightDistance;
    mediump float fDiffuseFactor = max(dot(vNormalColor, vLightDirection), 0.0) / fLightAttitude;
    mediump float fSpecularFactor = pow(max(dot(vLightDirection, vReflect), 0.0), 16.0);
    lowp vec4 fDiffuseColor = vColor;
    
    fDiffuseColor = fDiffuseColor * fDiffuseFactor + vSpecularColor * fSpecularFactor;
    vColor = fDiffuseColor + vColor * 0.25;
    gl_FragColor = vColor;
}

