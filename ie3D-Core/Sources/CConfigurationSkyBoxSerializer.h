//
//  CConfigurationSkyBoxSerializer.h
//  ie3D-Core
//
//  Created by Sergey Sergeev on 5/6/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationSkyBoxSerializer_h
#define CConfigurationSkyBoxSerializer_h

#include "IConfigurationSerializer.h"

class CConfigurationSkyBoxSerializer final : public IConfigurationSerializer
{
private:
    
protected:
    
public:
    
    CConfigurationSkyBoxSerializer(void);
    ~CConfigurationSkyBoxSerializer(void);
    
    ISharedConfiguration serialize(const std::string& filename);
    void deserialize(const std::string& filename,
                     ISharedConfigurationRef configuration);
};

#endif
