varying mediump vec4   OUT_TexCoordProj;
varying mediump vec2   OUT_TexCoord;
varying mediump vec3   OUT_LightDirection[4];
varying mediump float  OUT_LightDistance[4];
varying mediump vec3   OUT_CameraDirection;
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
    
    //mediump vec3 vCameraDirection = OUT_CameraPosition; //normalize(OUT_Position - OUT_CameraPosition);
    
    //mediump vec3 vTemp;
    //vTemp.x = dot(vCameraDirection, vTangent);
    //vTemp.y = dot(vCameraDirection, vBinormal);
    //vTemp.z = dot(vCameraDirection, vNormal);
    //vCameraDirection = normalize(vTemp);
    
    mediump vec3 vReflect = reflect(OUT_CameraDirection, vNormalColor);
    lowp vec4 vColor = mix(vReflectionColor, vRefractionColor, 0.75);
    
    //mediump vec3  vLightDirection;
    mediump float fLightDistance;
    mediump float fLightAttitude;
    mediump float fDiffuseFactor;
    mediump float fSpecularFactor;
    lowp vec4 fDiffuseColor = vColor;

    //for(int i = 0; i < INT_LightsCount; i++)
    //{
    //vLightDirection = OUT_LightPosition[0]; //normalize(OUT_LightPosition[0] - OUT_Position);
        //fLightDistance = length(OUT_Position - OUT_LightPosition[i]);
        
        //vTemp.x = dot(vLightDirection, vTangent);
        //vTemp.y = dot(vLightDirection, vBinormal);
        //vTemp.z = dot(vLightDirection, vNormal);
        //vLightDirection = normalize(vTemp);
        
        //fLightAttitude = fLightConst + fLightLinear * fLightDistance + fLightExponential * fLightDistance * fLightDistance;
    fDiffuseFactor = max(dot(vNormalColor, OUT_LightDirection[0]), 0.0); // fLightAttitude;
    fSpecularFactor = pow(max(dot(OUT_LightDirection[0], vReflect), 0.0), 16.0);
    
    fDiffuseColor = fDiffuseColor * fDiffuseFactor + vSpecularColor * fSpecularFactor;
    //}
    
    vColor = fDiffuseColor + vColor * 0.25;
    gl_FragColor = vColor;
}

