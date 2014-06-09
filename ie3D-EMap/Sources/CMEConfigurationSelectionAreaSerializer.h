//
//  CMEConfigurationSelectionAreaSerializer.h
//  ie3D-Demo
//
//  Created by Sergey Sergeev on 6/7/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef CMEConfigurationSelectionAreaSerializer_h
#define CMEConfigurationSelectionAreaSerializer_h

#include "IConfigurationSerializer.h"

class CMEConfigurationSelectionAreaSerializer final : public IConfigurationSerializer
{
private:
    
protected:
    
public:
    
    CMEConfigurationSelectionAreaSerializer(void);
    ~CMEConfigurationSelectionAreaSerializer(void);
    
    ISharedConfiguration serialize(const std::string& filename);
    void deserialize(const std::string& filename,
                     ISharedConfigurationRef configuration);
};


#endif
