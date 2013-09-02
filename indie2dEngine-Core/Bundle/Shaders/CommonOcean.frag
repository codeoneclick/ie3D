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
    
    mediump vec2 vTexCoord_01 = vec2(OUT_TexCoord.x * 8.0 + sin(OUT_Timer) * 0.33,
								   OUT_TexCoord.y * 8.0 + cos(OUT_Timer) * 0.66);
	
	mediump vec2 vTexCoord_02 = vec2(OUT_TexCoord.x * 8.0 - sin(OUT_Timer) * 0.75,
								   OUT_TexCoord.y * 8.0 - cos(OUT_Timer) * 0.25);
                                   
    lowp vec4 vWaterColor = (texture2D(SAMPLER_03, vTexCoord_01) + texture2D(SAMPLER_03, vTexCoord_02));
    vRefractionColor = mix(vRefractionColor, vWaterColor, 0.25);
    vReflectionColor = mix(vRefractionColor, vReflectionColor, 0.25);
    
    mediump vec3 vBinormal = vec3(1.0, 0.0, 0.0);
    mediump vec3 vTangent = vec3(0.0, 0.0, 1.0);
    mediump vec3 vNormal = vec3(0.0, 1.0, 0.0);
    
    mediump vec3 vLightDirection = normalize(OUT_LightPosition - OUT_Position);
    mediump vec3 vCameraDirection = normalize( OUT_Position - OUT_CameraPosition);
    
    mediump vec3 vTemp;
    vTemp.x = dot(vLightDirection, vTangent);
    vTemp.y = dot(vLightDirection, vBinormal);
    vTemp.z = dot(vLightDirection, vNormal);
    vLightDirection = normalize(vTemp);
    
    vTemp.x = dot(vCameraDirection, vTangent);
    vTemp.y = dot(vCameraDirection, vBinormal);
    vTemp.z = dot(vCameraDirection, vNormal);
    vCameraDirection = normalize(vTemp);
    
    lowp vec3 vNormalColor = (texture2D(SAMPLER_03, vTexCoord_01) * 2.0 - 1.0).xyz;

    lowp vec4 vSpecularColor = vec4(0.7, 0.7, 0.7, 1.0);
    lowp vec3 vReflect = reflect(vCameraDirection, vNormalColor);
    vSpecularColor = vSpecularColor * pow(max(dot(vLightDirection, vReflect), 0.0), 16.0);
    
    mediump float fDiffuseFactor = max(dot(vNormalColor, vLightDirection), 0.0);
    
    gl_FragColor = vReflectionColor * vec4(0.17, 0.27, 0.26, 1.0) * fDiffuseFactor + vSpecularColor;
}

