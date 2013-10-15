//
//  ITemplateSerializer.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef ITemplateSerializer_h
#define ITemplateSerializer_h

#include "HCommon.h"

#define USE_PROXY_SERVER 1

class ITemplate;
class ITemplateSerializer
{
private:
    
protected:

#if defined(__USE_CURL__)
    
    CURL* m_context;
    
#endif
    std::string m_message;
    
    bool _RegisterCurlContext(void);
    void _UnregisterCurlContext(void);
    
    static size_t _Callback(char* _data, size_t _size, size_t _nmemb, void *userdata);
    
public:
    
    ITemplateSerializer(void);
    virtual ~ITemplateSerializer(void);
    
    virtual std::shared_ptr<ITemplate> Serialize(const std::string& _filename) = 0;
    virtual std::shared_ptr<ITemplate> Serialize(const std::string& _host, ui32 _port, const std::string& _filename) = 0;
};

#endif
