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
        _stream.read((char*)&x, sizeof(f32));
        _stream.read((char*)&y, sizeof(f32));
        _stream.read((char*)&z, sizeof(f32));
        m_positions.push_back(glm::vec3(x, y, z));
        
        
        _stream.read((char*)&x, sizeof(f32));
        _stream.read((char*)&y, sizeof(f32));
        _stream.read((char*)&z, sizeof(f32));
        _stream.read((char*)&w, sizeof(f32));
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
    _stream.read((char*)&numFrames, sizeof(i32));
    m_frames.reserve(numFrames);
    
    for (i32 i = 0; i < numFrames; ++i)
    {
        std::shared_ptr<CAnimationFrame> frame = std::make_shared<CAnimationFrame>(_numBones);
        frame->_Serialize(_stream);
        m_frames.push_back( frame );
    }
}