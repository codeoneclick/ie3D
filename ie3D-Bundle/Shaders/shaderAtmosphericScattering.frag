
#if defined(__OPENGL_30__)

in vec3 OUT_RayleighPhase;
in vec4 OUT_Mie;
in vec3 OUT_Direction;
in vec3 OUT_LightDirection;

#else

varying vec3 OUT_RayleighPhase;
varying vec4 OUT_Mie;
varying vec3 OUT_Direction;
varying vec3 OUT_LightDirection;

#endif

const float fG = -0.98;
const float fG2 = -0.98 * -0.98;
const float fExposure =  -1.0;

float getRayleighPhase(float cosin2)
{
    return 0.75 + 0.75 * cosin2;
}

float getMiePhase(float fCos, float fCos2)
{
    vec3 vHG = vec3(1.5 * ((1.0 - fG2) / (2.0 + fG2)), 1.0 + fG2, 2.0 * fG);
    return vHG.x * (1.0 + fCos2) / pow(vHG.y - vHG.z * fCos, 1.5);
}

void main(void)
{
    vec4 vColor = vec4(1.0, 1.0, 1.0, 1.0);
    
    vec2 fCos;
    fCos.x = dot(OUT_LightDirection, OUT_Direction) / length(OUT_Direction);
    fCos.y = fCos.x * fCos.x;
    
    vec3 vMie = getMiePhase(fCos.x, fCos.y) * OUT_Mie.rgb;
    vec3 vRayleighPhase = getRayleighPhase(fCos.y) * OUT_RayleighPhase;
    vColor.rgb = 1.0 - exp(fExposure * (vRayleighPhase + vMie));
    vColor.a = min(OUT_Mie.a, 0.75);

    gl_FragColor = vColor;
}