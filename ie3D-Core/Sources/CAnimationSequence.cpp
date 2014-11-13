//
//  CAnimationSequence.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 11/13/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CAnimationSequence.h"

CFrameData::CFrameData(const std::vector<glm::quat>& rotations,
                       const std::vector<glm::vec3>& positions,
                       const std::vector<glm::vec3>& scales) :
m_rotations(rotations),
m_positions(positions),
m_scales(scales)
{
    
}

CFrameData::~CFrameData(void)
{
    m_rotations.clear();
    m_positions.clear();
    m_scales.clear();
}

const glm::quat& CFrameData::getRotation(ui32 index) const
{
    assert(m_rotations.size() > index);
    return m_rotations.at(index);
}

const glm::vec3& CFrameData::getPosition(ui32 index) const
{
    assert(m_positions.size() > index);
    return m_positions.at(index);
}

const glm::vec3& CFrameData::getScale(ui32 index) const
{
    assert(m_scales.size() > index);
    return m_scales.at(index);
}

CSequenceData::CSequenceData(const std::string& animationName,
                             ui32 animationFPS,
                             const std::vector<CSharedFrameData>& frames) :
IResourceData(E_RESOURCE_DATA_CLASS_SEQUENCE_DATA),
m_animationName(animationName),
m_animationFPS(animationFPS),
m_frames(frames)
{
    
}

CSequenceData::~CSequenceData(void)
{
    m_frames.clear();
}

ui32 CSequenceData::getNumFrames(void) const
{
    return static_cast<ui32>(m_frames.size());
}

ui32 CSequenceData::getAnimationFPS(void) const
{
    return m_animationFPS;
}

const std::string CSequenceData::getAnimationName(void) const
{
    return m_animationName;
}

CSharedFrameData CSequenceData::getFrame(ui32 index) const
{
    assert(m_frames.size() > index);
    return m_frames.at(index);
}

CAnimationSequence::CAnimationSequence(const std::string& guid) :
IResource(E_RESOURCE_CLASS_SEQUENCE, guid),
m_sequenceData(nullptr)
{
    
}

CAnimationSequence::~CAnimationSequence(void)
{
    
}

void CAnimationSequence::onResourceDataSerializationFinished(ISharedResourceDataRef resourceData)
{
    assert(resourceData != nullptr);
    switch(resourceData->getResourceDataClass())
    {
        case E_RESOURCE_DATA_CLASS_SEQUENCE_DATA:
        {
            m_sequenceData = std::static_pointer_cast<CSequenceData>(resourceData);
            m_status |= E_RESOURCE_STATUS_LOADED;
        }
            break;
            
        default:
        {
            assert(false);
        }
            break;
    }
}

void CAnimationSequence::onResourceDataCommitFinished(ISharedResourceDataRef resourceData)
{
    assert(resourceData != nullptr);
    switch(resourceData->getResourceDataClass())
    {
        case E_RESOURCE_DATA_CLASS_SEQUENCE_DATA:
        {
            m_status |= E_RESOURCE_STATUS_COMMITED;
        }
            break;
            
        default:
        {
            assert(false);
        }
            break;
    }
}

ui32 CAnimationSequence::getNumFrames(void) const
{
    return IResource::isLoaded() ? m_sequenceData->getNumFrames() : 0;
}
    
ui32 CAnimationSequence::getAnimationFPS(void) const
{
    return IResource::isLoaded() ? m_sequenceData->getAnimationFPS() : 0;
}
    
const std::string CAnimationSequence::getAnimationName(void) const
{
    return IResource::isLoaded() ? m_sequenceData->getAnimationName() : "";
}
    
CSharedFrameData CAnimationSequence::getFrame(ui32 index) const
{
    return IResource::isLoaded() ? m_sequenceData->getFrame(index) : nullptr;
}
