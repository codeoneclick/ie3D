//
//  CGameTransitionTemplateLoader.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CGameTransitionTemplateLoader_h
#define CGameTransitionTemplateLoader_h

#include "ITemplateLoader.h"

class ITemplateLoadingHandler;
class CGameTransitionTemplateLoader final : public ITemplateLoader
{
private:
    
protected:
    
public:
    
    CGameTransitionTemplateLoader(void);
    ~CGameTransitionTemplateLoader(void);
    
    void Load(const std::string& _filename, std::shared_ptr<ITemplateLoadingHandler> _handler);
};

#endif 
