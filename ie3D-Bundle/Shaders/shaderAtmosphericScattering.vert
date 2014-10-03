
attribute vec3 IN_Position;
attribute vec2 IN_TexCoord;
attribute vec4 IN_Normal;
attribute vec4 IN_Tangent;
attribute vec4 IN_Color;
attribute vec4 IN_Extra;

uniform mat4   MATRIX_Projection;
uniform mat4   MATRIX_View;
uniform mat4   MATRIX_World;

uniform vec3   VECTOR_CameraPosition;
uniform vec3   VECTOR_LightPosition_01;

const vec4 scaleIteration = vec4(0.01, 0.25, 0.04, 10.0);
const vec4 radiusInOutHeightMinMax = vec4(6356.75, 6456.55, -100.0, 2000.0);
const vec4 scatterFactors = vec4(0.04, 0.03, 0.025, 0.0188);
const vec4 invertWaveLength = vec4(5.602,9.473, 19.644, 0.0);

varying vec4 OUT_ColorRayleigh;
varying vec4 OUT_ColorMie;
varying vec3 OUT_Direction;
varying vec3 OUT_LightDirection;

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

    OUT_LightDirection = normalize(vPosition.xyz - VECTOR_LightPosition_01);
    vec3 vCameraPosition = VECTOR_CameraPosition;
    vec3 vCameraRay = normalize(vPosition.xyz - vCameraPosition);
    
    float fCameraHeight = (VECTOR_CameraPosition.y - radiusInOutHeightMinMax.z) / (radiusInOutHeightMinMax.w - radiusInOutHeightMinMax.z);
    fCameraHeight = radiusInOutHeightMinMax.x + fCameraHeight * (radiusInOutHeightMinMax.y - radiusInOutHeightMinMax.x);
    
    vec3 vStart = vec3(VECTOR_CameraPosition.x, fCameraHeight, VECTOR_CameraPosition.z);
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
        
        float fLightAngle = dot(-OUT_LightDirection, vSamplePoint) / fHeight;
        
        float fCameraAngle = dot(vCameraRay, vSamplePoint) / fHeight;
        
        float fScatter = (fStartOffset + fDepth * (scale(fLightAngle) - scale(fCameraAngle)));
        
        vec3 vAttenuate = exp(-fScatter * (invertWaveLength.xyz * scatterFactors.z + scatterFactors.w));
        
        color += vAttenuate * (fDepth * fScaledLength);
        
        vSamplePoint += vSampleRay;
    }

    OUT_ColorRayleigh.xyz = color * (invertWaveLength.xyz * scatterFactors.x);
    OUT_ColorRayleigh.w = 1.0;
    
    OUT_ColorMie.xyz = color * scatterFactors.y;
    OUT_ColorMie.w = 1.0;

    OUT_Direction = vCameraPosition - vPosition.xyz;
}