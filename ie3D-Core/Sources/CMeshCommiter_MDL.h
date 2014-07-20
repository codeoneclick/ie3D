//
//  CMeshCommiter_MDL.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/26/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CMeshCommiter_MDL_h
#define CMeshCommiter_MDL_h

#include "IResourceCommiter.h"

class CMeshCommiter_MDL final : public IResourceCommiter
{
private:
    
protected:
    
public:
    
    CMeshCommiter_MDL(const std::string& guid, ISharedResourceRef resource);
    ~CMeshCommiter_MDL(void);
    
    void commit(void);
};

#endif
