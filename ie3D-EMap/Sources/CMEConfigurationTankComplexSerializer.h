//
//  CMEConfigurationTankComplexSerializer.h
//  ie3D-EMap
//
//  Created by sergey.sergeev on 7/23/14.
//
//

#ifndef CMEConfigurationTankComplexSerializer_h
#define CMEConfigurationTankComplexSerializer_h

#include "IConfigurationSerializer.h"

class CMEConfigurationTankComplexSerializer final : public IConfigurationSerializer
{
private:
    
protected:
    
public:
    
    CMEConfigurationTankComplexSerializer(void);
    ~CMEConfigurationTankComplexSerializer(void);
    
    ISharedConfiguration serialize(const std::string& filename);
    void deserialize(const std::string& filename,
                     ISharedConfigurationRef configuration);
};

#endif
