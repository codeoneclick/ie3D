
#if defined(__OPENGL_30__)

out vec2   OUT_TexCoord;
out vec4   OUT_TillingTexcoordLayer_01;
out vec4   OUT_TillingTexcoordLayer_02;
out vec4   OUT_TillingTexcoordLayer_03;
//out vec3   OUT_CameraPosition;
//out vec3   OUT_LightDirection;
out vec3   OUT_Normal;
out float  OUT_Fog;
out vec4   OUT_ShadowParameters;

out vec3 v_positionWS;
out mat3 v_tangentMatrix;

#else

varying vec2   OUT_TexCoord;
varying vec4   OUT_TillingTexcoordLayer_01;
varying vec4   OUT_TillingTexcoordLayer_02;
varying vec4   OUT_TillingTexcoordLayer_03;
//varying vec3   OUT_CameraPosition;
//varying vec3   OUT_LightDirection;
varying vec3   OUT_Normal;
varying float  OUT_Fog;
varying vec4   OUT_ShadowParameters;

varying vec3 v_positionWS;
varying mat3 v_tangentMatrix;

#endif

uniform mat4 u_matrixP;
uniform mat4 u_matrixV;

uniform mat4 MATRIX_GlobalLightProjection;
uniform mat4 MATRIX_GlobalLightView;
uniform vec4 VECTOR_ClipPlane;

uniform float IN_TillingTexcoordLayer_01;
uniform float IN_TillingTexcoordLayer_02;
uniform float IN_TillingTexcoordLayer_03;

uniform float IN_fogLinearStart;
uniform float IN_fogLinearEnd;

const mat4 mBiasMatrix = mat4(0.5, 0.0, 0.0, 0.0,
                              0.0, 0.5, 0.0, 0.0,
                              0.0, 0.0, 0.5, 0.0,
                              0.5, 0.5, 0.5, 1.0);

void main(void)
{
    vec4 vertexPosWS = vec4(IN_Position, 1.0);
    gl_Position = u_matrixP * u_matrixV * vertexPosWS;
    gl_ClipDistance[0] = dot(vertexPosWS.xyz, VECTOR_ClipPlane.xyz);
    
    OUT_ShadowParameters = mBiasMatrix * MATRIX_GlobalLightProjection * MATRIX_GlobalLightView * vertexPosWS;
    
    OUT_TexCoord = IN_TexCoord;
    //OUT_CameraPosition = VECTOR_CameraPosition;
    
    OUT_Normal = IN_Tangent.xyz;
    
    //vec3 vLightDirection = VECTOR_GlobalLightPosition - vertexPosWS.xyz;
    //OUT_LightDirection = normalize(vLightDirection);
    
    OUT_Fog = clamp(((u_matrixV * vertexPosWS).z + IN_fogLinearStart) / (IN_fogLinearStart - IN_fogLinearEnd) * -1.0, 0.0, 1.0);
    
#if defined(__OSX__) || (defined(__IOS__) && defined(__IOS_HIGH_PERFORMANCE__))
    OUT_TillingTexcoordLayer_01 = vertexPosWS / IN_TillingTexcoordLayer_01;
    OUT_TillingTexcoordLayer_02 = vertexPosWS / IN_TillingTexcoordLayer_02;
    OUT_TillingTexcoordLayer_03 = vertexPosWS / IN_TillingTexcoordLayer_03;
#elif defined(__IOS__)
    
#else
    OUT_TillingTexcoordLayer_01 = vec4(OUT_TexCoord * IN_TillingTexcoordLayer_01, 0.0, 0.0);
    OUT_TillingTexcoordLayer_02 = vec4(OUT_TexCoord * IN_TillingTexcoordLayer_02, 0.0, 0.0);
    OUT_TillingTexcoordLayer_03 = vec4(OUT_TexCoord * IN_TillingTexcoordLayer_03, 0.0, 0.0);
#endif
    
    vec3 normal = IN_Normal.xyz;
    vec3 tangent = IN_Tangent.xyz;
    vec3 bitangent = cross(normal, tangent);
    
    v_tangentMatrix = mat3(tangent, bitangent, normal);
    
    v_positionWS = vertexPosWS.xyz;
    
    //v_eyeDirTS = matrixTangent * (VECTOR_CameraPosition - vertexPosWS.xyz);
    //v_lightDirTS = matrixTangent * (VECTOR_GlobalLightPosition - vertexPosWS.xyz);
}