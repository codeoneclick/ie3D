//
//  CWorldSpaceOperationTemplateSerializer.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationWSRenderOperationSerializer_h
#define CConfigurationWSRenderOperationSerializer_h

#include "IConfigurationSerializer.h"

class CConfigurationWSRenderOperationSerializer final : public IConfigurationSerializer
{
private:
    
protected:
    
public:
    
    CConfigurationWSRenderOperationSerializer(void);
    ~CConfigurationWSRenderOperationSerializer(void);
    
    ISharedConfiguration serialize(const std::string& filename);
    void deserialize(const std::string& filename, ISharedConfigurationRef configuration);
};

#endif
