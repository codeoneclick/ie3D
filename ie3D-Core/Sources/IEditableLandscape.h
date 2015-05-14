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

class CHeightmapAccessor;

class IEditableLandscape
{
private:
    
    ui32 m_editableSize;
    ui32 m_editableStrength;
    ui32 m_editableFalloffCoefficient;
    ui32 m_editableSmoothCoefficient;
    
protected:

    std::shared_ptr<CHeightmapAccessor> m_heightmapAccessor;
    std::vector<CSharedLandscapeChunk> m_chunks;
    
public:
    IEditableLandscape(void);
    ~IEditableLandscape(void);
    
    void setEditableSize(ui32 value);
    void setEditableStrength(ui32 value);
    void setEditableFalloffCoefficient(ui32 value);
    void setEditableSmoothCoefficient(ui32 value);
    
    void pressureHeight(const glm::vec3& point, f32 pressureForce);
    
    void generateVertecesData(const glm::ivec2& size, f32 frequency, i32 octaves, ui32 seed);
    
};

#endif
