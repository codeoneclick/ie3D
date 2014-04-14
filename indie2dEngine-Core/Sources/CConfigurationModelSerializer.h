//
//  CConfigurationModelSerializer.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/28/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationModelSerializer_h
#define CConfigurationModelSerializer_h

#include "IConfigurationSerializer.h"

class CConfigurationModelSerializer final : public IConfigurationSerializer
{
private:
    
protected:
    
public:
    
    CConfigurationModelSerializer(void);
    ~CConfigurationModelSerializer(void);
    
    ISharedConfiguration serialize(const std::string& filename);
    void deserialize(const std::string& filename,
                     ISharedConfigurationRef configuration);
};

#endif 
