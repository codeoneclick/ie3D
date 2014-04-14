//
//  CScreenSpaceOperationTemplateSerializer.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationSSRenderOperationSerializer_h
#define CConfigurationSSRenderOperationSerializer_h

#include "IConfigurationSerializer.h"

class CConfigurationSSRenderOperationSerializer final : public IConfigurationSerializer
{
private:
    
protected:
    
public:
    
    CConfigurationSSRenderOperationSerializer(void);
    ~CConfigurationSSRenderOperationSerializer(void);
    
    ISharedConfiguration serialize(const std::string& filename);
    void deserialize(const std::string& filename,
                     ISharedConfigurationRef configuration);
};

#endif


