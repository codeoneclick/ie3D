//
//  IFabricator.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/28/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IFabricator.h"

IFabricator::IFabricator(const std::shared_ptr<CTemplateAccessor>& _templateAccessor, const std::shared_ptr<CResourceAccessor>& _resourceAccessor) :
m_templateAccessor(_templateAccessor),
m_resourceAccessor(_resourceAccessor)
{

}


IFabricator::~IFabricator(void)
{

}

