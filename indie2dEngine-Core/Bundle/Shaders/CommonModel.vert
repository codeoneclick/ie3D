
attribute vec3 IN_Position;
attribute vec2 IN_TexCoord;
attribute vec4 IN_Normal;
attribute vec4 IN_Tangent;
attribute vec4 IN_Color;

uniform mat4   MATRIX_Projection;
uniform mat4   MATRIX_View;
uniform mat4   MATRIX_World;
uniform mat4   MATRIX_Bones[64];

uniform vec3   VECTOR_CameraPosition;
uniform vec3   VECTOR_LightPosition;
uniform vec4   VECTOR_ClipPlane;

varying vec4   OUT_LightPosition;
varying vec3   OUT_Normal;
varying vec2   OUT_TexCoord;
varying float  OUT_ClipPlane;

void main(void)
{
    vec4 vPosition = vec4(IN_Position, 1.0);
    vec4 vNormal = vec4(IN_Normal.xyz / 127.0 - 1.0, 1.0);
    vec4 vWeights = IN_Color / 255.0;
    
    int index = int(IN_Tangent.x);
    vec4 vBonePosition = MATRIX_Bones[index] * vPosition * vWeights.x;
    vec4 vBoneNormal = MATRIX_Bones[index] * vNormal * vWeights.x;
    
    index = int(IN_Tangent.y);
    vBonePosition += MATRIX_Bones[index] * vPosition * vWeights.y;
    vBoneNormal += MATRIX_Bones[index] * vNormal * vWeights.y;
    
    index = int(IN_Tangent.z);
    vBonePosition += MATRIX_Bones[index] * vPosition * vWeights.z;
    vBoneNormal += MATRIX_Bones[index] * vNormal * vWeights.z;
    
    index = int(IN_Tangent.w);
    vBonePosition += MATRIX_Bones[index] * vPosition * vWeights.w;
    vBoneNormal += MATRIX_Bones[index] * vNormal * vWeights.w;

    vBonePosition = MATRIX_World * vBonePosition;
    
    gl_Position = MATRIX_Projection * MATRIX_View * vBonePosition;

    OUT_Normal = (vBoneNormal).xyz;
    
    vec3 vLightDistance = VECTOR_LightPosition - vBonePosition.xyz;
    OUT_LightPosition.xyz = normalize(vLightDistance);
    OUT_LightPosition.w = 1.0 - dot(vLightDistance / 16.0, vLightDistance / 16.0);
    OUT_TexCoord = IN_TexCoord / 32767.0  - 1.0;
    OUT_ClipPlane = dot(vBonePosition.xyz, VECTOR_ClipPlane.xyz) + VECTOR_ClipPlane.w;
}