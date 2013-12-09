//
//  CMaterialTemplateSerializer.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CMaterialTemplateSerializer_h
#define CMaterialTemplateSerializer_h

#include "ITemplateSerializer.h"

class CMaterialTemplateSerializer final : public ITemplateSerializer
{
private:
    
protected:
    
public:
    
    CMaterialTemplateSerializer(void);
    ~CMaterialTemplateSerializer(void);
    
    std::shared_ptr<ITemplate> Serialize(const std::string& _filename);
    std::shared_ptr<ITemplate> Serialize(const std::string& _host, ui32 _port, const std::string& _filename);
    void Deserialize(const std::string& _filename, std::shared_ptr<ITemplate> _template);
};

#endif 
