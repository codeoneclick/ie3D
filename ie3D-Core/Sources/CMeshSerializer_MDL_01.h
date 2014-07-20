//
//  CMeshSerializer_MDL_01.h
//  ie3D-Core
//
//  Created by Sergey Sergeev on 7/19/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef CMeshSerializer_MDL_01_h
#define CMeshSerializer_MDL_01_h

#include "IResourceSerializer.h"

class CMeshSerializer_MDL_01 final : public IResourceSerializer
{
private:
    
protected:
    
    std::string m_filename;
    
public:
    
    CMeshSerializer_MDL_01(const std::string& filename, ISharedResourceRef resource);
    ~CMeshSerializer_MDL_01(void);
    
    void serialize(void);
};


#endif 
