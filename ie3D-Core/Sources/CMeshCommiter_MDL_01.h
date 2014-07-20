//
//  CMeshCommiter_MDL_01.h
//  ie3D-Core
//
//  Created by Sergey Sergeev on 7/19/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef CMeshCommiter_MDL_01_h
#define CMeshCommiter_MDL_01_h

#include "IResourceCommiter.h"

class CMeshCommiter_MDL_01 final : public IResourceCommiter
{
private:
    
protected:
    
public:
    
    CMeshCommiter_MDL_01(const std::string& guid, ISharedResourceRef resource);
    ~CMeshCommiter_MDL_01(void);
    
    void commit(void);
};

#endif
