#if defined(__OPENGL_30__)

in vec2 OUT_TexCoord;
in float OUT_ClipPlane;

#else

varying vec2 OUT_TexCoord;
varying float OUT_ClipPlane;

#endif

uniform sampler2D SAMPLER_01;

void main(void)
{
    if(OUT_ClipPlane < 0.0)
        discard;
    
    gl_FragColor = texture2D(SAMPLER_01, OUT_TexCoord);
}