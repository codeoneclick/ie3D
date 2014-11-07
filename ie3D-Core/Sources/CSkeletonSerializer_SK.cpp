//
//  CSkeletonSerializer_SK.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 8/13/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CSkeletonSerializer_SK.h"
#include "CMesh.h"
#include "CBone.h"

CSkeletonSerializer_SK::CSkeletonSerializer_SK(const std::string& filename,
                                               ISharedResourceRef resource) :
IResourceSerializer(filename, resource),
m_filename(filename)
{
    
}

CSkeletonSerializer_SK::~CSkeletonSerializer_SK(void)
{
    
}

void CSkeletonSerializer_SK::serialize(void)
{
    assert(m_resource != nullptr);
    m_status = E_SERIALIZER_STATUS_INPROGRESS;
    
    std::shared_ptr<std::istream> filestream = IResourceSerializer::openStream(m_filename, &m_status);
    
    ui32 numBones; i32 id, parentId;
    filestream->read((char*)&numBones, sizeof(i32));
    std::shared_ptr<CSkeletonData> skeletonData = std::make_shared<CSkeletonData>(numBones);
    
    for (ui32 i = 0; i < numBones; ++i)
    {
        filestream->read((char*)&id, sizeof(i32));
        filestream->read((char*)&parentId, sizeof(i32));
        skeletonData->addBone(id, parentId);
    }
    IResourceSerializer::closeStream(filestream);
    
    m_status = E_SERIALIZER_STATUS_SUCCESS;
    IResourceSerializer::onResourceDataSerializationFinished(skeletonData);
}