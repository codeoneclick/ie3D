//
//  CTextureSerializer_PVR.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/16/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CTextureSerializer_PVR_h
#define CTextureSerializer_PVR_h

#include "IResourceSerializer.h"

class CTextureSerializer_PVR final : public IResourceSerializer
{
private:
    
protected:
    
    std::string m_filename;
    
public:
    
    CTextureSerializer_PVR(const std::string& _filename, std::shared_ptr<IResource> _resource);
    ~CTextureSerializer_PVR(void);
    
    void Serialize(void);
};

#endif
