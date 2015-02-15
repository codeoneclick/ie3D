//
//  CAnimationSequence.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 11/13/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef CAnimationSequence_h
#define CAnimationSequence_h

#include "IResource.h"

extern const std::string kBindposeAnimationName;

class CFrameData final
{
private:
    
protected:
    
    std::vector<glm::quat> m_rotations;
    std::vector<glm::vec3> m_positions;
    std::vector<glm::vec3> m_scales;
    
public:
    
    CFrameData(const std::vector<glm::quat>& rotations,
               const std::vector<glm::vec3>& positions,
               const std::vector<glm::vec3>& scales);
    
    ~CFrameData(void);
    
    const glm::quat& getRotation(ui32 index) const;
    const glm::vec3& getPosition(ui32 index) const;
    const glm::vec3& getScale(ui32 index) const;
};

class CSequenceData final : public IResourceData
{
private:
    
protected:
    
    std::vector<CSharedFrameData> m_frames;
    ui32 m_animationFPS;
    std::string m_animationName;
    
public:
    
    CSequenceData(const std::string& animationName,
                  ui32 animationFPS,
                  const std::vector<CSharedFrameData>& frames);
    
    ~CSequenceData(void);
    
    ui32 getNumFrames(void) const;
    
    ui32 getAnimationFPS(void) const;
    
    const std::string getAnimationName(void) const;
    
    CSharedFrameData getFrame(ui32 index) const;
};

class CAnimationSequence : public IResource
{
private:
    
protected:
    
    CSharedSequenceData m_sequenceData;
    
    void onResourceDataSerializationFinished(ISharedResourceDataRef resourceData);
    void onResourceDataCommitFinished(ISharedResourceDataRef resourceData);
    
public:
    
    CAnimationSequence(const std::string& guid);
    
    static CSharedAnimationSequence constructCustomAnimationSequence(const std::string& guid,
                                                                     CSharedSequenceDataRef data);
    
    ~CAnimationSequence(void);
    
    ui32 getNumFrames(void) const;
    
    ui32 getAnimationFPS(void) const;
    
    const std::string getAnimationName(void) const;
    
    CSharedFrameData getFrame(ui32 index) const;

};

#endif
