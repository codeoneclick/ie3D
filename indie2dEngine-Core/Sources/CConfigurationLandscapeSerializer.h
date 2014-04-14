//
//  CConfigurationLandscapeSerializer.h
//  indieEngineCore
//
//  Created by Sergey Sergeev on 11/25/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationLandscapeSerializer_h
#define CConfigurationLandscapeSerializer_h

#include "IConfigurationSerializer.h"

class CConfigurationLandscapeSerializer final : public IConfigurationSerializer
{
private:
    
protected:
    
public:
    
    CConfigurationLandscapeSerializer(void);
    ~CConfigurationLandscapeSerializer(void);
    
    ISharedConfiguration serialize(const std::string& filename);
    void deserialize(const std::string& filename,
                     ISharedConfigurationRef configuration);
};

#endif
