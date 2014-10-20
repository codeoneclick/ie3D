varying highp vec4 OUT_Position;

const highp vec2 vCameraRange = vec2(0.01, 1024.0);

highp float getDepth(in highp float fZ)
{
    highp float fDepth = fZ;
    highp float fNearZ = vCameraRange.x;
    highp float fFarZ = vCameraRange.y;
    fDepth = (2.0 * fNearZ) / (fNearZ + fFarZ - fDepth * (fFarZ - fNearZ));
    return fDepth;
}

void main(void)
{
    highp float fZ = OUT_Position.z / OUT_Position.w;
    gl_FragColor = vec4(vec3(getDepth(fZ)), 1.0);
}
