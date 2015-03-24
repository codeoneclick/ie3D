
#if defined(__OPENGL_30__)

out vec2 v_texCoord;
out vec4 v_texCoordProj;
out vec2 v_texCoordDisplace_01;
out vec2 v_texCoordDisplace_02;

out vec3 v_eyePosWS;
out vec3 v_eyeDirTS;

out vec3 v_lightPosWS;
out vec3 v_lightDirTS;

out vec3 v_vertexPosWS;

out float v_fogDistance;

#else

varying vec2 v_texCoord;
varying vec4 v_texCoordProj;
varying vec2 v_texCoordDisplace_01;
varying vec2 v_texCoordDisplace_02;

varying vec3 v_eyePosWS;
varying vec3 v_eyeDirTS;

varying vec3 v_lightPosWS;
varying vec3 v_lightDirTS;

varying vec3 v_vertexPosWS;

varying float v_fogDistance;

#endif

uniform mat4 u_matrixM;
uniform mat4 u_matrixVP;

uniform vec3   VECTOR_CameraPosition;
uniform vec3   VECTOR_GlobalLightPosition;
uniform vec4   VECTOR_ClipPlane;
uniform float  FLOAT_Timer;

const float k_texCoordScale = 24.0;

const  vec3 k_binormal = vec3(1.0, 0.0, 0.0);
const  vec3 k_tangent = vec3(0.0, 0.0, 1.0);
const  vec3 k_normal = vec3(0.0, 1.0, 0.0);

void main(void)
{
    vec4 vertexPosWS = u_matrixM * vec4(IN_Position, 1.0);
    gl_Position = u_matrixVP * vertexPosWS;
    
    v_texCoord = IN_TexCoord;
    vec2 texCoord = IN_TexCoord;
    texCoord *= k_texCoordScale;
    v_texCoordDisplace_01 = vec2(texCoord.x + sin(FLOAT_Timer) * 0.25,
                                 texCoord.y - cos(FLOAT_Timer) * 0.75);
    
    v_texCoordDisplace_02 = vec2(texCoord.x - sin(FLOAT_Timer) * 0.75,
                                 texCoord.y + cos(FLOAT_Timer) * 0.25);
    v_texCoordProj = gl_Position;
    
    mat3 matrixTangent = mat3(k_tangent,
                              k_binormal,
                              k_normal);
    
    v_eyePosWS = VECTOR_CameraPosition;
    v_lightPosWS = VECTOR_GlobalLightPosition;
    
    v_eyeDirTS = (VECTOR_CameraPosition - vertexPosWS.xyz) * matrixTangent;
    v_lightDirTS = (VECTOR_GlobalLightPosition - vertexPosWS.xyz) * matrixTangent;
    
    v_vertexPosWS = vertexPosWS.xyz;
    
    v_fogDistance = length(vec3(256.0, 0.0, 256.0) - vertexPosWS.xyz);
}
