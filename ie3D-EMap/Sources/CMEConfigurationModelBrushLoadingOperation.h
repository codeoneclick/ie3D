//
//  CMEConfigurationModelBrushLoadingOperation.h
//  ie3D-EMap
//
//  Created by sergey.sergeev on 10/27/14.
//
//

#ifndef CMEConfigurationModelBrushLoadingOperation_h
#define CMEConfigurationModelBrushLoadingOperation_h

#include "IConfigurationLoadingOperation.h"

class CMEConfigurationModelBrushLoadingOperation : public IConfigurationLoadingOperation
{
private:
    
protected:
    
public:
    
    CMEConfigurationModelBrushLoadingOperation(void);
    ~CMEConfigurationModelBrushLoadingOperation(void);
    
    ISharedConfiguration serialize(const std::string& filename);
};

#endif
