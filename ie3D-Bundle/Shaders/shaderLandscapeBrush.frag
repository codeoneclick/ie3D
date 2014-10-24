#if defined(__OPENGL_30__)

in vec2 OUT_TexCoord;
in vec2 OUT_Position;

#else

varying vec2 OUT_TexCoord;
varying vec2 OUT_Position;

#endif

uniform vec2 IN_Center;
uniform float IN_Radius;

uniform sampler2D SAMPLER_01;

void main(void)
{
    float distance = length(IN_Center - OUT_Position);
    gl_FragColor = vec4(0.0, 1.0, 0.0, clamp(IN_Radius - distance, 0.0, 0.5));
}