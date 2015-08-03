#if defined(__OPENGL_30__)

in vec3 OUT_TexCoord;
in vec4 OUT_ShadowParameters;

#else

varying vec3 OUT_TexCoord;
varying vec4 OUT_ShadowParameters;

#endif

uniform samplerCube SAMPLER_01;
uniform sampler2D SAMPLER_02;

const vec2 vCameraRange = vec2(0.01, 1024.0);

float getShadowMapPassDepth(in vec2 vTexCoord)
{
    float fNearZ = vCameraRange.x;
    float fFarZ = vCameraRange.y;
    float fDepth = texture2D(SAMPLER_02, vTexCoord).x;
    return (2.0 * fNearZ) / (fNearZ + fFarZ - fDepth * (fFarZ - fNearZ));
}

float getCurrentDepth(in  float fZ)
{
    float fDepth = fZ;
    float fNearZ = vCameraRange.x;
    float fFarZ = vCameraRange.y;
    fDepth = (2.0 * fNearZ) / (fNearZ + fFarZ - fDepth * (fFarZ - fNearZ));
    return fDepth;
}

void main(void)
{
    vec2 vTexCoord = OUT_ShadowParameters.st / OUT_ShadowParameters.w;
    float fZ = OUT_ShadowParameters.z / OUT_ShadowParameters.w;
    float fShadow = max(step(getCurrentDepth(fZ), getShadowMapPassDepth(vTexCoord)), 0.5);
    vec4 color = textureCube(SAMPLER_01, OUT_TexCoord);
    color.rgb *= fShadow;
    gl_FragColor = color;
}