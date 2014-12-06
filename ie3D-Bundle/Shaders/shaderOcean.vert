
#if defined(__OPENGL_30__)

//out vec3   OUT_LightPosition;
//out vec3   OUT_CameraPosition;
//out vec3 OUT_cameraDirection;
//out vec3 OUT_lightDirection;
//out vec3   OUT_Position;
varying vec3 v_cameraDirTS;
varying vec3 v_cameraDirWS;
varying vec3 v_lightDirTS;
out vec2   OUT_TexCoord;
out vec4   OUT_TexCoordProjection;
out vec2   OUT_TexCoordDisplace_01;
out vec2   OUT_TexCoordDisplace_02;
out vec4   OUT_Extra;

#else

//varying vec3   OUT_LightPosition;
//varying vec3   OUT_CameraPosition;
varying vec3 v_cameraDirWS;
varying vec3 v_cameraDirTS;
varying vec3 v_lightDirWS;
varying vec3 v_lightDirTS;
varying vec3 v_positionWS;
varying float v_fogDistance;
//varying vec3   OUT_Position;
varying vec2   OUT_TexCoord;
varying vec4   OUT_TexCoordProjection;
varying vec2   OUT_TexCoordDisplace_01;
varying vec2   OUT_TexCoordDisplace_02;
varying vec4   OUT_Extra;

#endif

uniform mat4   MATRIX_Projection;
uniform mat4   MATRIX_View;
uniform mat4   MATRIX_Normal;
uniform mat4   MATRIX_World;
uniform mat4 u_matrixViewInverse;

uniform vec3   VECTOR_CameraPosition;
uniform vec3   VECTOR_GlobalLightPosition;
uniform vec4   VECTOR_ClipPlane;
uniform int    INT_LightsCount;
uniform float  FLOAT_Timer;

const float k_fTexCoordScale = 32.0;

const  vec3 k_vBinormal = vec3(1.0, 0.0, 0.0);
const  vec3 k_vTangent = vec3(0.0, 0.0, 1.0);
const  vec3 k_vNormal = vec3(0.0, 1.0, 0.0);

void main(void)
{
    vec4 vPosition = MATRIX_World * vec4(IN_Position, 1.0);
    gl_Position = MATRIX_Projection * MATRIX_View * vPosition;
    
    vec2 vTexCoord = IN_TexCoord;
    OUT_TexCoord = vTexCoord;
    vTexCoord *= k_fTexCoordScale;
    OUT_TexCoordDisplace_01 = vec2(vTexCoord.x + sin(FLOAT_Timer) * 0.25,
                                   vTexCoord.y - cos(FLOAT_Timer) * 0.75);
	
	OUT_TexCoordDisplace_02 = vec2(vTexCoord.x - sin(FLOAT_Timer) * 0.75,
                                   vTexCoord.y + cos(FLOAT_Timer) * 0.25);
    OUT_TexCoordProjection = gl_Position;
    
    mat3 matrixTangent = mat3(k_vTangent,
                              k_vBinormal,
                              k_vNormal);
    
    v_cameraDirWS = VECTOR_CameraPosition;//(u_matrixViewInverse * (vec4(VECTOR_CameraPosition, 1.0) - vPosition)).xyz;
    v_cameraDirTS = (VECTOR_CameraPosition - vPosition.xyz) * matrixTangent;
    v_lightDirWS = VECTOR_GlobalLightPosition;//(u_matrixViewInverse * (vec4(VECTOR_GlobalLightPosition, 1.0) - vPosition)).xyz;
    v_lightDirTS = (VECTOR_GlobalLightPosition - vPosition.xyz) * matrixTangent;
    
    v_positionWS = vPosition.xyz;
    
    v_fogDistance = length(vec3(256.0, 0.0, 256.0) - vPosition.xyz);
    v_fogDistance = clamp((v_fogDistance - 384.0) / 448.0, 0.0, 1.0);
    //OUT_LightPosition = VECTOR_GlobalLightPosition;
    //OUT_Position = vPosition.xyz;
    OUT_Extra = IN_Extra;
}

