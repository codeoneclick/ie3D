//
//  IGameObjectTemplateSerializer.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/28/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef IGameObjectTemplateSerializer_h
#define IGameObjectTemplateSerializer_h

#include "HCommon.h"
#include "HTemplates.h"

class IGameObjectTemplateSerializer
{
private:
    
protected:
    
public:
    
    IGameObjectTemplateSerializer(void);
    virtual ~IGameObjectTemplateSerializer(void);
    
    std::vector<std::shared_ptr<SMaterialTemplate> > Deserialize(pugi::xml_node const& _templateNode);
    
};

#endif 
