
#if defined(__OPENGL_30__)

out vec2   OUT_TexCoord;

#else

varying vec2   OUT_TexCoord;

#endif

uniform mat4 u_matrixM;
uniform mat4 u_matrixVP;
uniform mat4 MATRIX_Bones[32];

uniform vec4 VECTOR_ClipPlane;

uniform int INT_FLAG_01;
uniform int INT_FLAG_02;
uniform int INT_FLAG_03;
uniform int INT_FLAG_04;

void main(void)
{
    vec4 vPosition = vec4(IN_Position, 1.0);
    vec4 vWeights = IN_Extra / 255.0;
    
    if(INT_FLAG_01 == 1)
    {
        int index = 0;
        vec4 vBonePosition = vec4(0.0, 0.0, 0.0, 0.0);
    
        for(int i = 0; i < 4; i++)
        {
            index = int(IN_Color[i]);
            vBonePosition += MATRIX_Bones[index] * vPosition * vWeights[i];
        }
        vPosition = vBonePosition;
    }
    vPosition = u_matrixM * vPosition;
    gl_Position = u_matrixVP * vPosition;
    gl_ClipDistance[0] = dot(vPosition.xyz, VECTOR_ClipPlane.xyz);
    OUT_TexCoord = IN_TexCoord;
}