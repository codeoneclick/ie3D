//
//  CMEConfigurationModelBrushSerializer.h
//  ie3D-EMap
//
//  Created by sergey.sergeev on 10/27/14.
//
//

#ifndef CMEConfigurationModelBrushSerializer_h
#define CMEConfigurationModelBrushSerializer_h

#include "IConfigurationSerializer.h"

class CMEConfigurationModelBrushSerializer final : public IConfigurationSerializer
{
private:
    
protected:
    
public:
    
    CMEConfigurationModelBrushSerializer(void);
    ~CMEConfigurationModelBrushSerializer(void);
    
    ISharedConfiguration serialize(const std::string& filename);
    void deserialize(const std::string& filename,
                     ISharedConfigurationRef configuration);
};

#endif
