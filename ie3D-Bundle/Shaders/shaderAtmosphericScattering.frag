uniform sampler2D SAMPLER_01;

varying highp vec4 OUT_ColorRayleigh;
varying highp vec4 OUT_ColorMie;
varying highp vec3 OUT_Direction;
varying highp vec3 OUT_LightDirection;

const highp vec4 gravity = vec4(-0.991, 0.982, 0.0, 0.0);

highp float getRayleighPhase(highp float cosin2)
{
    return 0.75 * (1.0 + cosin2);
}

highp float getMiePhase(highp float cosin, highp float cosin2)
{
    highp vec3 t3;
    t3.x = 1.5 * ((1.0 - gravity.y) / (2.0 + gravity.y));
    t3.y = 1.0 + gravity.y;
    t3.z = 2.0 * gravity.x;
    return t3.x * (1.0 + cosin2) / pow(t3.y - t3.z * cosin, 1.5);
}

void main(void)
{
    lowp vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
    
    highp vec2 cosin;
    cosin.x = dot(-OUT_LightDirection, OUT_Direction) / length(OUT_Direction);
    cosin.y = cosin.x * cosin.x;
    
    color.rgb = getRayleighPhase(cosin.y) * OUT_ColorRayleigh.rgb + getMiePhase(cosin.x, cosin.y) * OUT_ColorMie.rgb;
    
    color.a = 0.5;
    gl_FragColor = color;
}