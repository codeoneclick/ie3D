
#if defined(__OPENGL_30__)

out vec4 OUT_Color;

#else

varying vec4 OUT_Color;

#endif

uniform mat4   MATRIX_Projection;
uniform mat4   MATRIX_View;
uniform mat4   MATRIX_World;

void main(void)
{
    vec4 vPosition = vec4(IN_Position, 1.0);
    OUT_Color = IN_Color / 255.0;
    gl_Position = MATRIX_Projection * MATRIX_View * MATRIX_World * vPosition;
}