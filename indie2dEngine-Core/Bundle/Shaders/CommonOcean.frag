varying highp vec4   OUT_TexCoordProj;
varying highp vec2   OUT_TexCoord;
uniform highp float  OUT_Timer;

uniform sampler2D SAMPLER_01;
uniform sampler2D SAMPLER_02;
uniform sampler2D SAMPLER_03;

void main(void)
{
    highp vec2 vTexCoordProj = OUT_TexCoordProj.xy;
	vTexCoordProj.x = 0.5 - 0.5 * vTexCoordProj.x / OUT_TexCoordProj.w;
	vTexCoordProj.y = 0.5 + 0.5 * vTexCoordProj.y / OUT_TexCoordProj.w;
	vTexCoordProj = clamp(vTexCoordProj, 0.001, 0.999);
    lowp vec4 vReflectionColor = texture2D(SAMPLER_01, vTexCoordProj);
    
    vTexCoordProj = OUT_TexCoordProj.xy;
    vTexCoordProj.x = 0.5 + 0.5 * vTexCoordProj.x / OUT_TexCoordProj.w;
    vTexCoordProj.y = 0.5 + 0.5 * vTexCoordProj.y / OUT_TexCoordProj.w;
    vTexCoordProj = clamp(vTexCoordProj, 0.001, 0.999);
    lowp vec4 vRefractionColor = texture2D(SAMPLER_02, vTexCoordProj);
    
    highp vec2 vTexCoord_01 = vec2(OUT_TexCoord.x * 8.0 + sin(OUT_Timer) * 0.33,
								   OUT_TexCoord.y * 8.0 + cos(OUT_Timer) * 0.66);
	
	highp vec2 vTexCoord_02 = vec2(OUT_TexCoord.x * 8.0 - sin(OUT_Timer) * 0.75,
								   OUT_TexCoord.y * 8.0 - cos(OUT_Timer) * 0.25);
    
    lowp vec4 vWaterColor = texture2D(SAMPLER_03, vTexCoord_01) + texture2D(SAMPLER_03, vTexCoord_02);
    vRefractionColor = mix(vRefractionColor, vWaterColor, 0.25);
    vReflectionColor = mix(vRefractionColor, vReflectionColor, 0.25);
    gl_FragColor = vReflectionColor;
}

