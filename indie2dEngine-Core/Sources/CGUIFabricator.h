//
//  CGUIFabricator.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 10/10/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CGUIFabricator_h
#define CGUIFabricator_h

#include "HCommon.h"

class IGUIControl;
class CTemplateAccessor;
class CResourceAccessor;
class CGUIContainer;
class CGUIButton;

class CGUIFabricator
{
private:

	std::shared_ptr<CTemplateAccessor> m_templateAccessor;
    std::shared_ptr<CResourceAccessor> m_resourceAccessor;

protected:

	 std::set<std::shared_ptr<IGUIControl> > m_controlsContainer;

public:

	CGUIFabricator(std::shared_ptr<CTemplateAccessor> _templateAccessor, std::shared_ptr<CResourceAccessor> _resourceAccessor);
	~CGUIFabricator(void);

	std::shared_ptr<CGUIContainer> CreateContainer(const std::string& _filename);
    void DeleteCamera(const std::shared_ptr<CGUIContainer>& _control);
    
    std::shared_ptr<CGUIButton> CreateButton(const std::string& _filename);
    void DeleteLight(const std::shared_ptr<CGUIButton>& _control);
};

#endif 
