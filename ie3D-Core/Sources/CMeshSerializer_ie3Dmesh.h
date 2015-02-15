//
//  CMeshSerializer_ie3Dmesh.h
//  ie3D-Core
//
//  Created by Sergey Sergeev on 2/15/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#ifndef CMeshSerializer_ie3Dmesh_h
#define CMeshSerializer_ie3Dmesh_h

#include "IResourceSerializer.h"

class CMeshSerializer_ie3Dmesh final : public IResourceSerializer
{
private:
    
protected:
    
    std::string m_filename;
    
public:
    
    CMeshSerializer_ie3Dmesh(const std::string& filename, ISharedResourceRef resource);
    ~CMeshSerializer_ie3Dmesh(void);
    
    void serialize(void);
};

#endif
