//
//  CTextureCommiter_PVR.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CTextureCommiter_PVR_h
#define CTextureCommiter_PVR_h

#include "IResourceCommiter.h"

class CTextureCommiter_PVR final : public IResourceCommiter
{
private:
    
protected:
    
public:
    
    CTextureCommiter_PVR(const std::string& guid, ISharedResourceRef resource);
    ~CTextureCommiter_PVR(void);
    
    void commit(void);
};


#endif 
