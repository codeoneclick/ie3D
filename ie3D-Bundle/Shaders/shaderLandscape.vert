
attribute vec3 IN_Position;
attribute vec2 IN_TexCoord;
attribute vec4 IN_Normal;
attribute vec4 IN_Tangent;
attribute vec4 IN_Color;
attribute vec4 IN_Extra;

uniform mat4   MATRIX_Projection;
uniform mat4   MATRIX_View;
uniform mat4   MATRIX_World;
uniform vec4   VECTOR_ClipPlane;
uniform vec3   VECTOR_CameraPosition;
uniform vec3   VECTOR_LightPosition_01;

varying vec2   OUT_TexCoord;
varying vec4   OUT_TillingTexcoordLayer_01;
varying vec4   OUT_TillingTexcoordLayer_02;
varying vec4   OUT_TillingTexcoordLayer_03;
varying float  OUT_ClipPlane;
varying vec3   OUT_CameraPosition;
varying vec3   OUT_LightDirection;
varying vec3   OUT_Position;
varying vec3   OUT_Normal;
varying float  OUT_Fog;

uniform float IN_TillingTexcoordLayer_01;
uniform float IN_TillingTexcoordLayer_02;
uniform float IN_TillingTexcoordLayer_03;

uniform float IN_fogLinearStart;
uniform float IN_fogLinearEnd;

void main(void)
{
    vec4 vPosition = MATRIX_World * vec4(IN_Position, 1.0);
    gl_Position = MATRIX_Projection * MATRIX_View * vPosition;
    OUT_TexCoord = (IN_TexCoord / 32767.0  - 1.0);
    OUT_ClipPlane = dot(vPosition.xyz, VECTOR_ClipPlane.xyz) + VECTOR_ClipPlane.w;
    OUT_CameraPosition = VECTOR_CameraPosition;
    
    OUT_Normal = IN_Normal.xyz / 127.0 - 1.0;

    vec3 vLightDirection = VECTOR_LightPosition_01 - vPosition.xyz;
    OUT_LightDirection = normalize(vLightDirection);
    
    OUT_Fog = clamp(((MATRIX_View * vPosition).z + IN_fogLinearStart) / (IN_fogLinearStart - IN_fogLinearEnd) * -1.0, 0.0, 1.0);
    
#if defined(__OSX__) || (defined(__IOS__) && defined(__IOS_HIGH_PERFORMANCE__))
    OUT_TillingTexcoordLayer_01 = vPosition / IN_TillingTexcoordLayer_01;
    OUT_TillingTexcoordLayer_02 = vPosition / IN_TillingTexcoordLayer_02;
    OUT_TillingTexcoordLayer_03 = vPosition / IN_TillingTexcoordLayer_03;
#elif defined(__IOS__)
    
#else
    OUT_TillingTexcoordLayer_01 = vec4(OUT_TexCoord * IN_TillingTexcoordLayer_01, 0.0, 0.0);
    OUT_TillingTexcoordLayer_02 = vec4(OUT_TexCoord * IN_TillingTexcoordLayer_02, 0.0, 0.0);
    OUT_TillingTexcoordLayer_03 = vec4(OUT_TexCoord * IN_TillingTexcoordLayer_03, 0.0, 0.0);
#endif
}