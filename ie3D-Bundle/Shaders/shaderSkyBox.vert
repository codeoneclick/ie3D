
#if defined(__OPENGL_30__)

out vec3 OUT_TexCoord;

#else

varying vec3 OUT_TexCoord;

#endif

uniform mat4 u_matrixMVP;

void main(void)
{
    vec4 vPosition = vec4(IN_Position, 1.0);
    gl_Position = u_matrixMVP * vPosition;
    OUT_TexCoord = IN_Position;
}