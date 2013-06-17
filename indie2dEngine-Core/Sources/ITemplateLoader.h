//
//  ITemplateLoader.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef ITemplateLoader_h
#define ITemplateLoader_h

#include "HCommon.h"

class ITemplateLoadingHandler;
class ITemplateLoader
{
private:
    
protected:
    
    virtual void _Load(const std::string& _filename, std::shared_ptr<ITemplateLoadingHandler> _handler) = 0;
    
public:
    
    ITemplateLoader(void);
    ~ITemplateLoader(void);
    
    virtual void Load(const std::string& _filename, std::shared_ptr<ITemplateLoadingHandler> _handler) = 0;
};

#endif 