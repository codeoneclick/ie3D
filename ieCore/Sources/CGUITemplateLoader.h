//
//  CGUITemplateLoader.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CGUITemplateLoader_h
#define CGUITemplateLoader_h

#include "ITemplateLoader.h"
#include "HEnums.h"

class ITemplateLoadingHandler;
class CGUITemplateLoader final : public ITemplateLoader
{
private:
    
protected:
    
public:
    
    CGUITemplateLoader(void);
    ~CGUITemplateLoader(void);
    
	void Load(const std::string& _filename, std::shared_ptr<ITemplateLoadingHandler> _handler);
	void Load(const std::string& _filename, std::shared_ptr<ITemplateLoadingHandler> _handler, E_GUI_LOADING_SCHEME _scheme);
};


#endif 
