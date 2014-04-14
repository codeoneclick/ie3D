//
//  ITemplateSerializer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "ITemplateSerializer.h"
#include "CResourceAccessor.h"
#include "CCommonOS.h"

ITemplateSerializer::ITemplateSerializer(void)
{
    
}

ITemplateSerializer::~ITemplateSerializer(void)
{
    
}

pugi::xml_parse_result ITemplateSerializer::openXMLDocument(pugi::xml_document &document, const std::string &filename)
{
    pugi::xml_parse_result result;
#if defined(__NDK__)
    std::memstream* mstream;
    AAssetManager* manager = CResourceAccessor::Get_AAssetManager();
    AAsset* asset = AAssetManager_open(manager, filename.c_str(), AASSET_MODE_UNKNOWN);
    if(asset != nullptr)
    {
        ui32 size = AAsset_getLength(asset);
        char* buffer = new char[size];
        if(buffer != nullptr)
        {
            AAsset_read(asset, buffer, size);
        }
        mstream = new std::memstream(buffer, size);
        std::stringstream stringstream;
        stringstream<<mstream->rdbuf();
        std::string content(stringstream.str());
        AAsset_close(asset);
        result = _document.load(content.c_str());
    }
#else
    result = document.load_file(filename.c_str());
    if(result.status == pugi::status_file_not_found)
    {
        result = document.load_file(filename.c_str());
    }
#endif
    return result;
}

