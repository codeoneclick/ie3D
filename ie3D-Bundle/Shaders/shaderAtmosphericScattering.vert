
#if defined(__OPENGL_30__)

out vec3 OUT_RayleighPhase;
out vec4 OUT_Mie;
out vec3 OUT_Direction;
out vec3 OUT_LightDirection;

#else

varying vec3 OUT_RayleighPhase;
varying vec4 OUT_Mie;
varying vec3 OUT_Direction;
varying vec3 OUT_LightDirection;

#endif

uniform mat4   MATRIX_Projection;
uniform mat4   MATRIX_View;
uniform mat4   MATRIX_World;

uniform vec3   VECTOR_CameraPosition;
uniform vec3   VECTOR_GlobalLightPosition;

const vec4 scaleIteration = vec4(0.01, 0.25, 0.04, 10.0);
const vec4 radiusInOutHeightMinMax = vec4(6356.75, 6456.55, 0.0, 256.0);
const vec4 scatterFactors = vec4(0.04, 0.03, 0.025, 0.0188);
const float fKrESun = 20.0 * 0.0025;
const float fKmESun = 20.0 * 0.001;

float rayIntersection(vec3 position, vec3 direction, float outerRadius)
{
    float A = dot(direction, direction);
    float B = 2.0 * dot(position, direction);
    float C = dot(position, position) - outerRadius * outerRadius;
    
    float t0 = (-B + sqrt(B * B - 4.0 * A * C)) / 2.0 * A;
    float t1 = (-B - sqrt(B * B - 4.0 * A * C)) / 2.0 * A;
    
    return t0 >= 0.0 ? t0 : t1;
}

float scale(float cosin)
{
    float x = 1.0 - cosin;
    return scaleIteration.y * exp(-0.00287 + x * (0.459 + x * (3.83 + x * (-6.80 + x * 5.25))));
}

void main(void)
{
    vec4 vPosition = vec4(IN_Position, 1.0);
    vPosition = MATRIX_World * vPosition;
    
    gl_Position = MATRIX_Projection * MATRIX_View * vPosition;
    
    vec3 vInvertWavelength = vec3(1.0 / pow(0.650, 4.0), 1.0 / pow(0.570, 4.0), 1.0 / pow(0.475, 4.0));

    OUT_LightDirection = normalize(VECTOR_GlobalLightPosition - vPosition.xyz);
    vec3 vCameraPosition = VECTOR_CameraPosition;
    vec3 vCameraRay = normalize(vPosition.xyz - vCameraPosition);
    
    float fCameraHeight = (radiusInOutHeightMinMax.z) / (radiusInOutHeightMinMax.w - radiusInOutHeightMinMax.z);
    fCameraHeight = radiusInOutHeightMinMax.x + fCameraHeight * (radiusInOutHeightMinMax.y - radiusInOutHeightMinMax.x);
    
    vec3 vStart = vec3(0.0, fCameraHeight, 0.0);
    float fDistance = rayIntersection(vStart, vCameraRay, radiusInOutHeightMinMax.y);
    
    float fStartAngle = dot(vCameraRay, vStart) / fCameraHeight;
    
    float fStartDepth = exp(scaleIteration.z * (radiusInOutHeightMinMax.x - fCameraHeight ));
    float fStartOffset = fStartDepth * scale(fStartAngle);
    
    float fSampleLength = fDistance / 8.0;
    float fScaledLength = fSampleLength * scaleIteration.x;
    
    vec3 vSampleRay = vCameraRay * fSampleLength;
    vec3 vSamplePoint = vStart + vSampleRay * 0.5;
    
    vec3 color = vec3(0.0, 0.0, 0.0);
    
    for(int i = 0; i < 8; i++)
    {
        float fHeight = length(vSamplePoint);
        float fDepth = exp(scaleIteration.z * (radiusInOutHeightMinMax.x - fHeight));
        
        float fLightAngle = dot(OUT_LightDirection, vSamplePoint) / fHeight;
        
        float fCameraAngle = dot(vCameraRay, vSamplePoint) / fHeight;
        
        float fScatter = (fStartOffset + fDepth * (scale(fLightAngle) - scale(fCameraAngle)));
        
        vec3 vAttenuate = exp(-fScatter * (vInvertWavelength * scatterFactors.z + scatterFactors.w));
        
        color += vAttenuate * (fDepth * fScaledLength);
        
        vSamplePoint += vSampleRay;
    }

    OUT_RayleighPhase = color * (vInvertWavelength * fKrESun);
    OUT_Mie = vec4(color * fKmESun, clamp(vPosition.y, 0.0, 1.0));
    OUT_Mie.a = max(0.0, min(IN_Position.y + 1.0, 1.0));
    OUT_Direction = vCameraPosition - vPosition.xyz;
}