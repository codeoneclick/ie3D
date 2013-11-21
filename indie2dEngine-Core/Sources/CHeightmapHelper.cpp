//
//  CHeightmapHelper.cpp
//  iGaia
//
//  Created by Sergey Sergeev on 3/13/13.
//
//

#include "CHeightmapHelper.h"

static f32 g_upVectorOffset = 0.25f;

f32 CHeightmapHelper::Get_HeightValue(f32* _data, ui32 _width, ui32 _height,const glm::vec3& _position)
{
    f32 _x = _position.x / 1.0f;
    f32 _z = _position.z / 1.0f;
    i32 x = static_cast<i32>(floor(_x));
    i32 z = static_cast<i32>(floor(_z));
    f32 dx = _x - x;
    f32 dy = _z - z;

    if((x < 0) || (z < 0) || (x > (_width - 1)) || (z > (_height - 1)))
    {
        return -0.0f;
    }

    f32 height_00 = _data[x + z * _width];
    f32 height_01 = _data[x + z * _width];

    if(z < (_height - 1) && z >= 0)
    {
        height_01 = _data[x + (z + 1) * _width];
    }

    f32 height_10 = _data[x + z * _width];
    if(x < (_width - 1) && x >= 0)
    {
        height_10 = _data[x + 1 + z * _width];
    }

    f32 height_11 = _data[x + z * _width];
    if(z < (_height - 1) && z >= 0 && x < (_width - 1) && x >= 0)
    {
        height_11 = _data[x + 1 + (z + 1) * _width];
    }

    f32 height_0 = height_00 * (1.0f - dy) + height_01 * dy;
    f32 height_1 = height_10 * (1.0f - dy) + height_11 * dy;

    return height_0 * (1.0f - dx) + height_1 * dx;
}

f32 CHeightmapHelper::Get_RotationForPlane(const glm::vec3& _point_01 ,const glm::vec3& _point_02, const glm::vec3& _point_03)
{
    f32 vectorLength_01 = sqrtf(powf(_point_02.x - _point_01.x, 2) + powf(_point_02.y - _point_01.y, 2) + powf(_point_02.z - _point_01.z, 2));
    f32 vectorLength_02 = sqrtf(powf(_point_03.x - _point_01.x, 2) + powf(_point_03.y - _point_01.y, 2) + powf(_point_03.z - _point_01.z, 2));
    f32 scalar = (_point_02.x - _point_01.x)*(_point_03.x - _point_01.x) + (_point_02.y - _point_01.y)*(_point_03.y - _point_01.y) + (_point_02.z - _point_01.z) * (_point_03.z - _point_01.z);
    return scalar / (vectorLength_01 * vectorLength_02);
}

glm::vec2 CHeightmapHelper::Get_RotationOnHeightmap(f32* _data, ui32 _width, ui32 _height, glm::vec3 _position)
{
    glm::vec3 point_01 = _position;
    glm::vec3 point_02 = glm::vec3(_position.x, _position.y + g_upVectorOffset, _position.z);
    f32 height = Get_HeightValue(_data, _width, _height, glm::vec3(_position.x + g_upVectorOffset, 0.0f, _position.z));
    glm::vec3 point_03 = glm::vec3(_position.x + g_upVectorOffset, height, _position.z);
    height = Get_HeightValue(_data, _width, _height, glm::vec3(_position.x, 0.0f, _position.z + g_upVectorOffset));
    glm::vec3 point_04 = glm::vec3(_position.x, height, _position.z + g_upVectorOffset);

    f32 angle_01 = Get_RotationForPlane(point_01, point_02, point_03);
    f32 angle_02 = Get_RotationForPlane(point_01, point_02, point_04);

    return glm::vec2(-(-acosf(angle_02) + M_PI_2), -acosf(angle_01) + M_PI_2);
}
