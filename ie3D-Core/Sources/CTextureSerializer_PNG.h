//
//  CTextureSerializer_PNG.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 7/10/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef CTextureSerializer_PNG_h
#define CTextureSerializer_PNG_h

#include "IResourceSerializer.h"

class CTextureSerializer_PNG final : public IResourceSerializer
{
private:
    
protected:
    
    std::string m_filename;
    
public:
    
    CTextureSerializer_PNG(const std::string& filename,
                           ISharedResourceRef resource);
    ~CTextureSerializer_PNG(void);
    
    void serialize(void);
};


#endif
