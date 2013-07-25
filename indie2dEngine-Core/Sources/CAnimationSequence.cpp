//
//  CAnimationSequence.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/25/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CAnimationSequence.h"

CAnimationFrame::CAnimationFrame(i32 _numBones) :
m_numBones(_numBones)
{
    m_rotations.reserve(_numBones);
    m_positions.reserve(_numBones);
}

CAnimationFrame::~CAnimationFrame(void)
{
    
}

void CAnimationFrame::_Serialize(std::ifstream &_stream)
{
    f32 x, y, z ,w;
    for (i32 i = 0; i < m_numBones; ++i)
    {
        _stream>>x>>y>>z;
        m_positions.push_back(glm::vec3(x, y, z));
        _stream>>x>>y>>z>>w;
        m_rotations.push_back(glm::quat(x, y, z, w));
    }
}

CAnimationSequence::CAnimationSequence(void)
{
    
}

CAnimationSequence::~CAnimationSequence(void)
{
    m_frames.clear();
}

void CAnimationSequence::_Serialize(std::ifstream &_stream, i32 _numBones)
{
    i32 numFrames = 0;
    _stream >> numFrames;
    m_frames.reserve(numFrames);
    
    for (i32 i = 0; i < numFrames; ++i)
    {
        std::shared_ptr<CAnimationFrame> frame = std::make_shared<CAnimationFrame>(numFrames);
        frame->_Serialize(_stream);
        m_frames.push_back( frame );
    }
}