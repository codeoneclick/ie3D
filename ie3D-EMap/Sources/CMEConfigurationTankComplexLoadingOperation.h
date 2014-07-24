//
//  CMEConfigurationTankComplexLoadingOperation.h
//  ie3D-EMap
//
//  Created by sergey.sergeev on 7/23/14.
//
//

#ifndef CMEConfigurationTankComplexLoadingOperation_h
#define CMEConfigurationTankComplexLoadingOperation_h

#include "IConfigurationLoadingOperation.h"

class CMEConfigurationTankComplexLoadingOperation : public IConfigurationLoadingOperation
{
private:
    
protected:
    
public:
    
    CMEConfigurationTankComplexLoadingOperation(void);
    ~CMEConfigurationTankComplexLoadingOperation(void);
    
    ISharedConfiguration serialize(const std::string& filename);
};


#endif
