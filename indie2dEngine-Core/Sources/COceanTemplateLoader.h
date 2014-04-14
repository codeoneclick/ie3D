//
//  CConfigurationOceanLoader.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/24/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationOceanLoader_h
#define CConfigurationOceanLoader_h

#include "ITemplateLoader.h"

class ITemplateLoadingHandler;
class CConfigurationOceanLoader final : public ITemplateLoader
{
private:
    
protected:
    
public:
    
    CConfigurationOceanLoader(void);
    ~CConfigurationOceanLoader(void);
    
    void Load(const std::string& _filename, std::shared_ptr<ITemplateLoadingHandler> _handler);
};


#endif
