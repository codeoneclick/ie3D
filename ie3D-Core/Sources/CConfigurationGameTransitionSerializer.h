//
//  CConfigurationGameTransitionSerializer.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationGameTransitionSerializer_h
#define CConfigurationGameTransitionSerializer_h

#include "IConfigurationSerializer.h"

class CConfigurationGameTransitionSerializer final : public IConfigurationSerializer
{
private:
    
protected:
    
public:
    
    CConfigurationGameTransitionSerializer(void);
    ~CConfigurationGameTransitionSerializer(void);
    
    ISharedConfiguration serialize(const std::string& filename);
    void deserialize(const std::string& filename, ISharedConfigurationRef configuration);
};

#endif 

