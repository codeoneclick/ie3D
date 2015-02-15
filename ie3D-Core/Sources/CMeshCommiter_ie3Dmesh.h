//
//  CMeshCommiter_ie3Dmesh.h
//  ie3D-Core
//
//  Created by Sergey Sergeev on 2/15/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#ifndef CMeshCommiter_ie3Dmesh_h
#define CMeshCommiter_ie3Dmesh_h

#include "IResourceCommiter.h"

class CMeshCommiter_ie3Dmesh final : public IResourceCommiter
{
private:
    
protected:
    
public:
    
    CMeshCommiter_ie3Dmesh(const std::string& guid, ISharedResourceRef resource);
    ~CMeshCommiter_ie3Dmesh(void);
    
    void commit(void);
};

#endif
