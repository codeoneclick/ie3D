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

varying vec3   OUT_LightPosition[4];
varying vec3   OUT_CameraPosition;
varying vec3   OUT_Position;
varying vec4   OUT_TexCoordProj;
varying vec2   OUT_TexCoord;
varying float  OUT_Distance;

void main(void)
{
    vec4 vPosition = MATRIX_World * vec4(IN_Position, 1.0);
    gl_Position = MATRIX_Projection * MATRIX_View * vPosition;
    OUT_TexCoord = (IN_TexCoord / 32767.0  - 1.0) * 4.0;
    OUT_TexCoordProj = gl_Position;
    
    OUT_LightPosition[0] = VECTOR_LightPosition_01;
    OUT_LightPosition[1] = VECTOR_LightPosition_02;
    OUT_LightPosition[2] = VECTOR_LightPosition_03;
    OUT_LightPosition[3] = VECTOR_LightPosition_04;
    
    OUT_CameraPosition = VECTOR_CameraPosition;
    OUT_Position = vPosition.xyz;
}

