#if defined(__OPENGL_30__)

in vec3 OUT_TexCoord;

#else

varying vec3 OUT_TexCoord;

#endif

uniform samplerCube SAMPLER_01;

void main(void)
{
    vec4 vColor = textureCube(SAMPLER_01, OUT_TexCoord);
    gl_FragColor = vColor;
}