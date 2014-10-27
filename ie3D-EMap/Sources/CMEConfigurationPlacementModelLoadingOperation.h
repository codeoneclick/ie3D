//
//  CMEConfigurationPlacementModelLoadingOperation.h
//  ie3D-EMap
//
//  Created by sergey.sergeev on 10/27/14.
//
//

#ifndef CMEConfigurationPlacementModelLoadingOperation_h
#define CMEConfigurationPlacementModelLoadingOperation_h

#include "IConfigurationLoadingOperation.h"

class CMEConfigurationPlacementModelLoadingOperation : public IConfigurationLoadingOperation
{
private:
    
protected:
    
public:
    
    CMEConfigurationPlacementModelLoadingOperation(void);
    ~CMEConfigurationPlacementModelLoadingOperation(void);
    
    ISharedConfiguration serialize(const std::string& filename);
};

#endif
