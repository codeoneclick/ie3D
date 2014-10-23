
#if defined(__OPENGL_30__)

in vec2 OUT_TexCoord;

#else

varying vec2 OUT_TexCoord;

#endif

uniform sampler2D  SAMPLER_01;

void main()
{
	/*lowp vec4 color = vec4(0.0);
	color += texture2D(SAMPLER_01, OUT_BTexCoord[0]) * 0.204164;
	color += texture2D(SAMPLER_01, OUT_BTexCoord[1]) * 0.304005;
	color += texture2D(SAMPLER_01, OUT_BTexCoord[2]) * 0.304005;
	color += texture2D(SAMPLER_01, OUT_BTexCoord[3]) * 0.093913;
	color += texture2D(SAMPLER_01, OUT_BTexCoord[4]) * 0.093913;*/

    gl_FragColor = texture2D(SAMPLER_01, OUT_TexCoord);
}

