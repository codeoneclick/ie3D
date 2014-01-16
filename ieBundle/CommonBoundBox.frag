varying lowp  vec4   OUT_LightPosition;
varying lowp  vec3   OUT_Normal;
varying highp vec2   OUT_TexCoord;
varying lowp  float  OUT_ClipPlane;

uniform sampler2D SAMPLER_01;

void main(void)
{
    gl_FragColor = vec4(0.0, 1.0, 0.0, 0.25);
}