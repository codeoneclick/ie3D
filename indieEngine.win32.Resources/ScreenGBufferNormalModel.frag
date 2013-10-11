varying lowp  vec3   OUT_LightPosition;
varying lowp  vec3   OUT_Normal;
varying highp vec2   OUT_TexCoord;
varying lowp  float  OUT_ClipPlane;
varying highp vec4   OUT_Position;
varying highp float  OUT_Depth;

uniform sampler2D SAMPLER_01;

void main(void)
{
    if(OUT_ClipPlane < 0.0)
        discard;
    gl_FragColor = vec4(0.5 * OUT_Normal + vec3(0.5), OUT_Depth);
}