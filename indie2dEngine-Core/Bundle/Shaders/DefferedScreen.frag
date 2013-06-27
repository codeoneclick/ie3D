varying highp vec2 OUT_TexCoord;
uniform sampler2D SAMPLER_01;

void main(void)
{
    mediump vec3 lightPos = vec3(18.0, 0.0, 18.0);
    mediump vec3    p  = vec3(1.0, 1.0, 1.0);
    mediump vec3    n  = texture2D(SAMPLER_01, OUT_TexCoord).xyz;
    mediump vec3    l  = normalize     ( lightPos - p );
    mediump vec3    v  = normalize     ( -p );
    mediump vec3    h  = normalize     ( l + v );
    mediump float   diff = max         ( 0.2, dot ( l, n ) );
    mediump float   spec = pow         ( max ( 0.0, dot ( h, n ) ), 40.0 );
    
    gl_FragColor = vec4 ( vec3 ( diff + spec ), 1.0 );
}

