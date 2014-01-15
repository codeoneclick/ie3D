//
//  COceanTemplateLoader.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/24/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef COceanTemplateLoader_h
#define COceanTemplateLoader_h

#include "ITemplateLoader.h"

class ITemplateLoadingHandler;
class COceanTemplateLoader final : public ITemplateLoader
{
private:
    
protected:
    
public:
    
    COceanTemplateLoader(void);
    ~COceanTemplateLoader(void);
    
    void Load(const std::string& _filename, std::shared_ptr<ITemplateLoadingHandler> _handler);
};


#endif
