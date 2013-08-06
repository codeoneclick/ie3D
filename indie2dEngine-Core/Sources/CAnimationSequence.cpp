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
    m_scales.reserve(_numBones);
    m_eulers.reserve(_numBones);
    m_eulersYPR.reserve(_numBones);
}

CAnimationFrame::~CAnimationFrame(void)
{
    
}

void CAnimationFrame::_Serialize(std::ifstream &_stream)
{
    for (i32 i = 0; i < m_numBones; ++i)
    {
        glm::vec3 position;
        _stream.read((char*)&position, sizeof(glm::vec3));
        m_positions.push_back(glm::vec3(position));
        
        glm::quat rotation;
        _stream.read((char*)&rotation, sizeof(glm::quat));
        
        glm::quat _rotation = glm::quat(rotation.w, rotation.x, rotation.y, rotation.z);
        //glm::vec3 euler = glm::eulerAngles(_rotation);
        std::cout.setf(std::ios::fixed, std::ios::floatfield);
        std::cout.setf(std::ios::showpoint);
        std::cout.precision(3);
        std::cout<<"Bone index: "<<i<<std::endl;
        //std::cout<<"rotation: "<<euler.x<<","<<euler.y<<","<<euler.z<<std::endl;
        /*f32 value = rotation.x;
        rotation.x = rotation.z;
        rotation.z = value;*/
        m_rotations.push_back(_rotation);
        
        glm::vec3 scale;
        _stream.read((char*)&scale, sizeof(glm::vec3));
        m_scales.push_back(scale);
        
        glm::vec3 euler;
        _stream.read((char*)&euler, sizeof(glm::vec3));
        m_eulers.push_back(euler);
        
        glm::vec3 eulerYPR;
        _stream.read((char*)&eulerYPR, sizeof(glm::vec3));
        m_eulersYPR.push_back(eulerYPR);
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