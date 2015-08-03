
#if defined(__OPENGL_30__)

in vec4 OUT_Position;
in vec2 OUT_TexCoord;

#else

varying vec4 OUT_Position;
varying vec2 OUT_TexCoord;

#endif

uniform sampler2D SAMPLER_01;

const vec2 vCameraRange = vec2(0.01, 1024.0);

float getDepth(in float fZ)
{
    float fDepth = fZ;
    float fNearZ = vCameraRange.x;
    float fFarZ = vCameraRange.y;
    fDepth = (2.0 * fNearZ) / (fNearZ + fFarZ - fDepth * (fFarZ - fNearZ));
    return fDepth;
}
void main(void)
{
    //float fZ = OUT_Position.z / OUT_Position.w;
    //vec4 color = texture2D(SAMPLER_01, OUT_TexCoord)
    //gl_FragColor = vec4(vec3(getDepth(fZ) * vCameraRange.y), 1.0);
    gl_FragColor = vec4(1.0);
}
