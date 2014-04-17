//
//  IResourceSerializer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/16/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IResourceSerializer.h"
#include "IResource.h"
#include "CCommonOS.h"

IResourceSerializer::IResourceSerializer(const std::string& guid, ISharedResourceRef resource) :
m_guid(guid),
m_resource(resource),
m_status(E_SERIALIZER_STATUS_UNKNOWN)
{
    
}

IResourceSerializer::~IResourceSerializer(void)
{
    
}

std::shared_ptr<std::istream> IResourceSerializer::openStream(const std::string &filename)
{
#if defined(__NDK__)
    std::memstream* mstream;
    AAsset* asset = AAssetManager_open(m_assetManager, filename.c_str(), AASSET_MODE_UNKNOWN);
    if(asset)
    {
        ui32 size = AAsset_getLength(asset);
        char* buffer = new char[size];
        if(buffer != nullptr)
        {
            AAsset_read(asset, buffer, size);
        }
        mstream = new std::memstream(buffer, size);
        AAsset_close(asset);
        return mstream;
    }
#else
    std::shared_ptr<std::ifstream> filestream = std::make_shared<std::ifstream>();
    filestream->open(bundlepath().append(filename).c_str());
    if (!filestream->is_open())
    {
        m_status = E_SERIALIZER_STATUS_FAILURE;
        assert(false);
    }
    return filestream;
#endif
}

void IResourceSerializer::closeStream(const std::shared_ptr<std::istream> &stream)
{
#if defined(__NDK__)

#else
    std::static_pointer_cast<std::ifstream>(stream)->close();
#endif
}

void IResourceSerializer::onResourceDataSerializationFinished(ISharedResourceDataRef resourceData)
{
    m_resource->onResourceDataSerializationFinished(resourceData);
}

std::string IResourceSerializer::getGuid(void) const
{
    return m_guid;
}

E_SERIALIZER_STATUS IResourceSerializer::getStatus(void) const
{
    return m_status;
}
