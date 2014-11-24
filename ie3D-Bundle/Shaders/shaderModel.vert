
#if defined(__OPENGL_30__)

out vec2   OUT_TexCoord;
out float  OUT_ClipPlane;

#else

varying vec2   OUT_TexCoord;
varying float  OUT_ClipPlane;

#endif

uniform mat4   MATRIX_Projection;
uniform mat4   MATRIX_View;
uniform mat4   MATRIX_World;
uniform mat4   MATRIX_Bones[32];

uniform vec4   VECTOR_ClipPlane;

uniform int    INT_FLAG_01;
uniform int    INT_FLAG_02;
uniform int    INT_FLAG_03;
uniform int    INT_FLAG_04;

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
        vPosition = MATRIX_World * vBonePosition;
    }
    else
    {
        vPosition = MATRIX_World * vPosition;
    }

    gl_Position = MATRIX_Projection * MATRIX_View * vPosition;
    OUT_TexCoord = IN_TexCoord;
    
    OUT_ClipPlane = dot(vPosition.xyz, VECTOR_ClipPlane.xyz) + VECTOR_ClipPlane.w;
}