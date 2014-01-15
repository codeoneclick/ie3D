//
//  CGUITemplateLoadingOperation.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CGUITemplateLoadingOperation_h
#define CGUITemplateLoadingOperation_h

#include "ITemplateLoadingOperation.h"
#include "HEnums.h"

class CGUITemplateLoadingOperation : public ITemplateLoadingOperation
{
private:
    
protected:
    
	E_GUI_LOADING_SCHEME m_scheme;

public:
    
    CGUITemplateLoadingOperation(E_GUI_LOADING_SCHEME _scheme);
    ~CGUITemplateLoadingOperation(void);
    
    std::shared_ptr<I_RO_TemplateCommon> Serialize(const std::string& _filename);
};

#endif 
