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
    f32 m_blendingAnimationTimeinterval;
    
    std::string m_currentAnimationName;
    
    ui32 m_currentAnimationFrame;
    ui32 m_blendingAnimationFrame;
    
    CSharedAnimationSequence m_previousAnimationSequence;
    CSharedAnimationSequence m_currentAnimationSequence;
    
    CSharedSkeleton m_skeleton;
    
    std::map<std::string, CSharedAnimationSequence> m_animationsSequences;
    glm::mat4x4* m_bonesTransformations;
    bool m_isBinded;
    
    void bindPoseTransformation(void);
    bool tryBindCurrentAnimationSequence(void);
    
public:
    
    CAnimationMixer(CSharedSkeletonDataRef skeletonData);
    ~CAnimationMixer(void);
    
    glm::mat4x4* getTransformations(void) const;
    ui32 getTransformationSize(void) const;
    
    void addAnimationSequence(CSharedAnimationSequenceRef animationSequence);
    void removeAnimationSequence(CSharedAnimationSequenceRef animationSequence);
    
    void setAnimation(const std::string& name);
    void update(f32 deltatime);
    
    bool isAnimated(void);
};

#endif 
