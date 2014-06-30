//
//  CAnimationMixer.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 8/13/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CAnimationMixer_h
#define CAnimationMixer_h

#include "HCommon.h"
#include "HDeclaration.h"

class CAnimationMixer
{
private:
    
protected:
    
    f32 m_animationTime;
    CSharedSkeleton m_skeleton;
    CSharedSequenceData m_sequenceData;
    glm::mat4x4* m_bonesTransformations;
    
    void bindPoseTransformation(void);
    
public:
    
    CAnimationMixer(CSharedSkeletonDataRef skeletonData, CSharedSequenceDataRef sequenceData);
    ~CAnimationMixer(void);
    
    glm::mat4x4* getTransformations(void) const;
    ui32 getTransformationSize(void) const;
    
    void setAnimation(const std::string& name);
    void update(f32 deltatime);
};

#endif 
