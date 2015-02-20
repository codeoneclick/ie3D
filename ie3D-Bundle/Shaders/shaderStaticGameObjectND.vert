
#if defined(__OPENGL_30__)

out vec4 OUT_Position;

#else

varying vec4 OUT_Position;

#endif

uniform mat4 u_matrixM;
uniform mat4 MATRIX_GlobalLightProjection;
uniform mat4 MATRIX_GlobalLightView;

void main(void)
{
    OUT_Position = u_matrixM * vec4(IN_Position, 1.0);
    OUT_Position = MATRIX_GlobalLightProjection * MATRIX_GlobalLightView * OUT_Position;
    gl_Position = OUT_Position;
}
