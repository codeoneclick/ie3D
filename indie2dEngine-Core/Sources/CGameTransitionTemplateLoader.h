//
//  CConfigurationGameTransitionLoader.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationGameTransitionLoader_h
#define CConfigurationGameTransitionLoader_h

#include "ITemplateLoader.h"

class ITemplateLoadingHandler;
class CConfigurationGameTransitionLoader final : public ITemplateLoader
{
private:
    
protected:
    
public:
    
    CConfigurationGameTransitionLoader(void);
    ~CConfigurationGameTransitionLoader(void);
    
    void Load(const std::string& _filename, std::shared_ptr<ITemplateLoadingHandler> _handler);
};

#endif 
