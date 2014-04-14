//
//  CConfigurationLandscapeLoader.h
//  indieEngineCore
//
//  Created by Sergey Sergeev on 11/25/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationLandscapeLoader_h
#define CConfigurationLandscapeLoader_h

#include "ITemplateLoader.h"

class ITemplateLoadingHandler;
class CConfigurationLandscapeLoader final : public ITemplateLoader
{
private:
    
protected:
    
public:
    
    CConfigurationLandscapeLoader(void);
    ~CConfigurationLandscapeLoader(void);
    
    void Load(const std::string& _filename, std::shared_ptr<ITemplateLoadingHandler> _handler);
};


#endif
