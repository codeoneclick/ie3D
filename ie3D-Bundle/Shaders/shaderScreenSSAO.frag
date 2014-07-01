varying highp vec2 OUT_TexCoord;

uniform sampler2D  SAMPLER_01;
uniform sampler2D  SAMPLER_02;
uniform sampler2D  SAMPLER_03;
uniform sampler2D  SAMPLER_04;
uniform highp vec3 randomTable[8];

const highp vec2 camerarange = vec2(0.01, 1024.0);
highp float pw = 1.0 / 1024.0 * 0.5;
highp float ph = 1.0 / 768.0 * 0.5;

highp float getDepth(in mediump vec2 texCoord)
{
    if (texCoord.x < 0.0 || texCoord.y < 0.0)
    {
        return 1.0;
    }
    highp float nearZ = camerarange.x;
    highp float farZ = camerarange.y;
    lowp float depth = texture2D(SAMPLER_02, texCoord).x;
    return (2.0 * nearZ) / (nearZ + farZ - depth * (farZ - nearZ));
}

highp vec3 getNormal(in mediump vec2 texCoord)
{
    return normalize(2.0 * texture2D(SAMPLER_04, texCoord).xyz - 1.0);
}

highp float compareDepths(in highp float depth1, in highp float depth2)
{
    highp float gauss = 0.0;
    highp float depthDifference = (depth1 - depth2) * 100.0; //depth difference (0 - 100)
    highp float gaussDisplace = 0.2;
    highp float gaussArea = 2.0;
    if (depthDifference < gaussDisplace)
    {
        gaussArea = 0.5;
    }
    gauss = pow(2.7182, -2.0 * (depthDifference - gaussDisplace) * (depthDifference - gaussDisplace) / (gaussArea * gaussArea));
    return gauss;
}

highp float getAmbientOcclusion(highp float depth, highp float dw, highp float dh, highp vec3 normal)
{
    highp float coordw = OUT_TexCoord.x + dw / depth;
    highp float coordh = OUT_TexCoord.y + dh / depth;
    
    if (coordw  < 1.0 && coordw  > 0.0 && coordh < 1.0 && coordh  > 0.0)
    {
     	highp vec2 coord = vec2(coordw , coordh);
     	return compareDepths(depth, getDepth(coord)) * (1.0 - abs(dot(normal, getNormal(coord))));
    }
    return 0.0;
}

void main(void)
{
    highp vec3 random = texture2D(SAMPLER_03, OUT_TexCoord * vec2(8.0, 8.0)).xyz;
    highp vec3 normal = getNormal(OUT_TexCoord);
    random = random * 2.0 -  vec3(1.0);
    
    highp float depth = getDepth(OUT_TexCoord);
    highp float ambientOcclusion = 0.0;
    
    for(int i = 0; i < 4; ++i)
    {
        ambientOcclusion += getAmbientOcclusion(depth,  pw,  ph, normal);
        ambientOcclusion += getAmbientOcclusion(depth,  pw, -ph, normal);
        ambientOcclusion += getAmbientOcclusion(depth, -pw,  ph, normal);
        ambientOcclusion += getAmbientOcclusion(depth, -pw, -ph, normal);
        
        pw += random.x * 0.001;
        ph += random.y * 0.001;
    }
    ambientOcclusion = pow(1.0 - ambientOcclusion / 16.0, 2.0);
    gl_FragColor = vec4(ambientOcclusion, ambientOcclusion, ambientOcclusion, 1.0) * texture2D(SAMPLER_01, OUT_TexCoord);
}

