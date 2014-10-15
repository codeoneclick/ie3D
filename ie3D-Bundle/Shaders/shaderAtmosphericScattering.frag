uniform sampler2D SAMPLER_01;

varying highp vec3 OUT_RayleighPhase;
varying highp vec4 OUT_Mie;
varying highp vec3 OUT_Direction;
varying highp vec3 OUT_LightDirection;

const highp float fG = -0.98;
const highp float fG2 = -0.98 * -0.98;
const highp float fExposure =  -1.0;

highp float getRayleighPhase(highp float cosin2)
{
    return 0.75 + 0.75 * cosin2;
}

highp float getMiePhase(highp float fCos, highp float fCos2)
{
    highp vec3 vHG = vec3(1.5 * ((1.0 - fG2) / (2.0 + fG2)), 1.0 + fG2, 2.0 * fG);
    return vHG.x * (1.0 + fCos2) / pow(vHG.y - vHG.z * fCos, 1.5);
}

void main(void)
{
    highp vec4 vColor = vec4(1.0, 1.0, 1.0, 1.0);
    
    highp vec2 fCos;
    fCos.x = dot(OUT_LightDirection, OUT_Direction) / length(OUT_Direction);
    fCos.y = fCos.x * fCos.x;
    
    highp vec3 vMie = getMiePhase(fCos.x, fCos.y) * OUT_Mie.rgb;
    highp vec3 vRayleighPhase = getRayleighPhase(fCos.y) * OUT_RayleighPhase;
    vColor.rgb = 1.0 - exp(fExposure * (vRayleighPhase + vMie));
    vColor.a = min(OUT_Mie.a, 0.75);
    gl_FragColor = vColor;
}