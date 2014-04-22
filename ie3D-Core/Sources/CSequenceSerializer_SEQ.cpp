//
//  CSequenceSerializer_SEQ.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 8/13/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CSequenceSerializer_SEQ.h"
#include "CMesh.h"

CSequenceSerializer_SEQ::CSequenceSerializer_SEQ(const std::string& filename,
                                                 ISharedResourceRef resource) :
IResourceSerializer(filename, resource),
m_filename(filename)
{
    
}

CSequenceSerializer_SEQ::~CSequenceSerializer_SEQ(void)
{
    
}

void CSequenceSerializer_SEQ::serialize(void)
{
    assert(m_resource != nullptr);
    m_status = E_SERIALIZER_STATUS_INPROGRESS;
    
    std::shared_ptr<std::istream> filestream = IResourceSerializer::openStream(m_filename);
    
    i32 numBones = 0;
    filestream->read((char*)&numBones, sizeof(i32));
    i32 numFrames = 0;
    filestream->read((char*)&numFrames, sizeof(i32));
    
    std::vector<CSharedFrameData> frames;
    frames.resize(numFrames);
    
    for (ui32 i = 0; i < numFrames; ++i)
    {
        std::vector<glm::quat> rotations;
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> scales;
        
        for (i32 i = 0; i < numBones; ++i)
        {
            glm::vec3 position;
            filestream->read((char*)&position, sizeof(glm::vec3));
            positions.push_back(glm::vec3(position));
            
            glm::quat rotation;
            filestream->read((char*)&rotation, sizeof(glm::quat));
            glm::quat _rotation = glm::quat(rotation.w, -rotation.x, -rotation.y, -rotation.z);
            rotations.push_back(_rotation);
            
            glm::vec3 scale;
            filestream->read((char*)&scale, sizeof(glm::vec3));
            scales.push_back(scale);
        }
        CSharedFrameData frame = std::make_shared<CFrameData>(rotations,
                                                              positions,
                                                              scales);
        frames.push_back(frame);
    }
    CSharedSequenceData sequenceData = std::make_shared<CSequenceData>(m_filename,
                                                                       30,
                                                                       frames);
    
    IResourceSerializer::closeStream(filestream);
    m_status = E_SERIALIZER_STATUS_SUCCESS;
    IResourceSerializer::onResourceDataSerializationFinished(sequenceData);
}