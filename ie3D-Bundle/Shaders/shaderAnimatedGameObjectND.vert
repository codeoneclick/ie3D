
#if defined(__OPENGL_30__)

out vec4 OUT_Position;

#else

varying vec4 OUT_Position;

#endif

uniform mat4 u_matrixM;
uniform mat4 MATRIX_GlobalLightProjection;
uniform mat4 MATRIX_GlobalLightView;
uniform mat4 MATRIX_Bones[32];

uniform int    INT_FLAG_01;
uniform int    INT_FLAG_02;
uniform int    INT_FLAG_03;
uniform int    INT_FLAG_04;
                                       
void main(void)
{
    OUT_Position = vec4(IN_Position, 1.0);
    vec4 vWeights = IN_Extra / 255.0;
    
    if(INT_FLAG_01 == 1)
    {
        int index = 0;
        vec4 vBonePosition = vec4(0.0, 0.0, 0.0, 0.0);
        
        for(int i = 0; i < 4; i++)
        {
            index = int(IN_Color[i]);
            vBonePosition += MATRIX_Bones[index] * OUT_Position * vWeights[i];
        }
        OUT_Position = vBonePosition;
    }
    OUT_Position = MATRIX_GlobalLightProjection * MATRIX_GlobalLightView * u_matrixM * OUT_Position;
    gl_Position = OUT_Position;
}
