varying mediump vec4   OUT_TexCoordProj;
varying mediump vec2   OUT_TexCoord;
varying mediump vec3   OUT_LightPosition;
varying mediump vec3   OUT_CameraPosition;
varying mediump vec3   OUT_Position;

uniform lowp float  OUT_Timer;

uniform sampler2D SAMPLER_01;
uniform sampler2D SAMPLER_02;
uniform sampler2D SAMPLER_03;
uniform sampler2D SAMPLER_04;

const lowp vec3 vBinormal = vec3(1.0, 0.0, 0.0);
const lowp vec3 vTangent = vec3(0.0, 0.0, 1.0);
const lowp vec3 vNormal = vec3(0.0, 1.0, 0.0);

const lowp vec4 vSpecularColor = vec4(1.25, 0.5, 0.25, 1.0);
const lowp vec4 vWaterColor = vec4(0.17, 0.27, 0.26, 1.0);

const mediump float fLightConst = 0.3;
const mediump float fLightLinear = 0.007;
const mediump float fLightExponential = 0.00008;

void main(void)
{
    mediump vec2 vTexCoordProj = OUT_TexCoordProj.xy;
	vTexCoordProj.x = 0.5 - 0.5 * vTexCoordProj.x / OUT_TexCoordProj.w;
	vTexCoordProj.y = 0.5 + 0.5 * vTexCoordProj.y / OUT_TexCoordProj.w;
	vTexCoordProj = clamp(vTexCoordProj, 0.001, 0.999);
    lowp vec4 vReflectionColor = texture2D(SAMPLER_01, vTexCoordProj);
    
    vTexCoordProj = OUT_TexCoordProj.xy;
    vTexCoordProj.x = 0.5 + 0.5 * vTexCoordProj.x / OUT_TexCoordProj.w;
    vTexCoordProj.y = 0.5 + 0.5 * vTexCoordProj.y / OUT_TexCoordProj.w;
    vTexCoordProj = clamp(vTexCoordProj, 0.001, 0.999);
    lowp vec4 vRefractionColor = texture2D(SAMPLER_02, vTexCoordProj);
    
    mediump vec2 vTexCoord_01 = vec2(OUT_TexCoord.x * 4.0 + sin(OUT_Timer) * 0.33,
								     OUT_TexCoord.y * 4.0 + cos(OUT_Timer) * 0.66);
	
	mediump vec2 vTexCoord_02 = vec2(OUT_TexCoord.x * 4.0 - sin(OUT_Timer) * 0.75,
								     OUT_TexCoord.y * 4.0 - cos(OUT_Timer) * 0.25);
    
    mediump vec2 vTexCoord_03 = vec2(OUT_TexCoord.x * 8.0 - sin(OUT_Timer) * 0.64,
                                     OUT_TexCoord.y * 8.0 + cos(OUT_Timer) * 0.32);
                                   
    lowp vec4 vColor = texture2D(SAMPLER_03, vTexCoord_01) + texture2D(SAMPLER_03, vTexCoord_02);
    vColor = mix(vRefractionColor, vColor, 0.5);
    vColor = mix(vColor, vReflectionColor, 0.5);
    vColor *= vWaterColor;
    
    highp vec3 vLightDirection = normalize(OUT_LightPosition - OUT_Position);
    highp vec3 vCameraDirection = normalize( OUT_Position - OUT_CameraPosition);
    highp float fLightDistance = length(OUT_Position - OUT_LightPosition);
    lowp vec3 vNormalColor = (texture2D(SAMPLER_04, vTexCoord_03) * 2.0 - 1.0).xyz;
    
    highp vec3 vTemp;
    vTemp.x = dot(vLightDirection, vTangent);
    vTemp.y = dot(vLightDirection, vBinormal);
    vTemp.z = dot(vLightDirection, vNormal);
    vLightDirection = normalize(vTemp);
    
    vTemp.x = dot(vCameraDirection, vTangent);
    vTemp.y = dot(vCameraDirection, vBinormal);
    vTemp.z = dot(vCameraDirection, vNormal);
    vCameraDirection = normalize(vTemp);
    
    highp float fLightAttitude = fLightConst + fLightLinear * fLightDistance + fLightExponential * fLightDistance * fLightDistance;
    highp float fDiffuseFactor = max(dot(vNormalColor, vLightDirection), 0.75) / fLightAttitude;
    
    highp vec3 vReflect = reflect(vCameraDirection, vNormalColor);
    highp float fSpecularFactor = pow(max(dot(vLightDirection, vReflect), 0.0), 16.0);
    
    gl_FragColor = vColor * fDiffuseFactor + vSpecularColor * fSpecularFactor;
}

