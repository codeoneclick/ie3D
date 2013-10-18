//
//  CGUITemplateSerializer.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CGUITemplateSerializer.h"
#include "CCommonOS.h"
#include "ITemplate.h"

CGUITemplateSerializer::CGUITemplateSerializer(E_GUI_LOADING_SCHEME _scheme) :
m_scheme(_scheme)
{
    
}

CGUITemplateSerializer::~CGUITemplateSerializer(void)
{
    
}

std::shared_ptr<ITemplate> CGUITemplateSerializer::Serialize(const std::string& _filename)
{
    std::string path(Get_BundlePath());
    path.append(_filename);
    pugi::xml_document document;
    pugi::xml_parse_result result = document.load_file(path.c_str());
    assert(result.status == pugi::status_ok);
    pugi::xml_node node = document.child("control");
   
	return nullptr;
}

std::shared_ptr<ITemplate> CGUITemplateSerializer::Serialize(const std::string& _host, ui32 _port, const std::string& _filename)
{
    return nullptr;
}
