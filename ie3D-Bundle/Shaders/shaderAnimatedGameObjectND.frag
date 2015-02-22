
#if defined(__OPENGL_30__)

in vec4 OUT_Position;

#else

varying vec4 OUT_Position;

#endif

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
    float fZ = OUT_Position.z / OUT_Position.w;
    gl_FragColor = vec4(1.0);
}
