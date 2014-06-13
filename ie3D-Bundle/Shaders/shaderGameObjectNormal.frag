varying highp   vec3   OUT_Normal;

void main(void)
{
    gl_FragColor = vec4(0.5 * OUT_Normal + vec3(0.5), 1.0);
}