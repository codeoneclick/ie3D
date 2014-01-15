//
//  IFabricator.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/28/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef IFabricator_h
#define IFabricator_h

#include "HCommon.h"

class CTemplateAccessor;
class CResourceAccessor;
class IScreenSpaceTextureAccessor;

class IFabricator
{
private:

protected:
    
    std::shared_ptr<CTemplateAccessor> m_templateAccessor;
    std::shared_ptr<CResourceAccessor> m_resourceAccessor;
	std::shared_ptr<IScreenSpaceTextureAccessor> m_screenSpaceTextureAccessor;
	
	inline void _Set_ScreenSpaceTextureAccessor(std::shared_ptr<IScreenSpaceTextureAccessor> _screenSpaceTextureAccessor)
	{
		m_screenSpaceTextureAccessor = _screenSpaceTextureAccessor;
	};

public:
    
    IFabricator(const std::shared_ptr<CTemplateAccessor>& _templateAccessor, const std::shared_ptr<CResourceAccessor>& _resourceAccessor);
    virtual ~IFabricator(void);
    
};

#endif
