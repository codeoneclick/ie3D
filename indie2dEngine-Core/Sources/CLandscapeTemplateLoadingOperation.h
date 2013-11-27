//
//  CLandscapeTemplateLoadingOperation.h
//  indieEngineCore
//
//  Created by Sergey Sergeev on 11/25/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CLandscapeTemplateLoadingOperation_h
#define CLandscapeTemplateLoadingOperation_h

#include "ITemplateLoadingOperation.h"

class CLandscapeTemplateLoadingOperation : public ITemplateLoadingOperation
{
private:
    
protected:
    
public:
    
    CLandscapeTemplateLoadingOperation(void);
    ~CLandscapeTemplateLoadingOperation(void);
    
    std::shared_ptr<ITemplate> Serialize(const std::string& _filename);
};


#endif
