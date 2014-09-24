//
//  CConfigurationAtmosphericScatteringSerializer.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 9/24/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationAtmosphericScatteringSerializer_h
#define CConfigurationAtmosphericScatteringSerializer_h

#include "IConfigurationSerializer.h"

class CConfigurationAtmosphericScatteringSerializer final : public IConfigurationSerializer
{
private:
    
protected:
    
public:
    
    CConfigurationAtmosphericScatteringSerializer(void);
    ~CConfigurationAtmosphericScatteringSerializer(void);
    
    ISharedConfiguration serialize(const std::string& filename);
    void deserialize(const std::string& filename,
                     ISharedConfigurationRef configuration);
};


#endif
