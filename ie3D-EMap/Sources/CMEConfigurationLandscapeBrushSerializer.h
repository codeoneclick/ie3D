//
//  CMEConfigurationLandscapeBrushSerializer.h
//  ie3D-Demo
//
//  Created by Sergey Sergeev on 6/7/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef CMEConfigurationLandscapeBrushSerializer_h
#define CMEConfigurationLandscapeBrushSerializer_h

#include "IConfigurationSerializer.h"

class CMEConfigurationLandscapeBrushSerializer final : public IConfigurationSerializer
{
private:
    
protected:
    
public:
    
    CMEConfigurationLandscapeBrushSerializer(void);
    ~CMEConfigurationLandscapeBrushSerializer(void);
    
    ISharedConfiguration serialize(const std::string& filename);
    void deserialize(const std::string& filename,
                     ISharedConfigurationRef configuration);
};


#endif
