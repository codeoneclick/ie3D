
#if defined(__OPENGL_30__)

out vec3 OUT_TexCoord;

#else

varying vec3 OUT_TexCoord;

#endif

uniform mat4 MATRIX_Projection;
uniform mat4 MATRIX_View;
uniform mat4 MATRIX_Normal;
uniform mat4 MATRIX_World;

void main(void)
{
    vec4 vPosition = MATRIX_World * vec4(IN_Position, 1.0);
    gl_Position = MATRIX_Projection * MATRIX_View * vPosition;
    OUT_TexCoord = IN_Position;
}