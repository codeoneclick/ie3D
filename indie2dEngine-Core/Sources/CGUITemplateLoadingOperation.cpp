//
//  CGUITemplateLoadingOperation.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CGUITemplateLoadingOperation.h"
#include "CGUITemplateSerializer.h"
#include "ITemplate.h"

CGUITemplateLoadingOperation::CGUITemplateLoadingOperation(E_GUI_LOADING_SCHEME _scheme) :
m_scheme(_scheme)
{
    
}

CGUITemplateLoadingOperation::~CGUITemplateLoadingOperation(void)
{
    
}

std::shared_ptr<ITemplate> CGUITemplateLoadingOperation::Serialize(const std::string& _filename)
{
	std::shared_ptr<CGUITemplateSerializer> serializer = std::make_shared<CGUITemplateSerializer>(m_scheme);
    return serializer->Serialize(_filename);
}