//
//  CConfigurationLandscapeLoadingOperation.h
//  indieEngineCore
//
//  Created by Sergey Sergeev on 11/25/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationLandscapeLoadingOperation_h
#define CConfigurationLandscapeLoadingOperation_h

#include "ITemplateLoadingOperation.h"

class CConfigurationLandscapeLoadingOperation : public ITemplateLoadingOperation
{
private:
    
protected:
    
public:
    
    CConfigurationLandscapeLoadingOperation(void);
    ~CConfigurationLandscapeLoadingOperation(void);
    
    std::shared_ptr<I_RO_TemplateCommon> Serialize(const std::string& _filename);
};


#endif
