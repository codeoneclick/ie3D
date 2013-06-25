varying highp vec2 OUT_TexCoord;
uniform sampler2D SAMPLER_01;

void main(void)
{
    mediump float distance = 0.0033;
    mediump vec4 color;
	color  = texture2D(SAMPLER_01, vec2(OUT_TexCoord.x + distance, OUT_TexCoord.y + distance));
	color += texture2D(SAMPLER_01, vec2(OUT_TexCoord.x - distance, OUT_TexCoord.y - distance));
	color += texture2D(SAMPLER_01, vec2(OUT_TexCoord.x + distance, OUT_TexCoord.y - distance));
	color += texture2D(SAMPLER_01, vec2(OUT_TexCoord.x - distance, OUT_TexCoord.y + distance));
    gl_FragColor = color / 4.0;
}

