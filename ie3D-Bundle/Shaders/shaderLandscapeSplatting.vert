
#if defined(__OPENGL_30__)

out vec2   OUT_TexCoord;
out vec2   OUT_TexCoordScaled;

#else

varying vec2   OUT_TexCoord;
varying vec2   OUT_TexCoordScaled;

#endif

uniform mat4   MATRIX_Projection;
uniform mat4   MATRIX_View;
uniform mat4   MATRIX_World;

const float k_fTexCoordScale = 16.0;
                                       
void main(void)
{
    OUT_TexCoord = IN_TexCoord;
    OUT_TexCoordScaled = OUT_TexCoord * k_fTexCoordScale;
    gl_Position = vec4(IN_Position, 1.0);
}
