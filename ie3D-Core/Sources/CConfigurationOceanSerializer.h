//
//  CConfigurationOceanSerializer.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/24/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationOceanSerializer_h
#define CConfigurationOceanSerializer_h

#include "IConfigurationSerializer.h"

class CConfigurationOceanSerializer final : public IConfigurationSerializer
{
private:
    
protected:
    
public:
    
    CConfigurationOceanSerializer(void);
    ~CConfigurationOceanSerializer(void);
    
    ISharedConfiguration serialize(const std::string& filename);
    void deserialize(const std::string& filename,
                     ISharedConfigurationRef configuration);
};

#endif
