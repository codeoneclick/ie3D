
#if defined(__OPENGL_30__)

out vec2 OUT_TexCoord;
out vec4 OUT_Color;

#else

varying vec2 OUT_TexCoord;
varying vec4 OUT_Color;

#endif

uniform mat4 MATRIX_Projection;
uniform mat4 MATRIX_View;
uniform mat4 MATRIX_World;

void main(void)
{
    gl_Position = MATRIX_Projection * MATRIX_View * vec4(IN_Position, 1.0);
    OUT_TexCoord = IN_TexCoord;
    OUT_Color = IN_Color;
}