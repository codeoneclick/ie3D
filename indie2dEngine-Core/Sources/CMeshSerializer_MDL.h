//
//  CMeshSerializer_MDL.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/26/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CMeshSerializer_MDL_h
#define CMeshSerializer_MDL_h

#include "IResourceSerializer.h"

class CMeshSerializer_MDL final : public IResourceSerializer
{
private:
    
protected:
    
    std::string m_filename;
    
public:
    
    CMeshSerializer_MDL(const std::string& _filename, std::shared_ptr<IResource> _resource);
    ~CMeshSerializer_MDL(void);
    
    void Serialize(void);
};

#endif
