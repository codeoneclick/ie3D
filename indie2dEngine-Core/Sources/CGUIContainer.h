//
//  CGUIContainer.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 10/10/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CGUIContainer_h
#define CGUIContainer_h

#include "HCommon.h"
#include "IGUIControl.h"

class CGUIContainer : public IGUIControl
{
private:

protected:

	std::vector<std::shared_ptr<IGUIControl>> m_controls;

	void _OnSceneUpdate(f32 _deltatime);
    
    void _OnBind(const std::string& _mode);
    void _OnDraw(const std::string& _mode);
    void _OnUnbind(const std::string& _mode);
    void _OnBatch(const std::string& _mode);
    
    virtual void _OnResourceLoaded(std::shared_ptr<IResource> _resource, bool _success);
    virtual void _OnTemplateLoaded(std::shared_ptr<I_RO_TemplateCommon> _template);

public:

	CGUIContainer(const std::shared_ptr<CResourceAccessor>& _resourceAccessor, const std::shared_ptr<IScreenSpaceTextureAccessor>& _screenSpaceTextureAccessor);
	~CGUIContainer(void);

	void RegisterControl(const std::shared_ptr<IGUIControl>& _control);
	void UnregisterControl(const std::shared_ptr<IGUIControl>& _control);
};

#endif