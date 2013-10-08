attribute vec3 IN_Position;
attribute vec2 IN_TexCoord;
attribute vec4 IN_Normal;
attribute vec4 IN_Tangent;
attribute vec4 IN_Color;

uniform mat4   MATRIX_Projection;
uniform mat4   MATRIX_View;
uniform mat4   MATRIX_Normal;
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

varying vec3   OUT_LightDirection;
varying vec3   OUT_LightPosition;
varying vec3   OUT_CameraDirection;
varying vec3   OUT_CameraPosition;
varying vec3   OUT_Position;
varying vec4   OUT_TexCoordProj;
varying vec2   OUT_TexCoord;

/*const vec3 k_vBinormal = vec3(1.0, 0.0, 0.0);
const vec3 k_vTangent = vec3(0.0, 0.0, 1.0);
const vec3 k_vNormal = vec3(0.0, 1.0, 0.0);*/

void main(void)
{
    vec4 vPosition = MATRIX_World * vec4(IN_Position, 1.0);
    gl_Position = MATRIX_Projection * MATRIX_View * vPosition;
    OUT_TexCoord = (IN_TexCoord / 32767.0  - 1.0) * 4.0;
    OUT_TexCoordProj = gl_Position;
    
    /*vec3 vCameraDirection = normalize(vPosition.xyz - VECTOR_CameraPosition);
    vec3 vTemp;
    vTemp.x = dot(vCameraDirection, k_vTangent);
    vTemp.y = dot(vCameraDirection, k_vBinormal);
    vTemp.z = dot(vCameraDirection, k_vNormal);
    vCameraDirection = normalize(vTemp);
    OUT_CameraDirection = vCameraDirection;
    
    vec3 vLightDirection = normalize(VECTOR_LightPosition_01 - vPosition.xyz);
    vTemp.x = dot(vLightDirection, k_vTangent);
    vTemp.y = dot(vLightDirection, k_vBinormal);
    vTemp.z = dot(vLightDirection, k_vNormal);
    vLightDirection = normalize(vTemp);
    OUT_LightDirection = vLightDirection;*/
    
    OUT_LightPosition = VECTOR_LightPosition_01;
    OUT_CameraPosition = VECTOR_CameraPosition;
    OUT_Position = vPosition.xyz;
}

