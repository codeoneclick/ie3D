//
//  CScreenSpaceOperationTemplateSerializer.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CScreenSpaceRenderOperationTemplateSerializer_h
#define CScreenSpaceRenderOperationTemplateSerializer_h

#include "ITemplateSerializer.h"

class CScreenSpaceRenderOperationTemplateSerializer final : public ITemplateSerializer
{
private:
    
protected:
    
public:
    
    CScreenSpaceRenderOperationTemplateSerializer(void);
    ~CScreenSpaceRenderOperationTemplateSerializer(void);
    
    std::shared_ptr<ITemplate> Serialize(const std::string& _filename);
    std::shared_ptr<ITemplate> Serialize(const std::string& _host, ui32 _port, const std::string& _filename);
};

#endif


