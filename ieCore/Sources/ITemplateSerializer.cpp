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
#if defined(__USE_CURL__)
: m_context(nullptr)
#endif
{
    
}

ITemplateSerializer::~ITemplateSerializer(void)
{
    
}

pugi::xml_parse_result ITemplateSerializer::_LoadDocument(pugi::xml_document &_document, const std::string &_filename)
{
    pugi::xml_parse_result result;
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
    result = _document.load_file(_filename.c_str());
    if(result.status == pugi::status_file_not_found)
    {
        result = _document.load_file(path_filename.c_str());
    }
#endif
    return result;
}

size_t ITemplateSerializer::_Callback(char* _data, size_t _size, size_t _nmemb, void *userdata)
{
    const size_t size = _size * _nmemb;
    std::string strdata(_data, size);
    std::string* message = static_cast<std::string* >(userdata);
    message->append(strdata);
	return size;
}

bool ITemplateSerializer::_RegisterCurlContext(void)
{
#if defined(__USE_CURL__)
    curl_global_init(CURL_GLOBAL_DEFAULT);
    m_context = curl_easy_init();
    if(m_context)
    {
        struct curl_slist* headers = nullptr;
        curl_slist_append(headers, "Accept: application/json");
        curl_slist_append(headers, "Content-Type: application/json");
        curl_slist_append(headers, "charsets: utf-8");
        curl_easy_setopt(m_context, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(m_context, CURLOPT_WRITEDATA, (void *)&m_message);
        
#ifdef USE_PROXY_SERVER
        curl_easy_setopt(m_context, CURLOPT_PROXY, "127.0.0.1:3129");
        curl_easy_setopt(m_context, CURLOPT_PROXYAUTH, CURLAUTH_ANY);
#endif
        curl_easy_setopt(m_context, CURLOPT_WRITEFUNCTION, ITemplateSerializer::_Callback);
        return true;
    }
#endif
    return false;
}

void ITemplateSerializer::_UnregisterCurlContext(void)
{
#ifdef USE_CURL
    if(m_context)
    {
        curl_easy_cleanup(m_context);
    }
    curl_global_cleanup();
#endif
}

