//
//  CGUIConfigurationLoadingOperation.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

/*#include "CGUIConfigurationLoadingOperation.h"
#include "CConfigurationMaterialLoadingOperation.h"
#include "CGUIConfigurationSerializer.h"

CGUIConfigurationLoadingOperation::CGUIConfigurationLoadingOperation(E_GUI_LOADING_SCHEME _scheme) :
m_scheme(_scheme)
{
    
}

CGUIConfigurationLoadingOperation::~CGUIConfigurationLoadingOperation(void)
{
    
}

std::shared_ptr<I_RO_TemplateCommon> CGUIConfigurationLoadingOperation::Serialize(const std::string& _filename)
{*/
	/*std::shared_ptr<CGUIConfigurationSerializer> serializer = std::make_shared<CGUIConfigurationSerializer>(m_scheme);
	std::shared_ptr<SGUIControlTemplate> guicontrolTemplate = std::static_pointer_cast<SGUIControlTemplate>(serializer->Serialize(_filename));
    assert(guicontrolTemplate != nullptr);
    std::shared_ptr<CConfigurationMaterialLoadingOperation> materialLoadingOperation = std::make_shared<CConfigurationMaterialLoadingOperation>();
	std::shared_ptr<SMaterialTemplate> materialTemplate = std::static_pointer_cast<SMaterialTemplate>(materialLoadingOperation->Serialize(guicontrolTemplate->m_materialsFilename));
    assert(materialTemplate != nullptr);
    guicontrolTemplate->m_materialsTemplate = materialTemplate;
    return guicontrolTemplate;*/
/*    return nullptr;
}*/