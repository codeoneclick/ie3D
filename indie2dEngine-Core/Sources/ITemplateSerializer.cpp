//
//  ITemplateSerializer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "ITemplateSerializer.h"

ITemplateSerializer::ITemplateSerializer(void) :
m_context(nullptr)
{
    
}

ITemplateSerializer::~ITemplateSerializer(void)
{
    
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
    return false;
}

void ITemplateSerializer::_UnregisterCurlContext(void)
{
    if(m_context)
    {
        curl_easy_cleanup(m_context);
    }
    curl_global_cleanup();
}

