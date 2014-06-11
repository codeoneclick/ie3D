//
//  IEditableLandscape.h
//  ie3D-Core
//
//  Created by Sergey Sergeev on 5/27/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef IEditableLandscape_h
#define IEditableLandscape_h

#include "HCommon.h"
#include "HDeclaration.h"

class IEditableLandscape
{
private:
    
    CSharedHeightmapProcessor m_heightmapProcessor;
    
protected:

    void setHeightmapProcessor(CSharedHeightmapProcessorRef heightmapProcessor);
    
    
public:
    IEditableLandscape(void);
    ~IEditableLandscape(void);
    
    void pressureHeight(const glm::vec3& point, f32 radius,
                        bool isSmooth, f32 pressureForce);
    
};

#endif
