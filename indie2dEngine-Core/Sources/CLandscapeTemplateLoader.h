//
//  CLandscapeTemplateLoader.h
//  indieEngineCore
//
//  Created by Sergey Sergeev on 11/25/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CLandscapeTemplateLoader_h
#define CLandscapeTemplateLoader_h

#include "ITemplateLoader.h"

class ITemplateLoadingHandler;
class CLandscapeTemplateLoader final : public ITemplateLoader
{
private:
    
protected:
    
public:
    
    CLandscapeTemplateLoader(void);
    ~CLandscapeTemplateLoader(void);
    
    void Load(const std::string& _filename, std::shared_ptr<ITemplateLoadingHandler> _handler);
};


#endif
