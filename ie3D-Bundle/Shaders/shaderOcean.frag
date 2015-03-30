
#if defined(__OPENGL_30__)

in vec2 v_texCoord;
in vec4 v_texCoordProj;
in vec2 v_texCoordDisplace_01;
in vec2 v_texCoordDisplace_02;

in vec3 v_eyePosWS;
in vec3 v_eyeDirTS;

in vec3 v_lightPosWS;
in vec3 v_lightDirTS;

in vec3 v_vertexPosWS;

in float v_fogDistance;

#else

varying vec2 v_texCoord;
varying vec4 v_texCoordProj;
varying vec2 v_texCoordDisplace_01;
varying vec2 v_texCoordDisplace_02;

varying vec3 v_eyePosWS;
varying vec3 v_eyeDirTS;

varying vec3 v_lightPosWS;
varying vec3 v_lightDirTS;

varying vec3 v_vertexPosWS;

varying float v_fogDistance;

#endif

uniform sampler2D SAMPLER_01;
uniform sampler2D SAMPLER_02;
uniform sampler2D SAMPLER_03;
uniform sampler2D SAMPLER_04;

const vec4 k_specularColor = vec4(1.2, 1.0, 0.75, 1.0);
const float k_specularShine = 512.0;
const vec2 k_perturbationFactor = vec2(0.025, 0.025);

const float k_0 = 0.0;
const float k_1 = 1.0;
const float k_2 = 2.0;
const float k_05 = 0.5;
const float k_025 = 0.25;
const float k_033 = 0.33;

const  vec3 k_binormal = vec3(k_1, k_0, k_0);
const  vec3 k_tangent = vec3(k_0, k_0, k_1);
const  vec3 k_normal = vec3(k_0, k_1, k_0);

const vec4 k_waterColorBlue = vec4(0.16, 0.32, 0.32, 1.0);
const vec4 k_waterColorGreen = vec4(0.16, 0.32, 0.16, 1.0);

void main(void)
{
    vec3 eyeDirWS = normalize(v_eyePosWS - v_vertexPosWS);
    vec3 lightDirWS = normalize(v_lightPosWS - v_vertexPosWS);

    vec3 eyeDirTS = normalize(v_eyeDirTS);
    vec3 lightDirTS = normalize(v_lightDirTS);
    
    vec3 normalColor = texture2D(SAMPLER_03, v_texCoordDisplace_01).rgb;
    normalColor += texture2D(SAMPLER_03, v_texCoordDisplace_02).rgb;
    normalColor -= 1.0;
    
    float diffuseIntensity = clamp(dot(normalColor, lightDirTS), k_0, k_1);
    vec3 reflectVector = normalize(k_2 * diffuseIntensity * normalColor - lightDirTS);
    reflectVector.y = max(reflectVector.y , k_0);
    float specularIntensity = clamp(pow(dot(reflectVector, eyeDirTS), k_specularShine) * diffuseIntensity, k_0, k_1);
    
    vec2 texCoordProj = v_texCoordProj.xy;
    texCoordProj = k_05 + k_05 * texCoordProj / v_texCoordProj.w * vec2(-k_1, k_1);
    
    float deep = pow(clamp(1.0 - texture2D(SAMPLER_04, v_texCoord).r, 0.0, 1.0), 2.0);
    vec2 perturbationIntensity = k_perturbationFactor * normalColor.xy * deep;
    vec2 perturbatedTexCoord = texCoordProj + perturbationIntensity;
    
    vec4 reflectionColor = texture2D(SAMPLER_01, perturbatedTexCoord);
    vec4 refractionColor = texture2D(SAMPLER_02, vec2(k_05 + (k_05 - perturbatedTexCoord.x), perturbatedTexCoord.y));
    vec4 diffuseColor = k_waterColorBlue * diffuseIntensity * k_025;
    
    float fresnel = dot(k_normal, eyeDirWS);
    
    diffuseIntensity = clamp(dot(k_normal, lightDirWS), k_025, k_1);
    
    vec4 color = mix(reflectionColor, refractionColor, fresnel);
    color = color * diffuseIntensity + k_specularColor * specularIntensity + diffuseColor;
    color.a = 1.0;
    vec4 fogColor = vec4(0.85, 0.95, 1.0, 1.0);
    float fogDistance = (v_fogDistance - 384.0) / 448.0;
    fogDistance = clamp(fogDistance, 0.0, 1.0);
    color = mix(color, fogColor, fogDistance);
    gl_FragColor = color;
}
