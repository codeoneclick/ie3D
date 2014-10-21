varying vec2 OUT_TexCoord;

uniform sampler2D  SAMPLER_01;
uniform sampler2D  SAMPLER_02;
uniform sampler2D  SAMPLER_03;
uniform sampler2D  SAMPLER_04;
uniform vec3 randomTable[8];

const vec2 camerarange = vec2(0.01, 1024.0);
float pw = 1.0 / 1024.0 * 0.5;
float ph = 1.0 / 768.0 * 0.5;

float getDepth(in vec2 texCoord)
{
    if (texCoord.x < 0.0 || texCoord.y < 0.0)
    {
        return 1.0;
    }
    float nearZ = camerarange.x;
    float farZ = camerarange.y;
    float depth = texture2D(SAMPLER_02, texCoord).x;
    return (2.0 * nearZ) / (nearZ + farZ - depth * (farZ - nearZ));
}

vec3 getNormal(in vec2 texCoord)
{
    return normalize(2.0 * texture2D(SAMPLER_04, texCoord).xyz - 1.0);
}

float compareDepths(in float depth1, in float depth2)
{
    float gauss = 0.0;
    float depthDifference = (depth1 - depth2) * 100.0; //depth difference (0 - 100)
    float gaussDisplace = 0.2;
    float gaussArea = 2.0;
    if (depthDifference < gaussDisplace)
    {
        gaussArea = 0.5;
    }
    gauss = pow(2.7182, -2.0 * (depthDifference - gaussDisplace) * (depthDifference - gaussDisplace) / (gaussArea * gaussArea));
    return gauss;
}

float getAmbientOcclusion(float depth, float dw, float dh, vec3 normal)
{
    float coordw = OUT_TexCoord.x + dw / depth;
    float coordh = OUT_TexCoord.y + dh / depth;
    
    if (coordw  < 1.0 && coordw  > 0.0 && coordh < 1.0 && coordh  > 0.0)
    {
     	vec2 coord = vec2(coordw , coordh);
     	return compareDepths(depth, getDepth(coord)) * (1.0 - abs(dot(normal, getNormal(coord))));
    }
    return 0.0;
}

void main(void)
{
    vec3 random = texture2D(SAMPLER_03, OUT_TexCoord * vec2(8.0, 8.0)).xyz;
    vec3 normal = getNormal(OUT_TexCoord);
    random = random * 2.0 -  vec3(1.0);
    
    float depth = getDepth(OUT_TexCoord);
    float ambientOcclusion = 0.0;
    
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

