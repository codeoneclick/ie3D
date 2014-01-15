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
#include "IFabricator.h"

class IGUIControl;
class CGUIContainer;
class CGUIButton;

class CGUIFabricator : public virtual IFabricator
{
private:

protected:

	 std::set<std::shared_ptr<IGUIControl> > m_controlsContainer;

public:

	CGUIFabricator(const std::shared_ptr<CTemplateAccessor>& _templateAccessor, const std::shared_ptr<CResourceAccessor>& _resourceAccessor);
	~CGUIFabricator(void);

	std::shared_ptr<CGUIContainer> CreateGUIContainer(const std::string& _filename);
    void DeleteGUIContainer(const std::shared_ptr<CGUIContainer>& _control);
    
    std::shared_ptr<CGUIButton> CreateGUIButton(const std::string& _filename);
    void DeleteGUIButton(const std::shared_ptr<CGUIButton>& _control);
};

#endif 
