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
    
    ui32 m_numBones;
    std::set<CSharedBone> m_rootBones;
    glm::mat4x4* m_bonesTransformations;
    
public:
    
    CSkeleton(CSharedSkeletonDataRef skeletonData);
    ~CSkeleton(void);
    
    void addBone(CSharedBoneRef bone);
    CSharedBone getBone(ui32 index) const;
    
    ui32 getNumBones(void) const;
    const std::set<CSharedBone> getRootBones(void) const;
    
    glm::mat4* getBonesTransformations(void) const;
    
    void update(void);
    void bindPoseTransformation(void);
};

#endif

