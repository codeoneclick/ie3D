attribute vec3 IN_Position;
attribute vec2 IN_TexCoord;
attribute vec4 IN_Normal;
attribute vec4 IN_Tangent;
attribute vec4 IN_Color;

uniform mat4   MATRIX_Projection;
uniform mat4   MATRIX_View;
uniform mat4   MATRIX_World;
uniform mat4   MATRIX_GlobalLightProjection;
uniform mat4   MATRIX_GlobalLightView;

varying vec4 OUT_Position;
                                       
void main(void)
{
    OUT_Position = MATRIX_World * vec4(IN_Position, 1.0);
    OUT_Position = MATRIX_GlobalLightProjection * MATRIX_GlobalLightView * OUT_Position;
    gl_Position = OUT_Position;
}
