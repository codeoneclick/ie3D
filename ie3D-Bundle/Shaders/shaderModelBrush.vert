
#if defined(__OPENGL_30__)

out vec4 OUT_Color;

#else

varying vec4 OUT_Color;

#endif

uniform mat4 u_matrixM;
uniform mat4 u_matrixV;
uniform mat4 u_matrixP;

void main(void)
{
    vec4 vPosition = vec4(IN_Position, 1.0);
    OUT_Color = IN_Color / 255.0;
    gl_Position = u_matrixP * u_matrixV * u_matrixM * vPosition;
}