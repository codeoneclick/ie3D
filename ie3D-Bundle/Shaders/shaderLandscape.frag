
varying highp   vec2   OUT_TexCoord;
varying mediump float  OUT_ClipPlane;
varying mediump vec3   OUT_CameraPosition;
varying mediump vec3   OUT_Position;
varying mediump vec3   OUT_Tangent;
varying mediump vec3   OUT_Normal;
varying mediump vec3   OUT_Binormal;

uniform sampler2D SAMPLER_01;
uniform sampler2D SAMPLER_02;

void main(void)
{
    if(OUT_ClipPlane < 0.0)
        discard;
    
    lowp vec4 vDiffuseColor = texture2D(SAMPLER_01, OUT_TexCoord);
    
    
    mediump vec3 vCameraDirection = normalize(OUT_Position - OUT_CameraPosition);
    
    mediump vec3 vTemp;
    vTemp.x = dot(vCameraDirection, OUT_Tangent);
    vTemp.y = dot(vCameraDirection, OUT_Binormal);
    vTemp.z = dot(vCameraDirection, OUT_Normal);
    vCameraDirection = normalize(vTemp);
    
    mediump vec3 vLightDirection = normalize(vec3(512.0, 1024.0, 64.0) - OUT_Position);
    vTemp.x = dot(vLightDirection, OUT_Tangent);
    vTemp.y = dot(vLightDirection, OUT_Binormal);
    vTemp.z = dot(vLightDirection, OUT_Normal);
    vLightDirection = normalize(vTemp);
    
    gl_FragColor = vDiffuseColor;
}