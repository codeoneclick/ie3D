//
//  CHeightmapHelper.h
//  iGaia
//
//  Created by Sergey Sergeev on 3/13/13.
//
//

#ifndef CHeightmapHelper_h
#define CHeightmapHelper_h

#include "HCommon.h"

class CHeightmapHelper final
{
private:
    
    static f32 Get_RotationForPlane(const glm::vec3& _point_01 ,const glm::vec3& _point_02, const glm::vec3& _point_03);
    
protected:

public:
    
    CHeightmapHelper(void) {};
    ~CHeightmapHelper(void) {};

    static f32 Get_HeightValue(f32* _data, ui32 _width, ui32 _height,const glm::vec3& _position);
    static glm::vec2 Get_RotationOnHeightmap(f32* _data, ui32 _width, ui32 _height, glm::vec3 _position);
};


#endif 
