
attribute vec3 IN_Position;
attribute vec2 IN_TexCoord;
attribute vec4 IN_Normal;
attribute vec4 IN_Tangent;
attribute vec4 IN_Color;
attribute vec4 IN_Extra;

uniform mat4   MATRIX_Projection;
uniform mat4   MATRIX_View;
uniform mat4   MATRIX_World;
uniform mat4   MATRIX_Normal;
uniform mat4   MATRIX_Bones[64];

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

uniform int    INT_FLAG_01;
uniform int    INT_FLAG_02;
uniform int    INT_FLAG_03;
uniform int    INT_FLAG_04;

varying vec4   OUT_LightPosition;
varying vec3   OUT_Normal;
varying vec2   OUT_TexCoord;
varying float  OUT_ClipPlane;

void main(void)
{
    vec4 vPosition = vec4(IN_Position, 1.0);
    vec4 vNormal = vec4(IN_Normal.xyz / 127.0 - 1.0, 1.0);
    vec4 vTangent = vec4(IN_Tangent.xyz / 127.0 - 1.0, 1.0);
    vec4 vWeights = IN_Extra / 255.0;
    
    if(INT_FLAG_01 == 1)
    {
        vPosition = MATRIX_World * vPosition;
        vNormal = normalize(MATRIX_Normal * vNormal);
        vTangent = normalize(MATRIX_Normal * vTangent);
    }
    else
    {
        vPosition = MATRIX_World * vPosition;
        vNormal = normalize(MATRIX_Normal * vNormal);
        vTangent = normalize(MATRIX_Normal * vTangent);
    }

    gl_Position = MATRIX_Projection * MATRIX_View * vPosition;
    
    vec3 vLightDistance = VECTOR_LightPosition_01 - vPosition.xyz;
    OUT_LightPosition.xyz = normalize(vLightDistance);
    OUT_LightPosition.w = 1.0 - dot(vLightDistance / 16.0, vLightDistance / 16.0);
    OUT_TexCoord = IN_TexCoord / 32767.0  - 1.0;
    OUT_Normal = vNormal.xyz;
    OUT_ClipPlane = dot(vPosition.xyz, VECTOR_ClipPlane.xyz) + VECTOR_ClipPlane.w;
}