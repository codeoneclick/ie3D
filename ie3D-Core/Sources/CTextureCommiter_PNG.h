//
//  CTextureCommiter_PNG.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 7/10/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef CTextureCommiter_PNG_h
#define CTextureCommiter_PNG_h

#include "IResourceCommiter.h"

class CTextureCommiter_PNG final : public IResourceCommiter
{
private:
    
protected:
    
public:
    
    CTextureCommiter_PNG(const std::string& guid, ISharedResourceRef resource);
    ~CTextureCommiter_PNG(void);
    
    void commit(void);
};

#endif
