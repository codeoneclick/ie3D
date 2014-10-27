//
//  CMEConfigurationPlacementModelSerializer.h
//  ie3D-EMap
//
//  Created by sergey.sergeev on 10/27/14.
//
//

#ifndef CMEConfigurationPlacementModelSerializer_h
#define CMEConfigurationPlacementModelSerializer_h

#include "IConfigurationSerializer.h"

class CMEConfigurationPlacementModelSerializer final : public IConfigurationSerializer
{
private:
    
protected:
    
public:
    
    CMEConfigurationPlacementModelSerializer(void);
    ~CMEConfigurationPlacementModelSerializer(void);
    
    ISharedConfiguration serialize(const std::string& filename);
    void deserialize(const std::string& filename,
                     ISharedConfigurationRef configuration);
};

#endif
