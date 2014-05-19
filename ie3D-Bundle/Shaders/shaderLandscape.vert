
attribute vec3 IN_Position;
attribute vec2 IN_TexCoord;
attribute vec4 IN_Normal;
attribute vec4 IN_Tangent;
attribute vec4 IN_Color;
attribute vec4 IN_Extra;

uniform mat4   MATRIX_Projection;
uniform mat4   MATRIX_View;
uniform mat4   MATRIX_World;
uniform vec4   VECTOR_ClipPlane;
uniform vec3   VECTOR_CameraPosition;

varying vec2   OUT_TexCoord;
varying float  OUT_ClipPlane;
varying vec3   OUT_CameraPosition;
varying vec3   OUT_LightDirection;
varying vec3   OUT_Position;
varying vec3   OUT_Normal;

void main(void)
{
    vec4 vPosition = MATRIX_World * vec4(IN_Position, 1.0);
    gl_Position = MATRIX_Projection * MATRIX_View * vPosition;
    OUT_TexCoord = (IN_TexCoord / 32767.0  - 1.0);
    OUT_ClipPlane = dot(vPosition.xyz, VECTOR_ClipPlane.xyz) + VECTOR_ClipPlane.w;
    OUT_CameraPosition = VECTOR_CameraPosition;
    OUT_Position = vPosition.xyz;
    
    vec3 vNormal = IN_Normal.xyz / 127.0 - 1.0;
    vec3 vTangent = IN_Tangent.xyz / 127.0 - 1.0;
    vec3 vBinormal = cross(vNormal, vTangent);
    
    mat3 mTangentSpace = mat3(vTangent.x, vBinormal.x, vNormal.x,
                              vTangent.y, vBinormal.y, vNormal.y,
                              vTangent.z, vBinormal.z, vNormal.z);
    
    vec3 vLightDirection = vec3(vPosition) - vec3(512.0, 1024.0, 64.0);
    OUT_Normal = vNormal;
    OUT_LightDirection = normalize(mTangentSpace * vLightDirection);
}