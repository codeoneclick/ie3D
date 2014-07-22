varying highp vec2 OUT_TexCoord;
varying highp vec2 OUT_Position;

uniform highp vec2 IN_Center;
uniform highp float IN_Radius;

uniform sampler2D SAMPLER_01;

void main(void)
{
    highp float distance = length(IN_Center - OUT_Position);
    gl_FragColor = vec4(0.0, 1.0, 0.0, clamp(IN_Radius - distance, 0.0, 0.5));
}