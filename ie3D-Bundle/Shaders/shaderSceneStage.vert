
#if defined(__OPENGL_30__)

out vec3 OUT_TexCoord;
out vec4 OUT_ShadowParameters;

#else

varying vec2 OUT_TexCoord;
varying vec4 OUT_ShadowParameters;

#endif

uniform mat4 MATRIX_GlobalLightProjection;
uniform mat4 MATRIX_GlobalLightView;

uniform mat4 u_matrixM;
uniform mat4 u_matrixVP;

const mat4 mBiasMatrix = mat4(0.5, 0.0, 0.0, 0.0,
                              0.0, 0.5, 0.0, 0.0,
                              0.0, 0.0, 0.5, 0.0,
                              0.5, 0.5, 0.5, 1.0);

void main(void)
{
    vec4 vertexPosWS = u_matrixM * vec4(IN_Position, 1.0);
    gl_Position = u_matrixVP * vertexPosWS;
    OUT_ShadowParameters = mBiasMatrix * MATRIX_GlobalLightProjection * MATRIX_GlobalLightView * vertexPosWS;
    OUT_TexCoord = IN_Position;
}