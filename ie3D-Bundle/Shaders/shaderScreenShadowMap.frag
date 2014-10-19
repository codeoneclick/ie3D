varying highp vec4 OUT_Position;

highp vec4 compressInTo32RGBA(highp float fDepth)
{
    
    const highp vec4 vBitShift = vec4(256.0 * 256.0 * 256.0,
                                      256.0 * 256.0,
                                      256.0,
                                      1.0);
    
    const highp vec4 vBitMask = vec4(0.0,
                                     1.0 / 256.0,
                                     1.0 / 256.0,
                                     1.0 / 256.0);
    
    highp vec4 vCompressedColor = vec4(fract(fDepth * vBitShift));
    vCompressedColor -= vCompressedColor.xxyz * vBitMask;
    return vCompressedColor;
}

void main(void)
{
    highp float fNormalizedDistance  = OUT_Position.z / OUT_Position.w;
    fNormalizedDistance = (fNormalizedDistance + 1.0) / 2.0;
    
    // bias (to remove artifacts)
    fNormalizedDistance += 0.0005;
    
    //pack value into 32-bit RGBA texture
    gl_FragColor = vec4(1.0);//compressInTo32RGBA(fNormalizedDistance);
}
