//
//  CWorldSpaceOperationTemplateSerializer.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CWorldSpaceRenderOperationTemplateSerializer_h
#define CWorldSpaceRenderOperationTemplateSerializer_h

#include "ITemplateSerializer.h"

class CWorldSpaceRenderOperationTemplateSerializer final : public ITemplateSerializer
{
private:
    
protected:
    
public:
    
    CWorldSpaceRenderOperationTemplateSerializer(void);
    ~CWorldSpaceRenderOperationTemplateSerializer(void);
    
    std::shared_ptr<ITemplate> Serialize(const std::string& _filename);
    std::shared_ptr<ITemplate> Serialize(const std::string& _host, ui32 _port, const std::string& _filename);
};

#endif