
attribute vec3 IN_Position;
attribute vec2 IN_TexCoord;
attribute vec4 IN_Normal;
attribute vec4 IN_Tangent;
attribute vec4 IN_Color;

uniform mat4   MATRIX_Projection;
uniform mat4   MATRIX_View;
uniform mat4   MATRIX_World;

uniform vec3   VECTOR_CameraPosition;
uniform vec3   VECTOR_LightPosition;
uniform vec4   VECTOR_ClipPlane;

varying vec4   OUT_LightPosition;
varying vec3   OUT_Normal;
varying vec2   OUT_TexCoord;
varying float  OUT_ClipPlane;

void main(void)
{
    vec4 vPosition = MATRIX_World * vec4(IN_Position, 1.0);
    gl_Position = MATRIX_Projection * MATRIX_View * vPosition;

    OUT_Normal = IN_Normal.xyz / 127.0 - 1.0;

    vec3 vLightDistance = VECTOR_LightPosition - vec3(vPosition);
    OUT_LightPosition.xyz = normalize(vLightDistance);
    OUT_LightPosition.w = 1.0 - dot(vLightDistance / 16.0, vLightDistance / 16.0);
    OUT_TexCoord = IN_TexCoord;
    OUT_ClipPlane = dot(vec3(vPosition), VECTOR_ClipPlane.xyz) + VECTOR_ClipPlane.w;
}