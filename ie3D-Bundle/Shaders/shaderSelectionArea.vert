
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

void main(void)
{
    vec4 vPosition = vec4(IN_Position, 1.0);
    gl_Position = MATRIX_Projection * MATRIX_View * vPosition;
}