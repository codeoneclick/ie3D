attribute vec3 IN_Position;
attribute vec2 IN_TexCoord;
attribute vec4 IN_Normal;
attribute vec4 IN_Tangent;
attribute vec4 IN_Color;

uniform mat4   MATRIX_Projection;
uniform mat4   MATRIX_View;
uniform mat4   MATRIX_World;

uniform vec3   VECTOR_CameraPosition;
uniform vec3   VECTOR_LightPosition_01;
uniform vec3   VECTOR_LightPosition_02;
uniform vec3   VECTOR_LightPosition_03;
uniform vec3   VECTOR_LightPosition_04;
uniform vec3   VECTOR_LightColor_01;
uniform vec3   VECTOR_LightColor_02;
uniform vec3   VECTOR_LightColor_03;
uniform vec3   VECTOR_LightColor_04;
uniform vec4   VECTOR_ClipPlane;
uniform int    INT_LightsCount;

varying vec3   OUT_LightDirection[4];
varying vec3   OUT_CameraDirection;
varying vec3   OUT_Position;
varying vec4   OUT_TexCoordProj;
varying vec2   OUT_TexCoord;
varying float  OUT_Distance;

const vec3 vBinormal = vec3(1.0, 0.0, 0.0);
const vec3 vTangent = vec3(0.0, 0.0, 1.0);
const vec3 vNormal = vec3(0.0, 1.0, 0.0);

void main(void)
{
    vec4 vPosition = MATRIX_World * vec4(IN_Position, 1.0);
    gl_Position = MATRIX_Projection * MATRIX_View * vPosition;
    OUT_TexCoord = (IN_TexCoord / 32767.0  - 1.0) * 4.0;
    OUT_TexCoordProj = gl_Position;
    
    vec3 vCameraDirection = normalize(vPosition.xyz - VECTOR_CameraPosition);
    vec3 vTemp;
    vTemp.x = dot(vCameraDirection, vTangent);
    vTemp.y = dot(vCameraDirection, vBinormal);
    vTemp.z = dot(vCameraDirection, vNormal);
    OUT_CameraDirection = vTemp;
    
    vec3 vLightDirection[4];
    vLightDirection[0] = normalize(VECTOR_LightPosition_01 - vPosition.xyz);
    vLightDirection[1] = normalize(VECTOR_LightPosition_02 - vPosition.xyz);
    vLightDirection[2] = normalize(VECTOR_LightPosition_03 - vPosition.xyz);
    vLightDirection[3] = normalize(VECTOR_LightPosition_04 - vPosition.xyz);
    
    for(int i = 0; i < INT_LightsCount; i++)
    {
        vTemp.x = dot(vLightDirection[i], vTangent);
        vTemp.y = dot(vLightDirection[i], vBinormal);
        vTemp.z = dot(vLightDirection[i], vNormal);
        OUT_LightDirection[i] = vTemp;
    }
    
    OUT_Position = vPosition.xyz;
}

