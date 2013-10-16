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
uniform float  FLOAT_Timer;

varying vec3   OUT_LightDirection;
varying vec3   OUT_LightPosition;
varying vec3   OUT_CameraDirection;
varying vec3   OUT_CameraPosition;
varying vec3   OUT_Position;
varying vec4   OUT_TexCoordProjection;
varying vec2   OUT_TexCoordDisplace_01;
varying vec2   OUT_TexCoordDisplace_02;

const float k_fTexCoordScale = 4.0;

void main(void)
{
    vec4 vPosition = MATRIX_World * vec4(IN_Position, 1.0);
    gl_Position = MATRIX_Projection * MATRIX_View * vPosition;
    
    
    vec2 vTexCoord = (IN_TexCoord / 32767.0  - 1.0) * k_fTexCoordScale;
    OUT_TexCoordDisplace_01 = vec2(vTexCoord.x + sin(FLOAT_Timer),
                                   vTexCoord.y - cos(FLOAT_Timer));
	
	OUT_TexCoordDisplace_02 = vec2(vTexCoord.x - sin(FLOAT_Timer),
                                   vTexCoord.y + cos(FLOAT_Timer));
    OUT_TexCoordProjection = gl_Position;
    
    OUT_LightPosition = VECTOR_LightPosition_01;
    OUT_CameraPosition = VECTOR_CameraPosition;
    OUT_Position = vPosition.xyz;
}
