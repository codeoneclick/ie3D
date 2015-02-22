#if defined(__OPENGL_30__)

in vec3 OUT_TexCoord;

#else

varying vec3 OUT_TexCoord;

#endif

uniform samplerCube SAMPLER_01;

void main(void)
{
    vec4 vColor = textureCube(SAMPLER_01, OUT_TexCoord);
    vec4 fogColor = vec4(0.85, 0.95, 1.0, 1.0);
    float height = OUT_TexCoord.y * (1.0 / 0.2);
    float fFogBlend = 1.0 - clamp(height, 0.0, 1.0);
    vColor = mix(vColor, fogColor, fFogBlend);
    gl_FragColor = vColor;
}