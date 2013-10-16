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

public:

	CGUIContainer(const std::shared_ptr<CResourceAccessor>& _resourceAccessor);
	~CGUIContainer(void);
};

#endif