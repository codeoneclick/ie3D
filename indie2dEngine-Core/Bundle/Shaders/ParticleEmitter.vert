attribute vec3 IN_Position;
attribute vec2 IN_TexCoord;
attribute vec4 IN_Normal;
attribute vec4 IN_Tangent;
attribute vec4 IN_Color;

uniform mat4 MATRIX_Projection;
uniform mat4 MATRIX_View;
uniform mat4 MATRIX_World;

varying vec2 OUT_TexCoord;
varying vec4 OUT_Color;

void main(void)
{
    gl_Position = MATRIX_Projection * MATRIX_View * vec4(IN_Position, 1.0);
    OUT_TexCoord = IN_TexCoord;
    OUT_Color = IN_Color / 255.0;
}