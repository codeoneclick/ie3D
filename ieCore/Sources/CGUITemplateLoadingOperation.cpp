//
//  CGUITemplateLoadingOperation.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CGUITemplateLoadingOperation.h"
#include "CMaterialTemplateLoadingOperation.h"
#include "CGUITemplateSerializer.h"

CGUITemplateLoadingOperation::CGUITemplateLoadingOperation(E_GUI_LOADING_SCHEME _scheme) :
m_scheme(_scheme)
{
    
}

CGUITemplateLoadingOperation::~CGUITemplateLoadingOperation(void)
{
    
}

std::shared_ptr<I_RO_TemplateCommon> CGUITemplateLoadingOperation::Serialize(const std::string& _filename)
{
	/*std::shared_ptr<CGUITemplateSerializer> serializer = std::make_shared<CGUITemplateSerializer>(m_scheme);
	std::shared_ptr<SGUIControlTemplate> guicontrolTemplate = std::static_pointer_cast<SGUIControlTemplate>(serializer->Serialize(_filename));
    assert(guicontrolTemplate != nullptr);
    std::shared_ptr<CMaterialTemplateLoadingOperation> materialLoadingOperation = std::make_shared<CMaterialTemplateLoadingOperation>();
	std::shared_ptr<SMaterialTemplate> materialTemplate = std::static_pointer_cast<SMaterialTemplate>(materialLoadingOperation->Serialize(guicontrolTemplate->m_materialsFilename));
    assert(materialTemplate != nullptr);
    guicontrolTemplate->m_materialsTemplate = materialTemplate;
    return guicontrolTemplate;*/
    return nullptr;
}