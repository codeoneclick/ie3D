#if defined(__OPENGL_30__)

out vec2   OUT_TexCoord;
out vec4   OUT_ShadowParameters;

#if defined PP_PARALLAX_MAPPING

out vec3 v_positionWS;
out mat3 v_tangentMatrix;

#else

out vec3 v_eyeDirTS;
out vec3 v_lightDirTS;

#endif

#else

varying vec2   OUT_TexCoord;
varying vec4   OUT_ShadowParameters;

#if defined PP_PARALLAX_MAPPING

varying vec3 v_positionWS;
varying mat3 v_tangentMatrix;

#else

varying vec3 v_eyeDirTS;
varying vec3 v_lightDirTS;

#endif

#endif

uniform mat4 u_matrixP;
uniform mat4 u_matrixV;
uniform mat4 u_matrixM;

uniform mat4 MATRIX_GlobalLightProjection;
uniform mat4 MATRIX_GlobalLightView;
uniform vec4 VECTOR_ClipPlane;

uniform float IN_fogLinearStart;
uniform float IN_fogLinearEnd;

#if !defined PP_PARALLAX_MAPPING

uniform vec3 VECTOR_CameraPosition;
uniform vec3 VECTOR_GlobalLightPosition;

#endif

const mat4 mBiasMatrix = mat4(0.5, 0.0, 0.0, 0.0,
                              0.0, 0.5, 0.0, 0.0,
                              0.0, 0.0, 0.5, 0.0,
                              0.5, 0.5, 0.5, 1.0);

void main(void)
{
    vec4 vertexPosWS = u_matrixM * vec4(IN_Position, 1.0);
    gl_Position = u_matrixP * u_matrixV * vertexPosWS;
    gl_ClipDistance[0] = dot(vertexPosWS.xyz, VECTOR_ClipPlane.xyz);
    
    OUT_ShadowParameters = mBiasMatrix * MATRIX_GlobalLightProjection * MATRIX_GlobalLightView * vertexPosWS;
    OUT_TexCoord = IN_TexCoord;
    
    highp vec3 normal = normalize(IN_Normal.xyz);
    highp vec3 tangent = normalize(IN_Tangent.xyz);
    highp vec3 bitangent = cross(-normal, tangent);
    
#if defined PP_PARALLAX_MAPPING
    
    v_tangentMatrix = mat3(tangent, bitangent, normal);
    v_positionWS = vertexPosWS.xyz;
    
#else
    
    mat3 tangentMatrix = mat3(tangent, bitangent, normal);
    
    v_eyeDirTS = normalize((VECTOR_CameraPosition - vertexPosWS.xyz) * tangentMatrix);
    v_lightDirTS = normalize((VECTOR_GlobalLightPosition - vertexPosWS.xyz) * tangentMatrix);
    
#endif
}