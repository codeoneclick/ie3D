//
//  CSkeleton.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/25/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CSkeleton_h
#define CSkeleton_h

#include "HCommon.h"
#include "HDeclaration.h"

class CSkeleton
{
private:
    
protected:
    
    glm::mat4x4* m_bonesTransformations;
    CSharedSkeletonData m_skeletonData;
    
public:
    
    CSkeleton(CSharedSkeletonDataRef skeletonData);
    ~CSkeleton(void);
    
    ui32 getNumBones(void) const;
    CSharedBone getBone(ui32 index) const;
    glm::mat4* getBonesTransformations(void) const;
    
    void update(void);
    void bindPoseTransformation(void);
};

#endif

