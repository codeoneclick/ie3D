attribute vec3 IN_Position;
attribute vec2 IN_TexCoord;
attribute vec4 IN_Normal;
attribute vec4 IN_Tangent;
attribute vec4 IN_Color;

uniform mat4   MATRIX_Projection;
uniform mat4   MATRIX_View;
uniform mat4   MATRIX_World;

varying vec2   OUT_TexCoord;
varying vec2   OUT_BTexCoord[5];
                                       
void main(void)
{
    OUT_TexCoord = IN_TexCoord / 32767.0  - 1.0;
    gl_Position = vec4(IN_Position, 1.0);
    
    vec2 stepTexcoord = vec2(0.01, 0.01);
	OUT_BTexCoord[0] = OUT_TexCoord.xy;
	OUT_BTexCoord[1] = OUT_TexCoord.xy + stepTexcoord;
	OUT_BTexCoord[2] = OUT_TexCoord.xy - stepTexcoord;
	OUT_BTexCoord[3] = OUT_TexCoord.xy + stepTexcoord;
	OUT_BTexCoord[4] = OUT_TexCoord.xy - stepTexcoord;
}
