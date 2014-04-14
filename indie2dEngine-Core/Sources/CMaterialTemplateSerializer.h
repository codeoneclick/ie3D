//
//  CConfigurationMaterialSerializer.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationMaterialSerializer_h
#define CConfigurationMaterialSerializer_h

#include "ITemplateSerializer.h"

class CConfigurationMaterialSerializer final : public ITemplateSerializer
{
private:
    
protected:
    
public:
    
    CConfigurationMaterialSerializer(void);
    ~CConfigurationMaterialSerializer(void);
    
    CSharedTemplateCommon serialize(const std::string& filename);
    void deserialize(const std::string& _filename, std::shared_ptr<I_RO_TemplateCommon> _template);
};

#endif 
