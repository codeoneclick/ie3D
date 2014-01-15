//
//  CModelTemplateLoader.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CModelTemplateLoader_h
#define CModelTemplateLoader_h

#include "ITemplateLoader.h"

class ITemplateLoadingHandler;
class CModelTemplateLoader final : public ITemplateLoader
{
private:
    
protected:
    
public:
    
    CModelTemplateLoader(void);
    ~CModelTemplateLoader(void);
    
    void Load(const std::string& _filename, std::shared_ptr<ITemplateLoadingHandler> _handler);
};


#endif 
