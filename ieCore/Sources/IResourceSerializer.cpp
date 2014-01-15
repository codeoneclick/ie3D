//
//  IResourceSerializer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/16/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IResourceSerializer.h"
#include "CResourceAccessor.h"
#include "CCommonOS.h"

IResourceSerializer::IResourceSerializer(const std::string& _guid, std::shared_ptr<IResource> _resource) :
m_guid(_guid),
m_resource(_resource),
m_status(E_SERIALIZER_STATUS_UNKNOWN)
{
    
}

IResourceSerializer::~IResourceSerializer(void)
{
    
}

std::istream* IResourceSerializer::_LoadData(const std::string &_filename)
{
    std::string path_filename(Get_BundlePath());
    path_filename.append(_filename);
#if defined(__NDK__)
    std::memstream* mstream;
    AAssetManager* manager = CResourceAccessor::Get_AAssetManager();
    AAsset* asset = AAssetManager_open(manager, _filename.c_str(), AASSET_MODE_UNKNOWN);
    if(asset == nullptr)
    {
        asset = AAssetManager_open(manager, path_filename.c_str(), AASSET_MODE_UNKNOWN);
    }
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
    std::ifstream* filestream = new std::ifstream();
    filestream->open(_filename.c_str());
    if (!filestream->is_open())
    {
        filestream->open(path_filename);
        if (!filestream->is_open())
        {
            m_status = E_SERIALIZER_STATUS_FAILURE;
            assert(false);
        }
    }
    return filestream;
#endif
}

void IResourceSerializer::_FreeData(std::istream *_stream)
{
#if defined(__NDK__)

#else
    static_cast<std::ifstream*>(_stream)->close();
#endif
    delete _stream;
    _stream = nullptr;
}