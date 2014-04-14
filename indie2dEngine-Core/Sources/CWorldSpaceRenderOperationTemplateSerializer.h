//
//  CWorldSpaceOperationTemplateSerializer.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationWSRenderOperationSerializer_h
#define CConfigurationWSRenderOperationSerializer_h

#include "ITemplateSerializer.h"

class CConfigurationWSRenderOperationSerializer final : public ITemplateSerializer
{
private:
    
protected:
    
public:
    
    CConfigurationWSRenderOperationSerializer(void);
    ~CConfigurationWSRenderOperationSerializer(void);
    
    std::shared_ptr<I_RO_TemplateCommon> Serialize(const std::string& _filename);
    std::shared_ptr<I_RO_TemplateCommon> Serialize(const std::string& _host, ui32 _port, const std::string& _filename);
    void Deserialize(const std::string& _filename, std::shared_ptr<I_RO_TemplateCommon> _template);
};

#endif
