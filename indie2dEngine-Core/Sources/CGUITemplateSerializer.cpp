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
	std::shared_ptr<SGUIControlTemplate> guicontrolTemplate = nullptr;

	switch (m_scheme)
	{
	case E_GUI_LOADING_SCHEME_NONE:
		{
			assert(false);
		}
		break;
	case E_GUI_LOADING_SCHEME_CONTAINER:
		{
			guicontrolTemplate = std::make_shared<SGUIContainerTemplate>();
			std::static_pointer_cast<SGUIContainerTemplate>(guicontrolTemplate)->m_isBatching = node.attribute("is_batching").as_bool();
			std::static_pointer_cast<SGUIContainerTemplate>(guicontrolTemplate)->m_width = node.child("width").attribute("value").as_float();
			std::static_pointer_cast<SGUIContainerTemplate>(guicontrolTemplate)->m_height = node.child("height").attribute("value").as_float();
		}
		break;
	case E_GUI_LOADING_SCHEME_BUTTON:
		{

		}
		break;
	default:
		{
			assert(false);
		}
		break;
	}

	guicontrolTemplate->m_materialsFilename = node.child("material").attribute("filename").as_string();
	return guicontrolTemplate;
}

std::shared_ptr<ITemplate> CGUITemplateSerializer::Serialize(const std::string& _host, ui32 _port, const std::string& _filename)
{
    return nullptr;
}
