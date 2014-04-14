//
//  CConfigurationModelLoader.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationModelLoader_h
#define CConfigurationModelLoader_h

#include "ITemplateLoader.h"

class ITemplateLoadingHandler;
class CConfigurationModelLoader final : public ITemplateLoader
{
private:
    
protected:
    
public:
    
    CConfigurationModelLoader(void);
    ~CConfigurationModelLoader(void);
    
    void Load(const std::string& _filename, std::shared_ptr<ITemplateLoadingHandler> _handler);
};


#endif 
